#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cstdint>

using namespace std;


// Возвращает вектор всех простых чисел от 2 до limit
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


// Вычисляет (base^exp) % mod за O
uint64_t modPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)                    // если текущий бит степени = 1
            result = (result * base) % mod;
        base = (base * base) % mod;     // возводим основание в квадрат
        exp >>= 1;                      // переходим к следующему биту
    }
    return result;
}


// Проверяет, является ли n простым, используя простые делители n-1
bool millerTest(uint64_t n, const vector<uint64_t>& primeFactors, int t, mt19937_64& rng) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    uint64_t nMinusOne = n - 1;
    uniform_int_distribution<uint64_t> dist(2, n - 1);

    for (int attempt = 0; attempt < t; ++attempt) {
        uint64_t a = dist(rng);

        if (modPow(a, nMinusOne, n) != 1)
            return false;

        bool all_one = true;
        for (uint64_t q : primeFactors) {
            if (modPow(a, nMinusOne / q, n) != 1) {
                all_one = false;
                break;
            }
        }
        if (all_one) return false; // все q дали 1 → составное
    }
    return true;
}

// Возвращает количество битов, необходимых для хранения числа n
int getBitCount(uint64_t n) {
    int bits = 0;
    while (n > 0) {
        bits++;
        n >>= 1;
    }
    return bits;
}


uint64_t generateCandidate(int bits, const vector<int>& smallPrimes, mt19937_64& rng, int& rejected, int t) {
    // Корректировка битности (работаем только с 3..64)
    if (bits < 3) bits = 3;
    if (bits > 64) bits = 64;

    const int bitsMinusOne = bits - 1;
    
    // Минимальное и максимальное значения для m (m должно быть (bits-1)-битным)
    uint64_t minM = 1ULL << (bitsMinusOne - 1);          // 2^(bits-2)
    uint64_t maxM = (1ULL << bitsMinusOne) - 1;          // 2^(bits-1) - 1
    
    // Минимальное и максимальное значения для n = 2m+1 (должно быть bits-битным)
    uint64_t minN = 1ULL << (bits - 1);                  // 2^(bits-1)
    uint64_t maxN = (1ULL << bits) - 1;                  // 2^bits - 1
    
    // Генераторы случайных индексов и степеней
    uniform_int_distribution<int> primeIndexDist(0, smallPrimes.size() - 1);
    uniform_int_distribution<int> exponentDist(1, 4); 

    while (true) {
        uint64_t m = 1;
        vector<uint64_t> usedPrimes;   // простые делители, использованные при построении m

        // Строим m как произведение случайных простых в случайных степенях
        while (m < minM) {
            int idx = primeIndexDist(rng);
            int q = smallPrimes[idx];
            int exp = exponentDist(rng);

            // Вычисляем factor = q^exp с защитой от переполнения
            uint64_t factor = 1;
            for (int e = 0; e < exp; ++e) {
                if (factor > maxM / q) {
                    factor = 1;
                    break;
                }
                factor *= q;
            }
            if (factor == 1) continue;   // не удалось получить множитель

            if (m > maxM / factor) continue;   // умножение выйдет за maxM
            m *= factor;
            usedPrimes.push_back(q);
        }

        // Формируем кандидата n = 2m + 1
        uint64_t n = 2 * m + 1;

        // Проверка битности
        int actualBits = getBitCount(n);
        if (actualBits != bits) continue;
        
        // Дополнительная проверка диапазона
        if (n < minN || n > maxN) continue;

        // Убираем дубликаты простых делителей
        sort(usedPrimes.begin(), usedPrimes.end());
        usedPrimes.erase(unique(usedPrimes.begin(), usedPrimes.end()), usedPrimes.end());
        
        // Добавляем 2 как обязательный простой делитель n-1 (потому что n-1 = 2m)
        usedPrimes.push_back(2);
        sort(usedPrimes.begin(), usedPrimes.end());
        usedPrimes.erase(unique(usedPrimes.begin(), usedPrimes.end()), usedPrimes.end());

        // Проверяем тестом Миллера (сначала с t, затем дополнительно с t=2)
        if (millerTest(n, usedPrimes, t, rng)) {
            if (millerTest(n, usedPrimes, 2, rng))
                return n;
        }
        rejected++;   // увеличиваем счётчик отвергнутых кандидатов
    }
}

int main() {
    int bits;
    cout << "Введите битность: ";
    cin >> bits;
    
    // Проверка на допустимые значения
    if (bits < 3) {
        cout << "Битность меньше 3. Устанавливаем 3 бита.\n";
        bits = 3;
    }
    if (bits > 64) {
        cout << "Битность больше 64. Устанавливаем 64 бита.\n";
        bits = 64;
    }
    
    const int tGeneration = 5;   // параметр надёжности при генерации

    random_device rd; 
    mt19937_64 rng(rd());

    // Получаем простые числа до 500
    vector<int> primesUnder500 = sieve(500);
    cout << "Простые числа < 500: " << primesUnder500.size() << " шт.\n";
    
    // Границы диапазона для вывода
    uint64_t minValue = 1ULL << (bits - 1);
    uint64_t maxValue = (1ULL << bits) - 1;
    
    cout << "Генерируем " << bits << "-битные простые числа\n";
    cout << "Диапазон: [" << minValue << ", " << maxValue << "]\n\n";

    // Контейнеры для хранения результатов
    vector<uint64_t> primes(10);
    vector<int> rejectedCounts(10, 0);

    // Генерируем 10 РАЗЛИЧНЫХ простых чисел
    for (int i = 0; i < 10; ++i) {
        int rejected = 0;
        uint64_t candidate;
        bool unique;
        
        do {
            unique = true;
            candidate = generateCandidate(bits, primesUnder500, rng, rejected, tGeneration);
            
            // Проверяем, не было ли такого числа уже
            for (int j = 0; j < i; ++j) {
                if (primes[j] == candidate) {
                    unique = false;
                    rejected++; // повтор считается отвергнутым
                    break;
                }
            }
        } while (!unique);
        
        primes[i] = candidate;
        rejectedCounts[i] = rejected;
        
        int actualBits = getBitCount(primes[i]);
        
        cout << "Число " << i + 1 << ": " << primes[i] 
             << " (битность: " << actualBits << "/" << bits << ")"
             << ", отвергнуто: " << rejected << "\n";
    }

    cout << "\n";
    cout << "+----+---------------------+---------------------+----------------------+\n";
    cout << "| №  |   Простое число     |  Результат проверки  | Отвергнуто (rejected) |\n";
    cout << "+----+---------------------+---------------------+----------------------+\n";

    for (int i = 0; i < 10; ++i) {
        cout << "| " << setw(2) << i + 1 << " | " 
             << setw(19) << primes[i] << " | "
             << setw(19) << "true" << " | "
             << setw(20) << rejectedCounts[i] << " |\n";
    }

    cout << "+----+---------------------+---------------------+----------------------+\n";

    return 0;
}