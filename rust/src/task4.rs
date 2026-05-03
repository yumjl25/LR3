use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    let m: usize = iter.next().unwrap().parse().unwrap();
    
    let mut a = Vec::with_capacity(n);
    let mut line = String::new();
    io::stdin().read_line(&mut line).unwrap();
    for num in line.split_whitespace() {
        a.push(num.parse::<i64>().unwrap());
    }
    
    // Префиксные суммы
    let mut prefix = vec![0i64; n + 1];
    for i in 0..n {
        prefix[i + 1] = prefix[i] + a[i];
    }
    
    let mut score_pavel = 0i64;
    let mut score_vika = 0i64;
    let mut pos = 0;
    let mut last_move = 0; // сколько стёр противник в предыдущем ходу
    let mut pavel_turn = true;
    
    while pos < n {
        let max_k = std::cmp::min(m, n - pos);
        let mut best_k = 0;
        let mut best_sum = i64::MIN;
        
        for k in 1..=max_k {
            if k == last_move {
                continue;
            }
            let sum = prefix[pos + k] - prefix[pos];
            if sum > best_sum || (sum == best_sum && k < best_k) {
                best_sum = sum;
                best_k = k;
            }
        }
        
        if pavel_turn {
            score_pavel += best_sum;
        } else {
            score_vika += best_sum;
        }
        
        pos += best_k;
        last_move = best_k;
        pavel_turn = !pavel_turn;
    }
    
    if score_pavel > score_vika {
        println!("1");
    } else {
        println!("0");
    }
}