use std::io;

// НОД для сокращения дроби
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

fn compute_sum(a: i32, b: i32) -> String {
    if b == 1 {
        return "infinity".to_string();
    }

    let b = b as i64;
    let numerator = match a {
        1 => b,
        2 => b * (b + 1),
        3 => b * (b * b + 4 * b + 1),
        4 => b * (b * b * b + 11 * b * b + 11 * b + 1),
        5 => b * (b * b * b * b + 26 * b * b * b + 66 * b * b + 26 * b + 1),
        6 => b * (b * b * b * b * b + 57 * b * b * b * b + 302 * b * b * b + 302 * b * b + 57 * b + 1),
        7 => b * (b * b * b * b * b * b + 120 * b * b * b * b * b + 1191 * b * b * b * b + 2416 * b * b * b + 1191 * b * b + 120 * b + 1),
        8 => b * (b * b * b * b * b * b * b + 247 * b * b * b * b * b * b + 4293 * b * b * b * b * b + 15619 * b * b * b * b + 15619 * b * b * b + 4293 * b * b + 247 * b + 1),
        9 => b * (b * b * b * b * b * b * b * b + 502 * b * b * b * b * b * b * b + 14608 * b * b * b * b * b * b + 88234 * b * b * b * b * b + 88234 * b * b * b * b + 14608 * b * b * b + 502 * b * b + b + 1),
        10 => b * (b * b * b * b * b * b * b * b * b + 1013 * b * b * b * b * b * b * b * b + 47840 * b * b * b * b * b * b * b + 455192 * b * b * b * b * b * b + 1310354 * b * b * b * b * b + 1310354 * b * b * b * b + 455192 * b * b * b + 47840 * b * b + 1013 * b + 1),
        _ => return "infinity".to_string(),
    };

    // Знаменатель: (b - 1)^(a + 1)
    let mut denominator = 1i64;
    for _ in 0..(a + 1) {
        denominator *= b - 1;
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