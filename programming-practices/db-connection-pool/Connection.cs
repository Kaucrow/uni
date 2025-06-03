using Npgsql;

public sealed class Connection : IDisposable
{
    private readonly NpgsqlConnection _dbConnection;

    private Action? _returnHandler;

    public NpgsqlConnection DbConnection => _dbConnection;

    public Connection(NpgsqlConnection dbConnection)
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