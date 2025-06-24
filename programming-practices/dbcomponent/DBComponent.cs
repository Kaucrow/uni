using Npgsql;

public sealed class DBComponent : IDisposable
{
    private NpgsqlTransaction? _transaction;
    private readonly Pool _connectionPool;
    private PoolManager? _poolManager;
    private Connection? _connection;

    public DBComponent(Pool connectionPool)
    {
        _connectionPool = connectionPool;
    }

    // Prepares a command to use the active transaction (if any)
    private async Task<NpgsqlCommand> PrepareCommand(string sql, NpgsqlParameter[] parameters)
    {
        var connection = await GetConnectionAsync();
        var cmd = new NpgsqlCommand(sql, connection.DbConnection)
        {
            Transaction = _transaction
        };
        cmd.Parameters.AddRange(parameters);
        return cmd;
    }

    // Fetch a single value (scalar)
    public async Task<object?> FetchOne(string sql, params NpgsqlParameter[] parameters)
    {
        await using var cmd = await PrepareCommand(sql, parameters);
        return await cmd.ExecuteScalarAsync();
    }

    // Fetch multiple rows
    public async Task<NpgsqlDataReader> Fetch(string sql, params NpgsqlParameter[] parameters)
    {
        var cmd = await PrepareCommand(sql, parameters);
        return await cmd.ExecuteReaderAsync();
    }

    // Execute with no results (insert/update/delete)
    public async Task<int> Execute(string sql, params NpgsqlParameter[] parameters)
    {
        await using var cmd = await PrepareCommand(sql, parameters);
        try
        {
            return await cmd.ExecuteNonQueryAsync();
        }
        catch (NpgsqlException ex)
        {
            throw new NpgsqlException(
                $"Execute failed: {sql} | Params: {string.Join(", ", parameters.Select(p => $"{p.ParameterName}={p.Value}"))}",
                ex
            );
        }
    }

    // Transaction management
    public async Task Begin()
    {
        if (_transaction != null)
            throw new InvalidOperationException("Transaction already in progress");

        var connection = await GetConnectionAsync();
        _transaction = await connection.DbConnection.BeginTransactionAsync();
    }

    public async Task Commit()
    {
        if (_transaction == null)
            throw new InvalidOperationException("No active transaction");

        await _transaction.CommitAsync();
        await CleanupTransaction();
    }

    public async Task Rollback()
    {
        if (_transaction == null)
            throw new InvalidOperationException("No active transaction");

        await _transaction.RollbackAsync();
        await CleanupTransaction();
    }

    private async Task<Connection> GetConnectionAsync()
    {
        if (_connection != null) return _connection;

        _poolManager = new PoolManager(_connectionPool);
        _connection = await _poolManager.GetConnectionAsync();
        return _connection;
    }

    private async Task CleanupTransaction()
    {
        if (_transaction != null)
        {
            await _transaction.DisposeAsync();
            _transaction = null;
        }
    }

    public void Dispose()
    {
        _transaction?.Dispose();
        _connection?.Dispose();
        _poolManager?.Dispose();
    }
}