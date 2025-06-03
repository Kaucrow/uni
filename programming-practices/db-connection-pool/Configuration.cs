using Tomlyn;
using Tomlyn.Model;
using Newtonsoft.Json;

public static class Configuration
{
    static Configuration()
    {
        var configPath = Path.Combine(Directory.GetCurrentDirectory(), "config/config.toml");

        string tomlContent = File.ReadAllText(configPath);

        TomlTable tomlTable = Toml.ToModel(tomlContent);

        if (tomlTable["database"] is TomlTable dbTable)
        {
            Database = new DatabaseConfig(
                host: (string)dbTable["host"],
                port: (string)dbTable["port"],
                user: (string)dbTable["user"],
                password: (string)dbTable["password"],
                name: (string)dbTable["name"],
                table: (string)dbTable["table"]
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
    }

    public static dynamic GetQueries()
    {
        var queriesPath = Path.Combine(Directory.GetCurrentDirectory(), "config/queries.json");
        string jsonContent = File.ReadAllText(queriesPath);
        dynamic? queries = JsonConvert.DeserializeObject(jsonContent);

        if (queries != null)
        {
            return queries;
        }
        else
        {
            throw new Exception("Failed to get queries");
        }
    }

    public static DatabaseConfig Database { get; }
    public static PoolConfig Pool { get; }

    public readonly struct DatabaseConfig
    {
        public DatabaseConfig(
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

        public string Host { get; }
        public string Port { get; }
        public string User { get; }
        public string Password { get; }
        public string Name { get; }
        public string Table { get; }
    }

    public readonly struct PoolConfig
    {
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

        public int StartupSize { get; }
        public int MaxSize { get; }
        public int SizeIncrement { get; }
    }
}