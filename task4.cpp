#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // Префиксные суммы для быстрого подсчёта суммы отрезка
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + a[i];
    }
    
    long long scorePavel = 0, scoreVika = 0;
    int pos = 0; 
    int last_move = 0;     // сколько чисел стёр противник в предыдущем ходу 
    bool pavel_turn = true;
    
    while (pos < n) {
        int max_k = min(m, n - pos);
        int best_k = -1;
        long long best_sum = -1e18;
        
        for (int k = 1; k <= max_k; ++k) {
            // Запрет: нельзя брать столько же, сколько стёр противник в прошлом ходу
            if (k == last_move) continue;
            
            long long sum = prefix[pos + k] - prefix[pos];
            if (sum > best_sum || (sum == best_sum && k < best_k)) {
                best_sum = sum;
                best_k = k;
            }
        }
        
        
        if (pavel_turn) {
            scorePavel += best_sum;
        } else {
            scoreVika += best_sum;
        }
        
        pos += best_k;
        last_move = best_k; 
        pavel_turn = !pavel_turn;
    }
    
    if (scorePavel > scoreVika) {
        cout << 1 << endl;
    } else {
        cout << 0 << endl;
    }
    
    return 0;
}