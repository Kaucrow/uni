using Npgsql;
using Spectre.Console;

class Program
{
    static async Task Main(string[] args)
    {
        string connectionString = string.Format(
            "Host={0};Username={1};Password={2};Database={3}",
            Configuration.Database.Host,
            Configuration.Database.User,
            Configuration.Database.Password,
            Configuration.Database.Name
        );

        await Pool.Initialize(connectionString, Configuration.Pool.StartupSize, Configuration.Pool.MaxSize, Configuration.Pool.SizeIncrement);
        var pool = Pool.Instance;
        var successCount = 0;
        var failureCount = 0;
        var tasks = new Task[Configuration.Database.TestUsers];

        var startTime = DateTime.Now;

        // Simulate concurrent users
        for (int i = 0; i < Configuration.Database.TestUsers; i++)
        {
            int userId = i;
            tasks[i] = Task.Run(async () =>
            {
                using var poolManager = new PoolManager(pool);
                try
                {
                    // Get connection with 5 second timeout
                    // using var timeoutCts = new CancellationTokenSource();
                    using var connection = await poolManager.GetConnectionAsync();

                    // Execute test query
                    using var cmd = new NpgsqlCommand(
                        string.Format(
                            (string)Configuration.GetQueries().testQuery,
                            "*",
                            Configuration.Database.Table.ToString()
                        ),
                        connection.DbConnection
                    );
                    await cmd.ExecuteScalarAsync();

                    Interlocked.Increment(ref successCount);
                    AnsiConsole.MarkupLine($"[yellow]User {userId}:[/] [green]Query succeeded[/]");
                }
                catch (Exception ex)
                {
                    Interlocked.Increment(ref failureCount);
                    AnsiConsole.MarkupLine($"[yellow]User {userId}:[/] [red]Failed - {ex.Message}[/]");
                }
            });
        }

        await Task.WhenAll(tasks);

        var endTime = DateTime.Now;
        var elapsedTime = endTime - startTime;

        AnsiConsole.MarkupLine(
            $"[magenta]\nResults:[/] [green]{successCount}[/] succeeded,[red]{failureCount}[/] failed, [yellow]time elapsed:[/] {elapsedTime.Hours:F2}:{elapsedTime.Minutes:F2}:{elapsedTime.Seconds:F2}"
        );
        pool.Dispose();
    }
}