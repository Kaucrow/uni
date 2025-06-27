using System.Data.Common;
using MySql.Data.MySqlClient;
using Npgsql;

/// <summary>
/// Provides a high-level database access component that manages connections,
/// transactions, and command execution for both PostgreSQL and MySQL databases.
/// </summary>
public sealed class DbComponent : IDisposable
{
    private DbTransaction? _transaction;
    private readonly Pool _connectionPool;
    private PoolManager? _poolManager;
    private Connection? _connection;

    /// <summary>
    /// Initializes a new instance of the DbComponent class.
    /// </summary>
    /// <param name="connectionPool">The connection pool to use for database operations</param>
    /// <exception cref="ArgumentNullException">Thrown when connectionPool is null</exception>
    public DbComponent(Pool connectionPool)
    {
        _connectionPool = connectionPool ?? throw new ArgumentNullException(nameof(connectionPool));
    }

    /// <summary>
    /// Prepares a database command with the specified SQL and parameters.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="sql">SQL command text</param>
    /// <param name="parameters">Array of parameters for the command</param>
    /// <returns>A configured DbCommand instance</returns>
    /// <exception cref="NotSupportedException">Thrown when database type is not supported</exception>
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

    /// <summary>
    /// Executes a query and returns a single value.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="sql">SQL query text</param>
    /// <param name="parameters">Parameters for the query</param>
    /// <returns>The first column of the first row in the result set</returns>
    public async Task<object?> FetchOne(DbType dbType, string sql, params DbParameter[] parameters)
    {
        await using var cmd = await PrepareCommand(dbType, sql, parameters);
        return await cmd.ExecuteScalarAsync();
    }

    /// <summary>
    /// Executes a query and returns a data reader for iterating through the results.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="sql">SQL query text</param>
    /// <param name="parameters">Parameters for the query</param>
    /// <returns>A DbDataReader for reading the result set</returns>
    /// <remarks>
    /// The caller is responsible for disposing the returned DbDataReader.
    /// </remarks>
    public async Task<DbDataReader> Fetch(DbType dbType, string sql, params DbParameter[] parameters)
    {
        var cmd = await PrepareCommand(dbType, sql, parameters);
        return await cmd.ExecuteReaderAsync();
    }

    /// <summary>
    /// Executes a non-query command (INSERT, UPDATE, DELETE).
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="sql">SQL command text</param>
    /// <param name="parameters">Parameters for the command</param>
    /// <returns>The number of rows affected</returns>
    /// <exception cref="NpgsqlException">Thrown when PostgreSQL command execution fails</exception>
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

    /// <summary>
    /// Begins a new database transaction.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <exception cref="InvalidOperationException">Thrown when a transaction is already active</exception>
    /// <exception cref="NotSupportedException">Thrown when database type is not supported</exception>
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

    /// <summary>
    /// Commits the current transaction.
    /// </summary>
    /// <exception cref="InvalidOperationException">Thrown when no transaction is active</exception>
    public async Task Commit()
    {
        if (_transaction == null)
            throw new InvalidOperationException("No active transaction");

        await _transaction.CommitAsync();
        await CleanupTransaction();
    }

    /// <summary>
    /// Rolls back the current transaction.
    /// </summary>
    /// <exception cref="InvalidOperationException">Thrown when no transaction is active</exception>
    public async Task Rollback()
    {
        if (_transaction == null)
            throw new InvalidOperationException("No active transaction");

        await _transaction.RollbackAsync();
        await CleanupTransaction();
    }

    /// <summary>
    /// Gets a database connection, reusing an existing one if available.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <returns>An active database connection</returns>
    private async Task<Connection> GetConnectionAsync(DbType dbType)
    {
        if (_connection != null) return _connection;

        _poolManager = new PoolManager(_connectionPool, dbType);
        _connection = await _poolManager.GetConnectionAsync();
        return _connection;
    }

    /// <summary>
    /// Cleans up the current transaction resources.
    /// </summary>
    private async Task CleanupTransaction()
    {
        if (_transaction != null)
        {
            await _transaction.DisposeAsync();
            _transaction = null;
        }
    }

    /// <summary>
    /// Releases all resources used by the DbComponent, including
    /// active transactions, connections, and pool managers.
    /// </summary>
    public void Dispose()
    {
        _transaction?.Dispose();
        _connection?.Dispose();
        _poolManager?.Dispose();
    }
}