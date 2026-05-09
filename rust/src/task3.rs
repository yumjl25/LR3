use std::io;

fn gcd(mut a: i64, mut b: i64) -> i64 {
    while b != 0 {
        let t = b;
        b = a % b;
        a = t;
    }
    a.abs()
}

fn reduce(num: &mut i64, den: &mut i64) {
    let g = gcd(*num, *den);
    *num /= g;
    *den /= g;
}

fn euler_coeff(a: usize) -> Vec<i64> {
    if a == 0 {
        return vec![];
    }
    
    let mut triangle: Vec<Vec<i64>> = vec![vec![]; a + 1];
    
    for n in 1..=a {
        triangle[n] = vec![0; n];
        triangle[n][0] = 1;
        triangle[n][n - 1] = 1;
        
        for k in 1..n - 1 {
            triangle[n][k] = (n - k) as i64 * triangle[n - 1][k - 1]
                           + (k + 1) as i64 * triangle[n - 1][k];
        }
    }
    
    triangle[a].clone()
}

fn compute_sum(a: i32, b: i32) -> String {
    if b == 1 {
        return "infinity".to_string();
    }
    
    let a_usize = a as usize;
    let bb = b as i64;
    
    let coeff = euler_coeff(a_usize);
    
    let mut a = 0i64;
    let mut power = 1i64;
    for k in 0..a_usize {
        a += coeff[k] * power;
        power *= bb;
    }
    
    let numerator = bb * a;
    
    let mut denominator = 1i64;
    for _ in 0..(a + 1) {
        denominator *= bb - 1;
    }
    
    let mut num = numerator;
    let mut den = denominator;
    reduce(&mut num, &mut den);
    
    format!("{}/{}", num, den)
}

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Failed to read line");
    let parts: Vec<i32> = input
        .split_whitespace()
        .map(|x| x.parse().expect("Not a number"))
        .collect();
    let (a, b) = (parts[0], parts[1]);
    println!("{}", compute_sum(a, b));
}