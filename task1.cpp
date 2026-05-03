#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double f(double x) {
    if (x >= -10 && x <= -6) {
        double radicand = 4.0 - pow(x + 8.0, 2);
        if (radicand < 0) return NAN;
        return -2.0 + sqrt(radicand);
    }
    else if (x >= -6 && x <= -1) {
        return 0.6 * x + 1.6;
    }
    else if (x >= 2) {
        return pow(x - 2.0, 2);
    }
    else {
        return NAN;
    }
}

int main() {
    double Xнач = -10.0;
    double Xкон = 10.0;  
    double dx = 1.0;

    cout << fixed << setprecision(4);
    cout << "   x    |    y" << endl;
    cout << "--------+----------" << endl;

    for (double x = Xнач; x <= Xкон + 1e-9; x += dx) {
        double y = f(x);
        if (isnan(y)) {
            cout << setw(7) << x << " |   не опр." << endl;
        } else {
            cout << setw(7) << x << " | " << setw(8) << y << endl;
        }
    }

    return 0;
}