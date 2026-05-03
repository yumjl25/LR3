#include <iostream>
#include <string>
#include <numeric> 
using namespace std;

// Функция сокращения дроби
void reduce(long long& num, long long& den) {
    long long g = gcd(num, den);
    num /= g;
    den /= g;
}

string computeSum(int a, int b) {
    if (b == 1) {
        return "infinity";
    }

    long long numerator = 0;
    long long denominator = 1;
    long long bb = b; 

    // Числитель по формуле: b * A_a(b)
    switch (a) {
        case 1:
            numerator = bb;
            break;
        case 2:
            numerator = bb * (bb + 1);
            break;
        case 3:
            numerator = bb * (bb*bb + 4*bb + 1);
            break;
        case 4:
            numerator = bb * (bb*bb*bb + 11*bb*bb + 11*bb + 1);
            break;
        case 5:
            numerator = bb * (bb*bb*bb*bb + 26*bb*bb*bb + 66*bb*bb + 26*bb + 1);
            break;
        case 6:
            numerator = bb * (bb*bb*bb*bb*bb + 57*bb*bb*bb*bb + 302*bb*bb*bb + 302*bb*bb + 57*bb + 1);
            break;
        case 7:
            numerator = bb * (bb*bb*bb*bb*bb*bb + 120*bb*bb*bb*bb*bb + 1191*bb*bb*bb*bb + 2416*bb*bb*bb + 1191*bb*bb + 120*bb + 1);
            break;
        case 8:
            numerator = bb * (bb*bb*bb*bb*bb*bb*bb + 247*bb*bb*bb*bb*bb*bb + 4293*bb*bb*bb*bb*bb + 15619*bb*bb*bb*bb + 15619*bb*bb*bb + 4293*bb*bb + 247*bb + 1);
            break;
        case 9:
            numerator = bb * (bb*bb*bb*bb*bb*bb*bb*bb + 502*bb*bb*bb*bb*bb*bb*bb + 14608*bb*bb*bb*bb*bb*bb + 88234*bb*bb*bb*bb*bb + 88234*bb*bb*bb*bb + 14608*bb*bb*bb + 502*bb*bb + bb + 1);
            break;
        case 10:
            numerator = bb * (bb*bb*bb*bb*bb*bb*bb*bb*bb + 1013*bb*bb*bb*bb*bb*bb*bb*bb + 47840*bb*bb*bb*bb*bb*bb*bb + 455192*bb*bb*bb*bb*bb*bb + 1310354*bb*bb*bb*bb*bb + 1310354*bb*bb*bb*bb + 455192*bb*bb*bb + 47840*bb*bb + 1013*bb + 1);
            break;
        default:
            return "infinity";
    }

    // Знаменатель: (b - 1)^(a + 1)
    denominator = 1;
    for (int i = 0; i < a + 1; ++i) {
        denominator *= (b - 1);
    }

    // Сокращаем дробь
    reduce(numerator, denominator);

    return to_string(numerator) + "/" + to_string(denominator);
}

int main() {
    int a, b;
    cin >> a >> b;
    cout << computeSum(a, b) << endl;
    return 0;
}