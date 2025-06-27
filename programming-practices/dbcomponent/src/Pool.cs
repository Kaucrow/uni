using Npgsql;
using MySql.Data.MySqlClient;
using System.Collections.Concurrent;

/// <summary>
/// Represents the type of database supported by the connection pool.
/// </summary>
public enum DbType
{
    /// <summary>
    /// PostgreSQL database
    /// </summary>
    Postgres,
    
    /// <summary>
    /// MySQL database
    /// </summary>
    MySQL
}

/// <summary>
/// Thread-safe connection pool implementation for managing database connections.
/// Supports PostgreSQL and MySQL databases.
/// </summary>
public sealed class Pool : IDisposable
{
    private static readonly Lazy<Pool> _instance = new Lazy<Pool>(() => new Pool());
    private readonly CancellationTokenSource _disposeTokenSource = new();
    private readonly Dictionary<DbType, DatabasePool> _dbPools = new();

    /// <summary>
    /// Internal class representing a pool for a specific database type.
    /// </summary>
    private class DatabasePool
    {
        /// <summary>
        /// Connection string for the database
        /// </summary>
        public string ConnectionString { get; set; } = null!;
        
        /// <summary>
        /// Maximum number of connections allowed in the pool
        /// </summary>
        public int MaxConnections { get; set; }
        
        /// <summary>
        /// Number of connections to create when pool needs to grow
        /// </summary>
        public int ConnectionIncrement { get; set; }
        
        /// <summary>
        /// Count of currently active connections
        /// </summary>
        public int ActiveConnections;
        
        /// <summary>
        /// Collection of available connections
        /// </summary>
        public ConcurrentBag<Connection> AvailableConnections = new();
        
        /// <summary>
        /// Semaphore to control access to the pool
        /// </summary>
        public SemaphoreSlim Semaphore = null!;
        
        /// <summary>
        /// Semaphore to control connection creation
        /// </summary>
        public SemaphoreSlim CreationSemaphore = new(1, 1);
    }

    /// <summary>
    /// Singleton instance of the connection pool
    /// </summary>
    public static Pool Instance => _instance.Value;

    /// <summary>
    /// Initializes the connection pool for a specific database type.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="connectionString">Connection string for the database</param>
    /// <param name="startupConnections">Initial number of connections to create</param>
    /// <param name="maxConnections">Maximum number of connections allowed</param>
    /// <param name="connectionIncrement">Number of connections to add when pool needs to grow</param>
    /// <exception cref="InvalidOperationException">Thrown when pool is already initialized for the database type</exception>
    public static async Task Initialize(
        DbType dbType,
        string connectionString,
        int startupConnections = 10,
        int maxConnections = 100,
        int connectionIncrement = 5)
    {
        if (Instance._dbPools.ContainsKey(dbType))
            throw new InvalidOperationException($"Pool for '{dbType}' is already initialized");

        var dbPool = new DatabasePool
        {
            ConnectionString = connectionString + (dbType == DbType.Postgres ? ";Pooling=false;SSL Mode=Disable;TCP Keepalive=true;Keepalive=60;No Reset On Close=true" : ""),
            MaxConnections = maxConnections,
            ConnectionIncrement = connectionIncrement,
            Semaphore = new SemaphoreSlim(maxConnections, maxConnections)
        };

        // Create initial connections
        for (int i = 0; i < startupConnections; i++)
        {
            Connection conn = await Instance.CreateNewConnectionAsync(dbType, dbPool.ConnectionString);
            dbPool.AvailableConnections.Add(conn);
        }
        Instance._dbPools[dbType] = dbPool;
    }

    private Pool() { }

