using Npgsql;
using MySql.Data.MySqlClient;
using System.Collections.Concurrent;

public enum DbType
{
    Postgres,
    MySQL
}

public sealed class Pool : IDisposable
{
    private static readonly Lazy<Pool> _instance = new Lazy<Pool>(() => new Pool());
    private readonly CancellationTokenSource _disposeTokenSource = new();
    // Diccionario para pools por tipo de base de datos
    private readonly Dictionary<DbType, DatabasePool> _dbPools = new();

    // Clase interna para manejar cada tipo de base de datos
    private class DatabasePool
    {
        public string ConnectionString { get; set; } = null!;
        public int MaxConnections { get; set; }
        public int ConnectionIncrement { get; set; }
        public int ActiveConnections;
        public ConcurrentBag<Connection> AvailableConnections = new();
        public SemaphoreSlim Semaphore = null!;
        public SemaphoreSlim CreationSemaphore = new(1, 1);
    }

    public static Pool Instance =>

        _instance.Value;

    // Inicializa un pool para un tipo de base de datos
    public static async Task Initialize(
        DbType dbType, // Usar enum en vez de string
        string connectionString,
        int startupConnections = 10,
        int maxConnections = 100,
        int connectionIncrement = 5
    )
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

        // Crear conexiones iniciales
        for (int i = 0; i < startupConnections; i++)
        {
            Connection conn = await Instance.CreateNewConnectionAsync(dbType, dbPool.ConnectionString);
            dbPool.AvailableConnections.Add(conn);
        }
        Instance._dbPools[dbType] = dbPool;
    }

    private Pool() { }

    // Obtiene una conexión para el tipo de base de datos
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

    // Valida la conexión según el tipo de base de datos
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

    // Devuelve la conexión al pool correspondiente
    public void ReturnConnection(DbType dbType, Connection connection)
    {
        if (connection == null) throw new ArgumentNullException(nameof(connection));
        if (!_dbPools.TryGetValue(dbType, out var dbPool))
            throw new InvalidOperationException($"Pool for '{dbType}' is not initialized");
        dbPool.AvailableConnections.Add(connection);
        dbPool.Semaphore.Release();
        Interlocked.Decrement(ref dbPool.ActiveConnections);
    }

    // Crea una nueva conexión según el tipo de base de datos
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