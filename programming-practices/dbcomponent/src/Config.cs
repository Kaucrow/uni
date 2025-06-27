using Tomlyn;
using Tomlyn.Model;
using YamlDotNet.Serialization;
using System.Dynamic;

/// <summary>
/// Provides static configuration settings loaded from TOML and YAML files.
/// Manages database configurations, connection pool settings, and dynamic queries.
/// </summary>
public static class Config
{
    private static readonly QueriesManager _queries = new QueriesManager("config/queries.yaml");
    
    /// <summary>
    /// Gets dynamic access to SQL queries loaded from YAML files.
    /// </summary>
    public static dynamic Queries => _queries.Current;

    /// <summary>
    /// Gets test configuration settings.
    /// </summary>
    public static TestConfig Test { get; }

    /// <summary>
    /// Gets connection pool configuration settings.
    /// </summary>
    public static PoolConfig Pool { get; }

    /// <summary>
    /// Gets PostgreSQL database configuration settings.
    /// </summary>
    public static PostgresConfig Postgres { get; }

    /// <summary>
    /// Gets MySQL database configuration settings.
    /// </summary>
    public static MySqlConfig MySql { get; }

    static Config()
    {
        var configPath = Path.Combine(Directory.GetCurrentDirectory(), "config/config.toml");
        string tomlContent = File.ReadAllText(configPath);
        TomlTable tomlTable = Toml.ToModel(tomlContent);

        if (tomlTable["test"] is TomlTable testTable)
        {
            Test = new TestConfig(
                testUsers: (int)(long)testTable["test-users"]
            );
        }

        if (tomlTable["pool"] is TomlTable poolTable)
        {
            Pool = new PoolConfig(
                startupSize: (int)(long)poolTable["default-size"],
                maxSize: (int)(long)poolTable["max-size"],
                sizeIncrement: (int)(long)poolTable["size-increment"]
            );
        }

        if (tomlTable["db-postgres"] is TomlTable postgresTable)
        {
            Postgres = new PostgresConfig(
                host: (string)postgresTable["host"],
                port: (string)postgresTable["port"],
                user: (string)postgresTable["user"],
                password: (string)postgresTable["password"],
                name: (string)postgresTable["name"],
                table: (string)postgresTable["table"]
            );
        }

        if (tomlTable["db-mysql"] is TomlTable mysqlTable)
        {
            MySql = new MySqlConfig(
                host: (string)mysqlTable["host"],
                port: (string)mysqlTable["port"],
                user: (string)mysqlTable["user"],
                password: (string)mysqlTable["password"],
                name: (string)mysqlTable["name"],
                table: (string)mysqlTable["table"]
            );
        }
    }

    /// <summary>
    /// Represents test configuration settings.
    /// </summary>
    public readonly struct TestConfig
    {
        /// <summary>
        /// Gets the number of test users configured.
        /// </summary>
        public int TestUsers { get; }

        /// <summary>
        /// Initializes a new instance of TestConfig.
        /// </summary>
        /// <param name="testUsers">Number of test users</param>
        public TestConfig(int testUsers)
        {
            TestUsers = testUsers;
        }
    }

    /// <summary>
    /// Represents connection pool configuration settings.
    /// </summary>
    public readonly struct PoolConfig
    {
        /// <summary>
        /// Gets the initial number of connections in the pool.
        /// </summary>
        public int StartupSize { get; }

        /// <summary>
        /// Gets the maximum number of connections allowed in the pool.
        /// </summary>
        public int MaxSize { get; }

        /// <summary>
        /// Gets the number of connections to add when the pool needs to grow.
        /// </summary>
        public int SizeIncrement { get; }

        /// <summary>
        /// Initializes a new instance of PoolConfig.
        /// </summary>
        /// <param name="startupSize">Initial pool size</param>
        /// <param name="maxSize">Maximum pool size</param>
        /// <param name="sizeIncrement">Pool growth increment</param>
        public PoolConfig(int startupSize, int maxSize, int sizeIncrement)
        {
            StartupSize = startupSize;
            MaxSize = maxSize;
            SizeIncrement = sizeIncrement;
        }
    }

