using Npgsql;
using System.Collections.Concurrent;
using System.Threading;

public sealed class Pool : IDisposable
{
    private static readonly Lazy<Pool> _instance = new Lazy<Pool>(() => new Pool());
    private readonly ConcurrentBag<Connection> _availableConnections;
    private readonly ConcurrentQueue<TaskCompletionSource<Connection>> _waitingQueue;
    private readonly CancellationTokenSource _disposeTokenSource = new();
    private SemaphoreSlim _semaphore = null!;
    private int _activeConnections;
    private string _connectionString = null!;
    private int _maxConnections;
    private int _connectionIncrement;
    private static bool _isInitialized;

    public static Pool Instance =>
        _isInitialized ?
        _instance.Value :
        throw new Exception("Pool has not been initialized. Call Initialize() first");

    public static void Initialize(
        string connectionString,
        int startupConnections = 10,
        int maxConnections = 100,
        int connectionIncrement = 5
    )
    {
        if (_isInitialized)
            throw new InvalidOperationException("Pool is already initialized");

        _isInitialized = true;

        Instance._connectionString = connectionString;
        Instance._maxConnections = maxConnections;
        Instance._connectionIncrement = connectionIncrement;
        Instance._semaphore = new SemaphoreSlim(maxConnections, maxConnections);

        // Create startup connections
        for (int i = 0; i < startupConnections; i++)
        {
            Instance._availableConnections.Add(Instance.CreateNewConnection());
        }
    }

    private Pool()
    {
        _availableConnections = new ConcurrentBag<Connection>();
        _waitingQueue = new ConcurrentQueue<TaskCompletionSource<Connection>>();
    }

    public async Task<Connection> GetConnectionAsync(CancellationToken cancellationToken = default)
    {
        // Combine the pool's disposal token with the provided cancellation token
        using var linkedCts = CancellationTokenSource.CreateLinkedTokenSource(
            _disposeTokenSource.Token,
            cancellationToken
        );

        try
        {
            // Wait for a connection to become available
            await _semaphore.WaitAsync(linkedCts.Token);

            // Try to get an existing connection
            {
                if (_availableConnections.TryTake(out var connection))
                {
                    Interlocked.Increment(ref _activeConnections);
                    return connection;
                }
            }

            // If no connections available but we have capacity, create new ones
            if (_activeConnections < _maxConnections)
            {
                int connectionsToCreate = Math.Min(
                    _connectionIncrement, 
                    _maxConnections - _availableConnections.Count
                );

                for (int i = 0; i < connectionsToCreate; i++)
                {
                    _availableConnections.Add(CreateNewConnection());
                }

                if (_availableConnections.TryTake(out var connection))
                {
                    Interlocked.Increment(ref _activeConnections);
                    return connection;
                }
                else
                {
                    // Shouldn't get here due to connections being created previously
                    throw new Exception("Failed to return a newly created connection");
                }
            }

            // Shouldn't get here due to semaphore
            throw new InvalidOperationException("No connections available");
        }
        catch (OperationCanceledException)
        {
            throw new TimeoutException("Connection request canceled or pool is disposing");
        }
    }

    public void ReturnConnection(Connection connection)
    {
        if (connection == null) throw new ArgumentNullException(nameof(connection));

        // Check if anyone is waiting for a connection
        if (_waitingQueue.TryDequeue(out var tcs))
        {
            if (!tcs.TrySetResult(connection))
            {
                // If the waiting task was canceled, return to available pool
                _availableConnections.Add(connection);
                _semaphore.Release();
            }
        }
        else
        {
            // No waiters, return to available pool
            _availableConnections.Add(connection);
            _semaphore.Release();
        }

        Interlocked.Decrement(ref _activeConnections);
    }

    private Connection CreateNewConnection()
    {
        var npgsqlConnection = new NpgsqlConnection(_connectionString);
        npgsqlConnection.Open();
        return new Connection(npgsqlConnection);
    }

    public void Dispose()
    {
        _disposeTokenSource.Cancel();
        
        // Clean up all connections
        while (_availableConnections.TryTake(out var connection))
        {
            connection.Close();
        }

        _semaphore.Dispose();
    }
}