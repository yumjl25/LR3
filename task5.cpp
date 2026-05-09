#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

vector<double> x;
vector<double> y;
int n = 10;

// Функция coffe - вычисляет координаты x и y графика остывания кофе
int coffe(double Tk, double Tsr, double r) {
    x.clear();
    y.clear();
    double t = 0;
    
    for (int i = 0; i < n; i++) {
        x.push_back(t);
        double T = Tsr + (Tk - Tsr) * exp(-r * t);
        y.push_back(T);
        
        if (i < 3) t += 10; //интервалы времени
        else if (i < 6) t += 20;
        else t += 30;
    }
    return 1;
}

// Функция aprox - вычисляет коэффициенты аппроксимирующей прямой a и b
int aprox(double& a, double& b) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    
    double denom = n * sum_x2 - sum_x * sum_x;
    a = (n * sum_xy - sum_x * sum_y) / denom;
    b = (sum_y - a * sum_x) / n;
    return 1;
}

// Функция korrel - вычисляет коэффициент корреляции
double korrel() {
    double mean_x = 0, mean_y = 0;
    
    for (int i = 0; i < n; i++) {
        mean_x += x[i];
        mean_y += y[i];
    }
    mean_x /= n;
    mean_y /= n;
    
    double num = 0, den_x = 0, den_y = 0;
    for (int i = 0; i < n; i++) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;
        num += dx * dy;
        den_x += dx * dx;
        den_y += dy * dy;
    }
    
    return num / sqrt(den_x * den_y);
}

int main() {
    double Tk, Tsr, r;
    double a, b;
    
    cin >> Tk >> Tsr >> r;
    
    coffe(Tk, Tsr, r);
    aprox(a, b);
    double kor = korrel();
    
    cout << fixed << setprecision(2);
    cout << "t\tT\n";
    for (int i = 0; i < n; i++) {
        cout << x[i] << "\t" << y[i] << "\n";
    }
    cout << "a=" << a << "\tb=" << b << "\tr=" << kor << "\n";
    
    return 0;
}