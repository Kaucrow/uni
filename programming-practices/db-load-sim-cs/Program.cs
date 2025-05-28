using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Npgsql;

class Program
{
    static async Task Main()
    {
        // Database connection string
        string connString = "Host=localhost;Username=postgres;Password=postgrespass;Database=test;Pooling=false";

        // Number of simulated users
        int numUsers = 1000;

        // How many times each user will try to connect
        int attemptsPerUser = 10;

        Console.WriteLine($"Starting simulation with {numUsers} users...");

        var startTime = Stopwatch.StartNew();
        var tasks = new List<Task<(int successfulConnections, long totalQueryTimeMs)>>();

        for (int userId = 0; userId < numUsers; userId++)
        {
            int currentUserId = userId; // Capture the value for the closure
            tasks.Add(Task.Run(async () =>
            {
                var random = new Random();
                int successfulConnections = 0;
                long totalQueryTimeMs = 0;

                for (int attempt = 0; attempt < attemptsPerUser; attempt++)
                {
                    // Random delay between attempts (0-100ms)
                    int delay = random.Next(0, 100);
                    await Task.Delay(delay);

                    var connectStart = Stopwatch.StartNew();
                    try
                    {
                        await using var conn = new NpgsqlConnection(connString);
                        await conn.OpenAsync();
                        
                        var connectTime = connectStart.Elapsed;
                        totalQueryTimeMs += connectTime.Milliseconds;
                        successfulConnections++;

                        // Simulate a simple query
                        try
                        {
                            await using var cmd = new NpgsqlCommand("SELECT 1", conn);
                            await cmd.ExecuteScalarAsync();

                            Console.WriteLine($"User {currentUserId} query successful on attempt {attempt}");
                            
                            // Simulate some "work" time (10-50ms)
                            await Task.Delay(random.Next(10, 50));
                        }
                        catch (Exception e)
                        {
                            Console.Error.WriteLine($"User {currentUserId} query failed on attempt {attempt}: {e.Message}");
                        }
                    }
                    catch (Exception e)
                    {
                        Console.Error.WriteLine($"User {currentUserId} connection failed on attempt {attempt}: {e.Message}");
                    }
                }

                return (successfulConnections, totalQueryTimeMs);
            }));
        }

        // Collect results
        int totalSuccessful = 0;
        long totalQueryTimeMs = 0;
        
        foreach (var task in tasks)
        {
            var (successful, queryTime) = await task;
            totalSuccessful += successful;
            totalQueryTimeMs += queryTime;
        }

        int totalAttempts = numUsers * attemptsPerUser;
        double successRate = (double)totalSuccessful / totalAttempts * 100.0;
        double avgQueryTime = totalSuccessful > 0 ? (double)totalQueryTimeMs / totalSuccessful : 0;

        Console.WriteLine($"\nSimulation completed in {startTime.Elapsed.TotalSeconds:F2}s");
        Console.WriteLine($"Total connection attempts: {totalAttempts}");
        Console.WriteLine($"Successful connections: {totalSuccessful} ({successRate:F2}%)");
        Console.WriteLine($"Average connection time: {avgQueryTime:F2}ms");
    }
}