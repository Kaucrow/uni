public sealed class PoolManager : IDisposable
{
    private readonly Pool _sharedPool;
    private Connection? _connection; // Tracks this client's active connection
    private bool _isDisposed;

    public PoolManager(Pool sharedPool) 
    {
        _sharedPool = sharedPool ?? throw new ArgumentNullException(nameof(sharedPool));
    }

    // Core Method: Get a connection for this client
    public async Task<Connection> GetConnectionAsync(
        CancellationToken cancellationToken = default)
    {
        if (_isDisposed)
            throw new ObjectDisposedException(nameof(PoolManager));

        if (_connection != null)
            throw new InvalidOperationException("Client already has an active connection");

        // Get connection from shared pool
        _connection = await _sharedPool.GetConnectionAsync(cancellationToken);
        _connection.SetReturnHandler(() =>
        {
            _sharedPool.ReturnConnection(_connection);
            _connection = null;
        });

        return _connection;
    }

    // Return the connection to the shared pool
    public void ReturnConnection()
    {
        if (_connection == null || _isDisposed) return;

        _connection = null;
    }

    public void Dispose()
    {
        if (_isDisposed) return;
        _isDisposed = true;
        ReturnConnection();
    }
}