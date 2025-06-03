using Npgsql;
using System.Collections.Concurrent;
using System.Threading;

public sealed class Pool : IDisposable
{
    private static readonly Lazy<Pool> _instance = new Lazy<Pool>(() => new Pool());
    private readonly ConcurrentBag<Connection> _availableConnections;
    private readonly CancellationTokenSource _disposeTokenSource = new();
    private SemaphoreSlim _semaphore = null!;
    private readonly SemaphoreSlim _creationSemaphore;
    private int _activeConnections;
    private string _connectionString = null!;
    private int _maxConnections;
    private int _connectionIncrement;
    private static bool _isInitialized;

    public static Pool Instance =>
        _isInitialized ?
        _instance.Value :
        throw new Exception("Pool has not been initialized. Call Initialize() first");

    public static async Task Initialize(
        string connectionString,
        int startupConnections = 10,
        int maxConnections = 100,
        int connectionIncrement = 5
    )
    {
        if (_isInitialized)
            throw new InvalidOperationException("Pool is already initialized");

        _isInitialized = true;

        Instance._connectionString = connectionString + ";Pooling=false;SSL Mode=Disable;TCP Keepalive=true;Keepalive=60;No Reset On Close=true";
        Instance._maxConnections = maxConnections;
        Instance._connectionIncrement = connectionIncrement;
        Instance._semaphore = new SemaphoreSlim(maxConnections, maxConnections);

        // Create startup connections
        for (int i = 0; i < startupConnections; i++)
        {
            Connection conn = await Instance.CreateNewConnectionAsync();
            Instance._availableConnections.Add(conn);
        }
    }

    private Pool()
    {
        _availableConnections = new ConcurrentBag<Connection>();
        _creationSemaphore = new SemaphoreSlim(1, 1);
    }

    public async Task<Connection> GetConnectionAsync(CancellationToken cancellationToken = default)
    {
        using var linkedCts = CancellationTokenSource.CreateLinkedTokenSource(
            _disposeTokenSource.Token,
            cancellationToken
        );

        try
        {
            await _semaphore.WaitAsync(linkedCts.Token);

            for (int i = 0; i < 3; i++)
            {
                if (_availableConnections.TryTake(out var connection))
                {
                    if (await ValidateConnection(connection))
                    {
                        Interlocked.Increment(ref _activeConnections);
                        return connection;
                    }
                    connection.Close();
                }
            }

            // No available connections - create new ones if under capacity
            await _creationSemaphore.WaitAsync(cancellationToken);
            try
            {
                // Double-check after getting creation lock
                if (_availableConnections.TryTake(out var connection))
                {
                    if (await ValidateConnection(connection))
                    {
                        Interlocked.Increment(ref _activeConnections);
                        return connection;
                    }
                    connection.Close();
                }

                var remainingCapacity = _maxConnections - Volatile.Read(ref _activeConnections);
                if (remainingCapacity <= 0)
                    throw new InvalidOperationException("Pool exhausted");

                int newConnectionsToCreate = Math.Min(_connectionIncrement, remainingCapacity);

                var newConnections = new List<Connection>();
                for (int i = 0; i < newConnectionsToCreate; i++)
                {
                    var newConn = await CreateNewConnectionAsync();
                    newConnections.Add(newConn);

                    if (i != 0) _availableConnections.Add(newConn);
                }

                Interlocked.Increment(ref _activeConnections);
                return newConnections[0];
            }
            finally {
                _creationSemaphore.Release();
            }
        }
        catch (OperationCanceledException)
        {
            throw new TimeoutException("Connection request canceled or pool is disposing");
        }
    }

    private async Task<bool> ValidateConnection(Connection conn)
    {
        try
        {
            using var cmd = new NpgsqlCommand("SELECT 1", conn.DbConnection);
            await cmd.ExecuteScalarAsync();
            return true;
        }
        catch
        {
            return false;
        }
    }

    public void ReturnConnection(Connection connection)
    {
        if (connection == null) throw new ArgumentNullException(nameof(connection));

        _availableConnections.Add(connection);
        _semaphore.Release();
        Interlocked.Decrement(ref _activeConnections);
    }

    private async Task<Connection> CreateNewConnectionAsync()
    {
        var conn = new NpgsqlConnection(_connectionString);
        await conn.OpenAsync();
        return new Connection(conn);
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