    /// <summary>
    /// Represents PostgreSQL database configuration settings.
    /// </summary>
    public readonly struct PostgresConfig
    {
        /// <summary>
        /// Gets the database server host.
        /// </summary>
        public string Host { get; }

        /// <summary>
        /// Gets the database server port.
        /// </summary>
        public string Port { get; }

        /// <summary>
        /// Gets the database username.
        /// </summary>
        public string User { get; }

        /// <summary>
        /// Gets the database password.
        /// </summary>
        public string Password { get; }

        /// <summary>
        /// Gets the database name.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the default table name.
        /// </summary>
        public string Table { get; }

        /// <summary>
        /// Initializes a new instance of PostgresConfig.
        /// </summary>
        public PostgresConfig(
            string host,
            string port,
            string user,
            string password,
            string name,
            string table)
        {
            Host = host;
            Port = port;
            User = user;
            Password = password;
            Name = name;
            Table = table;
        }
    }

    /// <summary>
    /// Represents MySQL database configuration settings.
    /// </summary>
    public readonly struct MySqlConfig
    {
        /// <summary>
        /// Gets the database server host.
        /// </summary>
        public string Host { get; }

        /// <summary>
        /// Gets the database server port.
        /// </summary>
        public string Port { get; }

        /// <summary>
        /// Gets the database username.
        /// </summary>
        public string User { get; }

        /// <summary>
        /// Gets the database password.
        /// </summary>
        public string Password { get; }

        /// <summary>
        /// Gets the database name.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Gets the default table name.
        /// </summary>
        public string Table { get; }

        /// <summary>
        /// Initializes a new instance of MySqlConfig.
        /// </summary>
        public MySqlConfig(
            string host,
            string port,
            string user,
            string password,
            string name,
            string table)
        {
            Host = host;
            Port = port;
            User = user;
            Password = password;
            Name = name;
            Table = table;
        }
    }

    /// <summary>
    /// Manages dynamic SQL queries loaded from YAML files with hot-reload capability.
    /// </summary>
    private sealed class QueriesManager : IDisposable
    {
        private readonly FileSystemWatcher _watcher;
        private readonly object _lock = new object();
        private readonly string _filePath;
        private dynamic _currentQueries = new ExpandoObject();

        /// <summary>
        /// Gets the current collection of queries.
        /// </summary>
        public dynamic Current
        {
            get
            {
                lock (_lock)
                {
                    return _currentQueries;
                }
            }
        }

        /// <summary>
        /// Initializes a new instance of QueriesManager.
        /// </summary>
        /// <param name="filePath">Path to the YAML queries file</param>
        /// <exception cref="ArgumentNullException">Thrown when filePath is null</exception>
        /// <exception cref="ArgumentException">Thrown when filePath is invalid</exception>
        /// <exception cref="DirectoryNotFoundException">Thrown when directory doesn't exist</exception>
        public QueriesManager(string filePath)
        {
            _filePath = filePath ?? throw new ArgumentNullException(nameof(filePath));

            string directory = Path.GetDirectoryName(_filePath)
                ?? throw new ArgumentException("Invalid file path", nameof(filePath));

            if (!Directory.Exists(directory))
                throw new DirectoryNotFoundException($"Directory not found: {directory}");

            RefreshQueries();
            _watcher = new FileSystemWatcher(directory)
            {
                Filter = Path.GetFileName(_filePath),
                NotifyFilter = NotifyFilters.LastWrite
            };
            _watcher.Changed += OnFileChanged;
            _watcher.EnableRaisingEvents = true;
        }

        private void OnFileChanged(object sender, FileSystemEventArgs e)
        {
            if (e.ChangeType == WatcherChangeTypes.Changed)
            {
                RefreshQueries();
            }
        }

        private void RefreshQueries()
        {
            lock (_lock)
            {
                try
                {
                    for (int i = 0; i < 3; i++)
                    {
                        try
                        {
                            string yamlContent = File.ReadAllText(_filePath);
                            var deserializer = new DeserializerBuilder().Build();
                            var dict = deserializer.Deserialize<Dictionary<object, object>>(yamlContent);
                            _currentQueries = ConvertToExpando(dict);
                            return;
                        }
                        catch (IOException) when (i < 2)
                        {
                            Thread.Sleep(100);
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine($"Error reloading queries: {ex.Message}");
                }
            }
        }

        private static dynamic ConvertToExpando(Dictionary<object, object> dictionary)
        {
            dynamic expando = new ExpandoObject();
            var expandoDict = (IDictionary<string, object>)expando;

            foreach (var kvp in dictionary)
            {
                string key = kvp.Key.ToString() ?? throw new InvalidDataException("YAML key cannot be null");
                expandoDict[key] = kvp.Value is Dictionary<object, object> nestedDict
                    ? ConvertToExpando(nestedDict) 
                    : kvp.Value;
            }
            
            return expando;
        }

        /// <summary>
        /// Releases all resources used by the QueriesManager.
        /// </summary>
        public void Dispose()
        {
            _watcher?.Dispose();
        }
    }
}