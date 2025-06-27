using Tomlyn;
using Tomlyn.Model;
using YamlDotNet.Serialization;
using System.Dynamic;

public static class Config
{
    private static readonly QueriesManager _queries = new QueriesManager("config/queries.yaml");
    public static dynamic Queries => _queries.Current;
    public static TestConfig Test { get; }
    public static PoolConfig Pool { get; }
    public static PostgresConfig Postgres { get; }
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

    public readonly struct TestConfig
    {
        public int TestUsers { get; }

        public TestConfig(
            int testUsers
        )
        {
            TestUsers = testUsers;
        }
    }

    public readonly struct PoolConfig
    {
        public int StartupSize { get; }
        public int MaxSize { get; }
        public int SizeIncrement { get; }

        public PoolConfig(
            int startupSize,
            int maxSize,
            int sizeIncrement
        )
        {
            StartupSize = startupSize;
            MaxSize = maxSize;
            SizeIncrement = sizeIncrement;
        }
    }

    public readonly struct PostgresConfig
    {
        public string Host { get; }
        public string Port { get; }
        public string User { get; }
        public string Password { get; }
        public string Name { get; }
        public string Table { get; }

        public PostgresConfig(
            string host,
            string port,
            string user,
            string password,
            string name,
            string table
        )
        {
            Host = host;
            Port = port;
            User = user;
            Password = password;
            Name = name;
            Table = table;
        }
    }

    public readonly struct MySqlConfig
    {
        public string Host { get; }
        public string Port { get; }
        public string User { get; }
        public string Password { get; }
        public string Name { get; }
        public string Table { get; }

        public MySqlConfig(
            string host,
            string port,
            string user,
            string password,
            string name,
            string table
        )
        {
            Host = host;
            Port = port;
            User = user;
            Password = password;
            Name = name;
            Table = table;
        }
    }

    private sealed class QueriesManager : IDisposable
    {
        private readonly FileSystemWatcher _watcher;
        private readonly object _lock = new object();
        private readonly string _filePath;
        private dynamic _currentQueries = new ExpandoObject();

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

        public QueriesManager(string filePath)
        {
            _filePath = filePath ?? throw new ArgumentNullException(nameof(filePath));

            // Validate directory exists before creating watcher
            string directory = Path.GetDirectoryName(_filePath)
                ?? throw new ArgumentException("Invalid file path", nameof(filePath));

            if (!Directory.Exists(directory))
                throw new DirectoryNotFoundException($"Directory not found: {directory}");

            // Initialize
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
                    // Retry file access
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

        public void Dispose()
        {
            _watcher?.Dispose();
        }
    }
}