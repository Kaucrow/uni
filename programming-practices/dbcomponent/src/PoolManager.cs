/// <summary>
/// Manages a single client's connection to a database pool, ensuring proper acquisition
/// and disposal of connections. This class is thread-safe for individual instances.
/// </summary>
public sealed class PoolManager : IDisposable
{
    private readonly Pool _sharedPool;
    private readonly DbType _dbType;
    private Connection? _connection; // Tracks this client's active connection
    private bool _isDisposed;

    /// <summary>
    /// Initializes a new instance of the PoolManager class.
    /// </summary>
    /// <param name="sharedPool">The shared connection pool to use</param>
    /// <param name="dbType">Type of database this manager will handle</param>
    /// <exception cref="ArgumentNullException">Thrown when sharedPool is null</exception>
    public PoolManager(Pool sharedPool, DbType dbType)
    {
        _sharedPool = sharedPool ?? throw new ArgumentNullException(nameof(sharedPool));
        _dbType = dbType;
    }

    /// <summary>
    /// Gets a database connection from the shared pool for exclusive use by this client.
    /// </summary>
    /// <param name="cancellationToken">Cancellation token to abort the connection request</param>
    /// <returns>A valid database connection</returns>
    /// <exception cref="ObjectDisposedException">Thrown when PoolManager is disposed</exception>
    /// <exception cref="InvalidOperationException">Thrown when client already has an active connection</exception>
    /// <remarks>
    /// The connection will automatically return to the pool when disposed or when the PoolManager is disposed.
    /// </remarks>
    public async Task<Connection> GetConnectionAsync(
        CancellationToken cancellationToken = default)
    {
        if (_isDisposed)
            throw new ObjectDisposedException(nameof(PoolManager));

        if (_connection != null)
            throw new InvalidOperationException("Client already has an active connection");

        // Get connection from shared pool
        _connection = await _sharedPool.GetConnectionAsync(_dbType, cancellationToken);
        _connection.SetReturnHandler(() =>
        {
            _sharedPool.ReturnConnection(_dbType, _connection);
            _connection = null;
        });

        return _connection;
    }

    /// <summary>
    /// Returns the current connection to the shared pool if one exists.
    /// </summary>
    /// <remarks>
    /// This method is idempotent - it's safe to call multiple times.
    /// The connection is automatically returned when PoolManager is disposed.
    /// </remarks>
    public void ReturnConnection()
    {
        if (_connection == null || _isDisposed) return;

        _connection = null;
    }

    /// <summary>
    /// Releases all resources used by the PoolManager, including returning any active connection.
    /// </summary>
    public void Dispose()
    {
        if (_isDisposed) return;
        _isDisposed = true;
        ReturnConnection();
    }
}