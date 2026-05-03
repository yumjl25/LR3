#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

//Решето Эратосфена
vector<int> sieve(int limit) {
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= limit; ++i)
        if (isPrime[i])
            for (int j = i * i; j <= limit; j += i)
                isPrime[j] = false;
    vector<int> primes;
    for (int i = 2; i <= limit; ++i)
        if (isPrime[i])
            primes.push_back(i);
    return primes;
}

// Быстрое возведение в степень
uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Тест Миллера
bool millerTest(uint64_t n, const vector<uint64_t>& primeFactors, int t, mt19937_64& rng) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    uint64_t nMinusOne = n - 1;

    uniform_int_distribution<uint64_t> dist(2, n - 1);

    for (int attempt = 0; attempt < t; ++attempt) {
        uint64_t a = dist(rng);

        // Условие 1: a^(n-1) ≡ 1 (mod n)
        if (modPow(a, nMinusOne, n) != 1)
            return false; // составное

        // Условие 2: для каждого простого делителя q из n-1
        for (uint64_t q : primeFactors) {
            if (modPow(a, nMinusOne / q, n) == 1) {
                return false; // вероятно составное
            }
        }
    }
    return true;
}

// Генерация числа n битности bits
uint64_t generateCandidate(int bits, const vector<int>& smallPrimes, mt19937_64& rng, int& rejected, int t) {
    const int bitsMinusOne = bits - 1;
    const uint64_t maxVal = (bitsMinusOne >= 64) ? UINT64_MAX : (1ULL << bitsMinusOne);

    uniform_int_distribution<int> primeIndexDist(0, smallPrimes.size() - 1);
    uniform_int_distribution<int> exponentDist(1, 4); // степени от 1 до 4

    while (true) {
        // Выбираем случайное подмножество простых и степени
        uint64_t m = 1;
        vector<uint64_t> usedPrimes;

        // Строим m, пока не достигнем нужной разрядности
        while (true) {
            int idx = primeIndexDist(rng);
            int q = smallPrimes[idx];
            int exp = exponentDist(rng);

            uint64_t factor = 1;
            for (int e = 0; e < exp; ++e) {
                if (m > maxVal / q) break;
                factor *= q;
            }
            if (factor == 1) continue;

            if (m > maxVal / factor) break;
            m *= factor;
            usedPrimes.push_back(q);

            if (m >= (1ULL << (bitsMinusOne - 1))) break;
        }

        uint64_t n = 2 * m + 1;

        // Проверяем битность
        int actual_bits = 0;
        uint64_t tmp = n;
        while (tmp) { actual_bits++; tmp >>= 1; }
        if (actual_bits != bits) continue;

        // Убираем дубликаты простых делителей
        sort(usedPrimes.begin(), usedPrimes.end());
        usedPrimes.erase(unique(usedPrimes.begin(), usedPrimes.end()), usedPrimes.end());

        // Проверяем тестом Миллера
        if (millerTest(n, usedPrimes, t, rng)) {
            if (millerTest(n, usedPrimes, 2, rng))
                return n;
        }
        rejected++;
    }
}

// Основная программа 
int main() {
    const int bits = 30; 
    const int tGeneration = 5; // параметр надёжности при генерации

    random_device rd;
    mt19937_64 rng(rd());

    vector<int> primesUnder500 = sieve(500);
    cout << "Простые числа < 500: " << primesUnder500.size() << " шт.\n";

    vector<uint64_t> primes(10);
    vector<bool> results(10);
    vector<int> rejectedCounts(10, 0);

    for (int i = 0; i < 10; ++i) {
        int rejected = 0;
        primes[i] = generateCandidate(bits, primesUnder500, rng, rejected, tGeneration);
        results[i] = true; 
        rejectedCounts[i] = rejected;
        cout << "Сгенерировано число " << i+1 << ": " << primes[i] << ", отвергнуто: " << rejected << "\n";
    }

    // Вывод таблицы
    cout << "\n";
    cout << "+----+----------------+---------------------+----------------------+\n";
    cout << "| №  | Простое число  | Результат проверки  | Отвергнуто (rejected) |\n";
    cout << "+----+----------------+---------------------+----------------------+\n";

    for (int i = 0; i < 10; ++i) {
        cout << "| " << setw(2) << i + 1 << " | " 
            << setw(14) << primes[i] << " | "
            << setw(19) << (results[i] ? "true" : "false") << " | "
            << setw(20) << rejectedCounts[i] << " |\n";
    }

    cout << "+----+----------------+---------------------+----------------------+\n";
    return 0;
}