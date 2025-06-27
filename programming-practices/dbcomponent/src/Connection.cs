//using Npgsql;
using System.Data.Common;

/// <summary>
/// A disposable wrapper for idk elatla
/// </summary>
public sealed class Connection : IDisposable
{
    private readonly DbConnection _dbConnection;
    private Action? _returnHandler;
    public DbConnection DbConnection => _dbConnection;

    public Connection(DbConnection dbConnection)
    {
        _dbConnection = dbConnection ?? throw new ArgumentNullException(nameof(dbConnection));
    }

    public void SetReturnHandler(Action handler) =>
        _returnHandler = handler;

    public void Close()
    {
        _dbConnection.CloseAsync();
    }

    public void Dispose()
    {
        _returnHandler?.Invoke();
        _returnHandler = null;
    }
}