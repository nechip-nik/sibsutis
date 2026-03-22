using System;
using System.IO;
using System.Numerics;
using System.Security.Cryptography;

class Program
{
    static void Main(string[] args)
    {
        Server server = new Server();
        server.GenerateKeys("keys.txt");

        Client client = new Client("user1");
        client.Initialize(server.GetSystemParameters());

        if (server.Authenticate(client))
        {
            Console.WriteLine("Клиент успешно аутентифицирован.");
        }
        else
        {
            Console.WriteLine("Аутентификация не удалась.");
        }
    }
}

public class Server
{
    private BigInteger N;
    private BigInteger V;
    private string expectedUser;

    public void GenerateKeys(string filePath)
    {
        // Генерация ключей
        expectedUser = "user1";
        BigInteger p = GenerateSmallPrime();
        BigInteger q = GenerateSmallPrime();
        N = p * q;

        BigInteger s = GenerateSmallRandom(N);
        V = BigInteger.ModPow(s, 2, N); 

        File.WriteAllLines(filePath, new string[]
        {
            expectedUser,
            N.ToString(),
            V.ToString()
        });

        Console.WriteLine($"Ключи сгенерированы:\nN = {N}\nV = {V}");
    }

    public (BigInteger N, BigInteger V) GetSystemParameters()
    {
        return (N, V);
    }

    public bool Authenticate(Client client)
    {
        BigInteger X = client.SendX();
        BigInteger c = GenerateChallenge();
        BigInteger Y = client.CalculateY(c);

        Console.WriteLine($"X = {X}, c = {c}, Y = {Y}");
        Console.WriteLine($"Проверка: (Y^2) mod N = {(Y * Y) % N}, " +
                          $"(X * V^c) mod N = {(X * BigInteger.ModPow(V, c, N)) % N}");

        return (Y * Y) % N == (X * BigInteger.ModPow(V, c, N)) % N;
    }

    private BigInteger GenerateChallenge()
    {
        return new Random().Next(0, 2); 
    }

    private BigInteger GenerateSmallPrime()
    {
        while (true)
        {
            BigInteger candidate = GenerateSmallRandom(1000); 
            if (IsPrime(candidate))
                return candidate;
        }
    }

    private bool IsPrime(BigInteger number)
    {
        if (number <= 1) return false;
        if (number <= 3) return true;
        if (number % 2 == 0 || number % 3 == 0) return false;
        for (BigInteger i = 5; i * i <= number; i += 6)
        {
            if (number % i == 0 || number % (i + 2) == 0)
                return false;
        }
        return true;
    }

    private BigInteger GenerateSmallRandom(BigInteger max)
    {
        using (RandomNumberGenerator rng = RandomNumberGenerator.Create())
        {
            byte[] bytes = max.ToByteArray();
            BigInteger result;
            do
            {
                rng.GetBytes(bytes);
                result = new BigInteger(bytes);
            } while (result <= 2 || result >= max); 
            return result;
        }
    }
}

public class Client
{
    private BigInteger s;
    private BigInteger N;
    private BigInteger V;
    private BigInteger r; 
    private BigInteger X; 

    public Client(string userName)
    {
        s = 0;
    }

    public void Initialize((BigInteger N, BigInteger V) systemParameters)
    {
        N = systemParameters.N;
        V = systemParameters.V;
        s = GenerateSmallRandom(N); 
        Console.WriteLine($"Закрытый ключ клиента s = {s}");
    }

    public BigInteger SendX()
    {
        r = GenerateSmallRandom(N); 
        X = BigInteger.ModPow(r, 2, N);
        return X;
    }

    public BigInteger CalculateY(BigInteger c)
    {
        return (r * BigInteger.ModPow(s, c, N)) % N;
    }

    private BigInteger GenerateSmallRandom(BigInteger max)
    {
        using (RandomNumberGenerator rng = RandomNumberGenerator.Create())
        {
            byte[] bytes = max.ToByteArray();
            BigInteger result;
            do
            {
                rng.GetBytes(bytes);
                result = new BigInteger(bytes);
            } while (result <= 2 || result >= max);
            return result;
        }
    }
}
