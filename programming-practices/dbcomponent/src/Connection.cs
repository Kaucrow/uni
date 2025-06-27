using System.Data.Common;

/// <summary>
/// Represents a wrapper for a database connection that manages connection lifetime
/// and provides controlled return to the connection pool.
/// </summary>
public sealed class Connection : IDisposable
{
    private readonly DbConnection _dbConnection;
    private Action? _returnHandler;

    /// <summary>
    /// Gets the underlying database connection.
    /// </summary>
    public DbConnection DbConnection => _dbConnection;

    /// <summary>
    /// Initializes a new instance of the Connection class.
    /// </summary>
    /// <param name="dbConnection">The database connection to wrap</param>
    /// <exception cref="ArgumentNullException">Thrown when dbConnection is null</exception>
    public Connection(DbConnection dbConnection)
    {
        _dbConnection = dbConnection ?? throw new ArgumentNullException(nameof(dbConnection));
    }

    /// <summary>
    /// Sets the handler that will be invoked when the connection is returned to the pool.
    /// </summary>
    /// <param name="handler">The action to execute when returning the connection</param>
    /// <remarks>
    /// The handler is typically set by the connection pool manager to properly
    /// return the connection to the available pool.
    /// </remarks>
    public void SetReturnHandler(Action handler) =>
        _returnHandler = handler;

    /// <summary>
    /// Closes the underlying database connection asynchronously.
    /// </summary>
    /// <remarks>
    /// This method initiates the close operation but does not wait for it to complete.
    /// For proper connection cleanup, prefer using Dispose().
    /// </remarks>
    public void Close()
    {
        _dbConnection.CloseAsync();
    }

    /// <summary>
    /// Releases the connection resources and returns it to the pool via the registered handler.
    /// </summary>
    /// <remarks>
    /// If a return handler was set via SetReturnHandler, it will be invoked to properly
    /// return the connection to the pool. Subsequent calls to Dispose() will have no effect.
    /// </remarks>
    public void Dispose()
    {
        _returnHandler?.Invoke();
        _returnHandler = null;
    }
}