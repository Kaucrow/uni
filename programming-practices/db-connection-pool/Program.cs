using Npgsql;

class Program
{
    static async Task Main(string[] args)
    {
        string connectionString = string.Format(
            "Host={0};Username={1};Password={2};Database={3};Pooling=false",
            Configuration.Database.Host,
            Configuration.Database.User,
            Configuration.Database.Password,
            Configuration.Database.Name
        );

        Pool.Initialize(connectionString, Configuration.Pool.StartupSize, Configuration.Pool.MaxSize, Configuration.Pool.SizeIncrement);
        var pool = Pool.Instance;
        var successCount = 0;
        var failureCount = 0;
        var tasks = new Task[1000];

        // Simulate 1000 concurrent users
        for (int i = 0; i < 1000; i++)
        {
            tasks[i] = Task.Run(async () =>
            {
                using var poolManager = new PoolManager(pool);
                try
                {
                    // Get connection with 5 second timeout
                    using var timeoutCts = new CancellationTokenSource(TimeSpan.FromSeconds(5));
                    using var connection = await poolManager.GetConnectionAsync(timeoutCts.Token);

                    // Execute test query
                    using var cmd = new NpgsqlCommand("SELECT 1", connection.DbConnection);
                    await cmd.ExecuteScalarAsync();

                    Interlocked.Increment(ref successCount);
                    Console.WriteLine($"User {Task.CurrentId}: Query succeeded");
                }
                catch (Exception ex)
                {
                    Interlocked.Increment(ref failureCount);
                    Console.WriteLine($"User {Task.CurrentId}: Failed - {ex.Message}");
                }
            });
        }

        await Task.WhenAll(tasks);
        
        Console.WriteLine($"\nResults: {successCount} succeeded, {failureCount} failed");
        pool.Dispose(); // Clean up pool
    }
}