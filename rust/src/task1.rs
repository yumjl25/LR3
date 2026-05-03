use std::f64;

fn f(x: f64) -> Option<f64> {
    if x >= -10.0 && x <= -6.0 {
        let radicand = 4.0 - (x + 8.0).powi(2);
        if radicand < 0.0 {
            return None;
        }
        return Some(-2.0 + radicand.sqrt());
    }
    else if x >= -6.0 && x <= -1.0 {
        return Some(0.6 * x + 1.6);
    }
    else if x >= 2.0 {
        return Some((x - 2.0).powi(2));
    }
    else {
        return None;
    }
}

fn main() {
    let x_nach = -10.0;
    let x_kon = 10.0; 
    let dx = 1.0;

    println!("   x    |    y");
    println!("--------+----------");

    let mut x = x_nach;
    while x <= x_kon + 1e-9 {
        match f(x) {
            Some(y) => println!("{:7.4} | {:8.4}", x, y),
            None => println!("{:7.4} |   не опр.", x),
        }
        x += dx;
    }
}