using Npgsql;
using Spectre.Console;

class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine(Config.Queries.Test.Transaction.Record);

        string connectionString = string.Format(
            "Host={0};Username={1};Password={2};Database={3}",
            Config.Database.Host,
            Config.Database.User,
            Config.Database.Password,
            Config.Database.Name
        );

        await Pool.Initialize(connectionString, Config.Pool.StartupSize, Config.Pool.MaxSize, Config.Pool.SizeIncrement);
        var pool = Pool.Instance;

        // Create test table if not exists
        await InitializeTestDatabase(pool);

        var startTime = DateTime.Now;
        var transferAmount = 100m;
        var successCount = 0;
        var failureCount = 0;
        var tasks = new Task[Config.Database.TestUsers];

        AnsiConsole.MarkupLine("[yellow]Starting transaction demo...[/]");
        AnsiConsole.MarkupLine($"[green]Simulating {Config.Database.TestUsers} concurrent transfers of {transferAmount}[/]");

        // Simulate concurrent bank transfers
        for (int i = 0; i < Config.Database.TestUsers; i++)
        {
            int userId = i;
            tasks[i] = Task.Run(async () =>
            {
                using var db = new DBComponent(pool);
                try
                {
                    await db.Begin();

                    // 1. Check source account balance
                    var balance = (decimal?)await db.FetchOne(
                        Config.Queries.Test.GetBalanceWithLock,
                        new NpgsqlParameter("@id", userId % 2 + 1) // Alternate between accounts 1 and 2
                    );

                    if (!balance.HasValue || balance < transferAmount)
                    {
                        throw new Exception("Insufficient funds");
                    }

                    // 2. Withdraw from source
                    await db.Execute(
                        Config.Queries.Test.Withdraw,
                        new NpgsqlParameter("@amount", transferAmount),
                        new NpgsqlParameter("@id", userId % 2 + 1)
                    );

                    // 3. Deposit to target
                    await db.Execute(
                        Config.Queries.Test.Deposit,
                        new NpgsqlParameter("@amount", transferAmount),
                        new NpgsqlParameter("@id", (userId % 2) + 2) // Alternate between accounts 2 and 1
                    );

                    // 4. Record transaction
                    await db.Execute(
                        Config.Queries.Test.Transaction.Record,
                        new NpgsqlParameter("@from", userId % 2 + 1),
                        new NpgsqlParameter("@to", (userId % 2) + 2),
                        new NpgsqlParameter("@amount", transferAmount)
                    );

                    await db.Commit();
                    Interlocked.Increment(ref successCount);
                    AnsiConsole.MarkupLine($"[yellow]User {userId}:[/] [green]Transfer succeeded[/]");
                }
                catch (Exception ex)
                {
                    try { await db.Rollback(); } catch { /* Ignore rollback errors */ }
                    Interlocked.Increment(ref failureCount);
                    AnsiConsole.MarkupLine($"[yellow]User {userId}:[/] [red]Transfer failed - {ex.Message}[/]");
                }
            });
        }

        await Task.WhenAll(tasks);

        // Verify final balances
        using (var db = new DBComponent(pool))
        {
            var account1 = await db.FetchOne(
                Config.Queries.Test.GetBalance,
                new NpgsqlParameter("@id", 1)
            );
            var account2 = await db.FetchOne(
                Config.Queries.Test.GetBalance,
                new NpgsqlParameter("@id", 2)
            );
            var transactionCount = await db.FetchOne(Config.Queries.Test.Transaction.Count);

            AnsiConsole.MarkupLine("\n[underline]Final Balances:[/]");
            AnsiConsole.MarkupLine($"Account 1: [green]{account1}[/]");
            AnsiConsole.MarkupLine($"Account 2: [green]{account2}[/]");
            AnsiConsole.MarkupLine($"Total transactions: [yellow]{transactionCount}[/]");
        }

        var endTime = DateTime.Now;
        var elapsedTime = endTime - startTime;

        AnsiConsole.MarkupLine(
            $"\n[magenta]Results:[/] [green]{successCount}[/] succeeded, [red]{failureCount}[/] failed, " +
            $"[yellow]Time elapsed:[/] {elapsedTime.TotalSeconds:F2}s"
        );
        pool.Dispose();
    }

    static async Task InitializeTestDatabase(Pool pool)
    {
        using var db = new DBComponent(pool);

        await db.Execute(Config.Queries.InitializeDb.Accounts.Table);

        await db.Execute(Config.Queries.InitializeDb.Transactions.Table);

        // Initialize test accounts if empty
        var countResult = await db.FetchOne(Config.Queries.InitializeDb.Accounts.Count);

        int count = 0;
        if (countResult != null && !(countResult is DBNull))
        {
            count = Convert.ToInt32(countResult);
        }

        if (count == 0)
        {
            await db.Execute(Config.Queries.InitializeDb.Accounts.Insert);
        }
        else
        {
            // Reset balances for demo
            await db.Execute(Config.Queries.InitializeDb.Accounts.Reset);
        }
        
        // Clear previous transactions
        await db.Execute(Config.Queries.InitializeDb.Transactions.Reset);
    }
}