    /// <summary>
    /// Gets a connection from the pool for the specified database type.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="cancellationToken">Cancellation token</param>
    /// <returns>A valid database connection</returns>
    /// <exception cref="InvalidOperationException">Thrown when pool is not initialized or exhausted</exception>
    /// <exception cref="TimeoutException">Thrown when connection request is canceled</exception>
    public async Task<Connection> GetConnectionAsync(DbType dbType, CancellationToken cancellationToken = default)
    {
        if (!_dbPools.TryGetValue(dbType, out var dbPool))
            throw new InvalidOperationException($"Pool for '{dbType}' is not initialized");

        using var linkedCts = CancellationTokenSource.CreateLinkedTokenSource(_disposeTokenSource.Token, cancellationToken);
        try
        {
            await dbPool.Semaphore.WaitAsync(linkedCts.Token);
            for (int i = 0; i < 3; i++)
            {
                if (dbPool.AvailableConnections.TryTake(out var connection))
                {
                    if (await ValidateConnection(dbType, connection))
                    {
                        Interlocked.Increment(ref dbPool.ActiveConnections);
                        return connection;
                    }
                    connection.Close();
                }
            }
            await dbPool.CreationSemaphore.WaitAsync(cancellationToken);
            try
            {
                if (dbPool.AvailableConnections.TryTake(out var connection))
                {
                    if (await ValidateConnection(dbType, connection))
                    {
                        Interlocked.Increment(ref dbPool.ActiveConnections);
                        return connection;
                    }
                    connection.Close();
                }
                var remainingCapacity = dbPool.MaxConnections - Volatile.Read(ref dbPool.ActiveConnections);
                if (remainingCapacity <= 0)
                    throw new InvalidOperationException("Pool exhausted");
                int newConnectionsToCreate = Math.Min(dbPool.ConnectionIncrement, remainingCapacity);
                var newConnections = new List<Connection>();
                for (int i = 0; i < newConnectionsToCreate; i++)
                {
                    var newConn = await CreateNewConnectionAsync(dbType, dbPool.ConnectionString);
                    newConnections.Add(newConn);
                    if (i != 0) dbPool.AvailableConnections.Add(newConn);
                }
                Interlocked.Increment(ref dbPool.ActiveConnections);
                return newConnections[0];
            }
            finally { dbPool.CreationSemaphore.Release(); }
        }
        catch (OperationCanceledException)
        {
            throw new TimeoutException("Connection request canceled or pool is disposing");
        }
    }

    /// <summary>
    /// Validates that a connection is still alive and usable.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="conn">Connection to validate</param>
    /// <returns>True if connection is valid, false otherwise</returns>
    private async Task<bool> ValidateConnection(DbType dbType, Connection conn)
    {
        try
        {
            if (dbType == DbType.Postgres)
            {
                using var cmd = new NpgsqlCommand("SELECT 1", conn.DbConnection as NpgsqlConnection);
                await cmd.ExecuteScalarAsync();
                return true;
            }
            else if (dbType == DbType.MySQL)
            {
                using var cmd = new MySqlCommand("SELECT 1", conn.DbConnection as MySqlConnection);
                await cmd.ExecuteScalarAsync();
                return true;
            }
            return false;
        }
        catch { return false; }
    }

    /// <summary>
    /// Returns a connection to the pool.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="connection">Connection to return</param>
    /// <exception cref="ArgumentNullException">Thrown when connection is null</exception>
    /// <exception cref="InvalidOperationException">Thrown when pool is not initialized</exception>
    public void ReturnConnection(DbType dbType, Connection connection)
    {
        if (connection == null) throw new ArgumentNullException(nameof(connection));
        if (!_dbPools.TryGetValue(dbType, out var dbPool))
            throw new InvalidOperationException($"Pool for '{dbType}' is not initialized");
        dbPool.AvailableConnections.Add(connection);
        dbPool.Semaphore.Release();
        Interlocked.Decrement(ref dbPool.ActiveConnections);
    }

    /// <summary>
    /// Creates a new database connection.
    /// </summary>
    /// <param name="dbType">Type of database</param>
    /// <param name="connectionString">Connection string</param>
    /// <returns>A new database connection</returns>
    /// <exception cref="NotSupportedException">Thrown when database type is not supported</exception>
    private async Task<Connection> CreateNewConnectionAsync(DbType dbType, string connectionString)
    {
        if (dbType == DbType.Postgres)
        {
            var conn = new NpgsqlConnection(connectionString);
            await conn.OpenAsync();
            return new Connection(conn);
        }
        else if (dbType == DbType.MySQL)
        {
            var conn = new MySqlConnection(connectionString);
            await conn.OpenAsync();
            return new Connection(conn);
        }
        throw new NotSupportedException($"Database type '{dbType}' not supported");
    }

    /// <summary>
    /// Disposes the connection pool and all its resources.
    /// </summary>
    public void Dispose()
    {
        _disposeTokenSource.Cancel();
        foreach (var dbPool in _dbPools.Values)
        {
            while (dbPool.AvailableConnections.TryTake(out var connection))
            {
                connection.Close();
            }
            dbPool.Semaphore.Dispose();
        }
    }
}