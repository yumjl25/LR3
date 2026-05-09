#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cstdint>

using namespace std;

// НОД для беззнаковых чисел
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Рекуррентное вычисление коэффициентов Эйлера
// Возвращает вектор E(a, k) для k = 0..a-1
vector<uint64_t> eulerCoeff(int a) {
    if (a < 1) return {};
    // Треугольник Эйлера: строки от 1 до a
    vector<vector<uint64_t>> triangle(a + 1);
    
    for (int n = 1; n <= a; n++) {
        triangle[n].resize(n);
        triangle[n][0] = 1;           // первый элемент всегда 1
        triangle[n][n - 1] = 1;       // последний элемент всегда 1
        
        for (int k = 1; k < n - 1; k++) {
            // E(n, k) = (n - k) * E(n-1, k-1) + (k + 1) * E(n-1, k)
            triangle[n][k] = (n - k) * triangle[n - 1][k - 1] 
                           + (k + 1) * triangle[n - 1][k];
        }
    }
    
    return triangle[a];
}

// Функция сокращения дроби
void reduce(uint64_t& num, uint64_t& den) {
    uint64_t g = gcd(num, den);
    num /= g;
    den /= g;
}

// Вычисление суммы ряда
string computeSum(int a, int b) {
    if (b == 1) {
        return "infinity";
    }
    
    uint64_t bb = b;
    
    // Получаем коэффициенты Эйлера для данного a
    vector<uint64_t> coeff = eulerCoeff(a);
    
    // Вычисляем A_a(b) = sum_{k=0}^{a-1} coeff[k] * b^k
    uint64_t A = 0;
    uint64_t power = 1;  // b^0
    for (int k = 0; k < a; k++) {
        A += coeff[k] * power;
        power *= bb;  // переходим к следующей степени b
    }
    
    uint64_t numerator = bb * A;
    uint64_t denominator = 1;
    for (int i = 0; i < a + 1; i++) {
        denominator *= (bb - 1);
    }
    
    reduce(numerator, denominator);
    
    return to_string(numerator) + "/" + to_string(denominator);
}

int main() {
    int a, b;
    cin >> a >> b;
    cout << computeSum(a, b) << endl;
    return 0;
}