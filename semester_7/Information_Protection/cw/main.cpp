#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class BigInteger {
public:
    vector<int> digits;

    BigInteger() {}
    BigInteger(int value) {
        while (value > 0) {
            digits.push_back(value % 10);
            value /= 10;
        }
    }

    BigInteger(const string& str) {
        for (int i = str.size() - 1; i >= 0; --i) {
            digits.push_back(str[i] - '0');
        }
    }

    BigInteger operator*(const BigInteger& other) const {
        BigInteger result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            for (size_t j = 0; j < other.digits.size(); ++j) {
                result.digits[i + j] += digits[i] * other.digits[j];
                result.digits[i + j + 1] += result.digits[i + j] / 10;
                result.digits[i + j] %= 10;
            }
        }
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        return result;
    }

    BigInteger operator%(const BigInteger& other) const {
        BigInteger quotient = *this / other;
        return *this - (quotient * other);
    }

    BigInteger operator/(const BigInteger& other) const {
        BigInteger quotient;
        BigInteger remainder = *this;
        while (remainder >= other) {
            BigInteger temp = other;
            int shift = 0;
            while (remainder >= (temp * 10)) {
                temp.digits.insert(temp.digits.begin(), 0);
                ++shift;
            }
            while (remainder >= temp) {
                remainder -= temp;
                quotient.digits[shift]++;
            }
        }
        return quotient;
    }

    BigInteger operator-(const BigInteger& other) const {
        BigInteger result = *this;
        for (size_t i = 0; i < other.digits.size(); ++i) {
            if (result.digits[i] < other.digits[i]) {
                result.digits[i] += 10;
                result.digits[i + 1]--;
            }
            result.digits[i] -= other.digits[i];
        }
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        return result;
    }

    bool operator>=(const BigInteger& other) const {
        if (digits.size() != other.digits.size()) {
            return digits.size() > other.digits.size();
        }
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return digits[i] > other.digits[i];
            }
        }
        return true;
    }

    bool operator==(const BigInteger& other) const {
        return digits == other.digits;
    }

    BigInteger ModPow(const BigInteger& exp, const BigInteger& mod) const {
        BigInteger result = 1;
        BigInteger base = *this;
        BigInteger exponent = exp;
        while (exponent > 0) {
            if (exponent.digits[0] % 2 == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exponent = exponent / 2;
        }
        return result;
    }

    string ToString() const {
        string str;
        for (int i = digits.size() - 1; i >= 0; --i) {
            str += to_string(digits[i]);
        }
        return str;
    }

    bool operator>(const BigInteger& other) const {
        return !(*this <= other);
    }

    bool operator<=(const BigInteger& other) const {
        if (digits.size() != other.digits.size()) {
            return digits.size() < other.digits.size();
        }
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return digits[i] < other.digits[i];
            }
        }
        return true;
    }

    bool operator<(const BigInteger& other) const {
        return !(*this >= other);
    }

    BigInteger& operator-=(const BigInteger& other) {
        *this = *this - other;
        return *this;
    }

    BigInteger& operator+=(const BigInteger& other) {
        *this = *this + other;
        return *this;
    }

    BigInteger operator+(const BigInteger& other) const {
        BigInteger result = *this;
        int carry = 0;
        for (size_t i = 0; i < max(digits.size(), other.digits.size()) || carry; ++i) {
            if (i == result.digits.size())
                result.digits.push_back(0);
            result.digits[i] += carry + (i < other.digits.size() ? other.digits[i] : 0);
            carry = result.digits[i] >= 10;
            if (carry)
                result.digits[i] -= 10;
        }
        return result;
    }

    BigInteger operator%(int mod) const {
        int result = 0;
        for (int i = digits.size() - 1; i >= 0; --i) {
            result = (result * 10 + digits[i]) % mod;
        }
        return result;
    }

    // Перегрузка оператора % для int
    BigInteger operator%(const int& mod) const {
        BigInteger result = *this;
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            result.digits[i] %= mod;
        }
        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        return result;
    }
};

class Client {
private:
    BigInteger s;
    BigInteger N;
    BigInteger V;
    BigInteger r;
    BigInteger X;

    BigInteger GenerateSmallRandom(const BigInteger& max) {
        BigInteger result;
        do {
            result = rand() % max;
        } while (result <= 2 || result >= max);
        return result;
    }

public:
    Client(const string& userName) {
        s = 0;
    }

    void Initialize(pair<BigInteger, BigInteger> systemParameters) {
        N = systemParameters.first;
        V = systemParameters.second;
        s = GenerateSmallRandom(N);
        cout << "Закрытый ключ клиента s = " << s.ToString() << endl;
    }

    BigInteger SendX() {
        r = GenerateSmallRandom(N);
        X = r.ModPow(2, N);
        return X;
    }

    BigInteger CalculateY(const BigInteger& c) {
        return (r * s.ModPow(c, N)) % N;
    }
};

class Server {
private:
    BigInteger N;
    BigInteger V;
    string expectedUser;

    BigInteger GenerateSmallPrime() {
        while (true) {
            BigInteger candidate = GenerateSmallRandom(1000);
            if (IsPrime(candidate))
                return candidate;
        }
    }

    bool IsPrime(const BigInteger& number) {
        if (number <= 1) return false;
        if (number <= 3) return true;
        if (number % 2 == 0 || number % 3 == 0) return false;
        for (BigInteger i = 5; i * i <= number; i += 6) {
            if (number % i == 0 || number % (i + 2) == 0)
                return false;
        }
        return true;
    }

    BigInteger GenerateSmallRandom(int max) {
        BigInteger result;
        do {
            result = rand() % max;
        } while (result <= 2 || result >= max);
        return result;
    }

    BigInteger GenerateChallenge() {
        return rand() % 2;
    }

public:
    void GenerateKeys(const string& filePath) {
        srand(time(0));
        expectedUser = "user1";
        BigInteger p = GenerateSmallPrime();
        BigInteger q = GenerateSmallPrime();
        N = p * q;

        BigInteger s = GenerateSmallRandom(1000); // Используем 1000 как максимальное значение для генерации случайного числа
        V = s.ModPow(2, N);

        ofstream file(filePath);
        file << expectedUser << endl;
        file << N.ToString() << endl;
        file << V.ToString() << endl;
        file.close();

        cout << "Ключи сгенерированы:\nN = " << N.ToString() << "\nV = " << V.ToString() << endl;
    }

    pair<BigInteger, BigInteger> GetSystemParameters() {
        return {N, V};
    }

    bool Authenticate(Client& client) {
        BigInteger X = client.SendX();
        BigInteger c = GenerateChallenge();
        BigInteger Y = client.CalculateY(c);

        cout << "X = " << X.ToString() << ", c = " << c.ToString() << ", Y = " << Y.ToString() << endl;
        cout << "Проверка: (Y^2) mod N = " << (Y.ModPow(2, N)).ToString() << ", "
             << "(X * V^c) mod N = " << ((X * V.ModPow(c, N)) % N).ToString() << endl;

        return (Y.ModPow(2, N)) == ((X * V.ModPow(c, N)) % N);
    }
};

int main() {
    Server server;
    server.GenerateKeys("keys.txt");

    Client client("user1");
    client.Initialize(server.GetSystemParameters());

    if (server.Authenticate(client)) {
        cout << "Клиент успешно аутентифицирован." << endl;
    } else {
        cout << "Аутентификация не удалась." << endl;
    }

    return 0;
}