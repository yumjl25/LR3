#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<int64_t> a(n);  // Используем int64_t, потому что числа могут быть отрицательными
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    vector<int64_t> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + a[i];
    }
    
    int64_t scorePavel = 0, scoreVika = 0;
    int pos = 0;
    int last_move = 0;
    bool pavel_turn = true;
    
    while (pos < n) {
        int max_k = min(m, n - pos);
        int best_k = -1;
        int64_t best_sum = LLONG_MIN; 
        
        for (int k = 1; k <= max_k; ++k) {
            if (k == last_move) continue;
            
            int64_t sum = prefix[pos + k] - prefix[pos];
            
            // Исправление: при равной сумме выбираем  k
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