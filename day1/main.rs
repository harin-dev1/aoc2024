use std::io::{self, BufRead};

fn main() {
    let stdin = io::stdin();
    let lines = stdin.lock().lines();
    
    let mut vals_a = Vec::new();
    let mut vals_b = Vec::new();

    for line in lines {
        let line = line.unwrap();
        let (a, b) = line.split_once(" ").unwrap();
        let (a, b) = (a.trim().parse::<u32>().unwrap(), b.trim().parse::<u32>().unwrap());
        vals_a.push(a);
        vals_b.push(b);
    }

    vals_a.sort();
    vals_b.sort();

    let mut res_a = 0;

    for i in 0..vals_a.len() {
        res_a += (vals_a[i] as i32 - vals_b[i] as i32).abs() as u64;
    }

    println!("res_a: {}", res_a);

    let mut res_b = 0;
    let mut b_idx = 0;
    let mut a_idx = 0;

    while a_idx < vals_a.len() {
        while b_idx < vals_b.len() && vals_b[b_idx] < vals_a[a_idx] {
            b_idx += 1;
        }
        let mut count = 0;
        while b_idx < vals_b.len() && vals_b[b_idx] == vals_a[a_idx] {
            count += 1;
            b_idx += 1;
        }
        res_b += vals_a[a_idx] * count;
        a_idx += 1;
        while a_idx < vals_a.len() && vals_a[a_idx] == vals_a[a_idx - 1] {
            res_b += vals_a[a_idx] * count;
            a_idx += 1;
        }
        if b_idx == vals_b.len() {
            break;
        }
    }

    println!("res_b: {}", res_b);
    
}