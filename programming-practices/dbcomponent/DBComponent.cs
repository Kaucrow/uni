using System.Data.Common;
using MySql.Data.MySqlClient;
using Npgsql;

public sealed class DbComponent : IDisposable
{
    private DbTransaction? _transaction;
    private readonly Pool _connectionPool;
    private PoolManager? _poolManager;
    private Connection? _connection;

    public DbComponent(Pool connectionPool)
    {
        _connectionPool = connectionPool;
    }

    // Prepares a command to use the active transaction (if any)
    private async Task<DbCommand> PrepareCommand(DbType dbType, string sql, DbParameter[] parameters)
    {
        var connection = await GetConnectionAsync(dbType);

        DbCommand cmd = dbType switch
        {
            DbType.Postgres => new NpgsqlCommand(sql, (NpgsqlConnection)connection.DbConnection),
            DbType.MySQL => new MySqlCommand(sql, (MySqlConnection)connection.DbConnection),
            _ => throw new NotSupportedException($"Database type {dbType} is not supported")
        };

        if (_transaction != null)
        {
            cmd.Transaction = _transaction;
        }

        cmd.Parameters.AddRange(parameters);
        return cmd;
    }

    // Fetch a single value (scalar)
    public async Task<object?> FetchOne(DbType dbType, string sql, params DbParameter[] parameters)
    {
        await using var cmd = await PrepareCommand(dbType, sql, parameters);
        return await cmd.ExecuteScalarAsync();
    }

    // Fetch multiple rows
    public async Task<DbDataReader> Fetch(DbType dbType, string sql, params DbParameter[] parameters)
    {
        var cmd = await PrepareCommand(dbType, sql, parameters);
        return await cmd.ExecuteReaderAsync();
    }

    // Execute with no results (insert/update/delete)
    public async Task<int> Execute(DbType dbType, string sql, params DbParameter[] parameters)
    {
        await using var cmd = await PrepareCommand(dbType, sql, parameters);
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
    public async Task Begin(DbType dbType)
    {
        if (_transaction != null)
            throw new InvalidOperationException("Transaction already in progress");

        var connection = await GetConnectionAsync(dbType);

        _transaction = dbType switch
        {
            DbType.Postgres => await ((NpgsqlConnection)connection.DbConnection).BeginTransactionAsync(),
            DbType.MySQL => await ((MySqlConnection)connection.DbConnection).BeginTransactionAsync(),
            _ => throw new NotSupportedException($"Database {dbType} is not supported")
        };
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

    private async Task<Connection> GetConnectionAsync(DbType dbType)
    {
        if (_connection != null) return _connection;

        _poolManager = new PoolManager(_connectionPool, dbType);
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