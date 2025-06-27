using System.Data.Common;
using Npgsql;
using MySql;
using Spectre.Console;
using MySql.Data.MySqlClient;

class Program
{
    static async Task Main(string[] args)
    {
        /*string postgresConnString = string.Format(
            "Host={0};Username={1};Password={2};Database={3}",
            Config.Database.Host,
            Config.Database.User,
            Config.Database.Password,
            Config.Database.Name
        );*/

        string mySqlConnString = string.Format(
            "Server={0};Uid={1};Pwd={2};Database={3};",
            Config.MySql.Host,
            Config.MySql.User,
            Config.MySql.Password,
            Config.MySql.Name
        );

        await Pool.Initialize(DbType.MySQL, mySqlConnString, Config.Pool.StartupSize, Config.Pool.MaxSize, Config.Pool.SizeIncrement);
        var pool = Pool.Instance;

        // Create test table if not exists
        await InitializeTestDatabase(pool);

        var startTime = DateTime.Now;
        var transferAmount = 100m;
        var successCount = 0;
        var failureCount = 0;
        var tasks = new Task[Config.Test.TestUsers];

        AnsiConsole.MarkupLine("[yellow]Starting transaction demo...[/]");
        AnsiConsole.MarkupLine($"[green]Simulating {Config.Test.TestUsers} concurrent transfers of {transferAmount}[/]");

        // Simulate concurrent bank transfers
        for (int i = 0; i < Config.Test.TestUsers; i++)
        {
            int userId = i;
            tasks[i] = Task.Run(async () =>
            {
                using var db = new DbComponent(pool);
                try
                {
                    await db.Begin(DbType.MySQL);

                    // 1. Check source account balance
                    var balance = (decimal?)await db.FetchOne(
                        DbType.MySQL,
                        Config.Queries.Test.GetBalanceWithLock,
                        new MySqlParameter("@id", userId % 2 + 1) // Alternate between accounts 1 and 2
                    );

                    if (!balance.HasValue || balance < transferAmount)
                    {
                        throw new Exception("Insufficient funds");
                    }

                    // 2. Withdraw from source
                    await db.Execute(
                        DbType.MySQL,
                        Config.Queries.Test.Withdraw,
                        new MySqlParameter("@amount", transferAmount),
                        new MySqlParameter("@id", userId % 2 + 1)
                    );

                    // 3. Deposit to target
                    await db.Execute(
                        DbType.MySQL,
                        Config.Queries.Test.Deposit,
                        new MySqlParameter("@amount", transferAmount),
                        new MySqlParameter("@id", (userId % 2) + 2) // Alternate between accounts 2 and 1
                    );

                    // 4. Record transaction
                    await db.Execute(
                        DbType.MySQL,
                        Config.Queries.Test.Transaction.Record,
                        new MySqlParameter("@from", userId % 2 + 1),
                        new MySqlParameter("@to", (userId % 2) + 2),
                        new MySqlParameter("@amount", transferAmount)
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
        using (var db = new DbComponent(pool))
        {
            var account1 = await db.FetchOne(
                DbType.MySQL,
                Config.Queries.Test.GetBalance,
                new MySqlParameter("@id", 1)
            );
            var account2 = await db.FetchOne(
                DbType.MySQL,
                Config.Queries.Test.GetBalance,
                new MySqlParameter("@id", 2)
            );
            var transactionCount = await db.FetchOne(DbType.MySQL, Config.Queries.Test.Transaction.Count);

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
        using var db = new DbComponent(pool);

        await db.Execute(DbType.MySQL, Config.Queries.InitializeDb.Accounts.Table);

        await db.Execute(DbType.MySQL, Config.Queries.InitializeDb.Transactions.Table);

        // Initialize test accounts if empty
        var countResult = await db.FetchOne(DbType.MySQL, Config.Queries.InitializeDb.Accounts.Count);

        int count = 0;
        if (countResult != null && !(countResult is DBNull))
        {
            count = Convert.ToInt32(countResult);
        }

        if (count == 0)
        {
            await db.Execute(DbType.MySQL, Config.Queries.InitializeDb.Accounts.Insert);
        }
        else
        {
            // Reset balances for demo
            await db.Execute(DbType.MySQL, Config.Queries.InitializeDb.Accounts.Reset);
        }

        // Clear previous transactions
        await db.Execute(DbType.MySQL, Config.Queries.InitializeDb.Transactions.Reset);
    }
}