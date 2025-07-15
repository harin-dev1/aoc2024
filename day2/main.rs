use std::io::{self, BufRead};
use std::mem::swap;

fn get_seq_len(vec: &Vec<i32>) -> usize {
    let sign = if vec[1] > vec[0] { 1 } else { -1 };
    for i in 1..vec.len() {
        let diff = (vec[i] - vec[i - 1]) * sign;
        if diff <= 0 || diff > 3 {
            return i;
        }
    }
    vec.len()
}

fn valid_seq(vec: &Vec<i32>) -> bool {
    get_seq_len(vec) == vec.len()
}

fn main() {
    let stdin = io::stdin();
    let lines = stdin.lock().lines();

    let mut res_a = 0;
    let mut res_b = 0;

    for line in lines {
        let line = line.unwrap();
        let mut vec = Vec::new();
        line.split_whitespace().for_each(|x| vec.push(x.parse::<i32>().unwrap()));

        let len = get_seq_len(&vec);
        if len == vec.len() {
            res_a += 1;
            continue;
        }
        if len == vec.len() - 1 {
            res_b += 1;
            continue;
        }

        let mut tmp_val = vec[len];
        vec.remove(len);
        if valid_seq(&vec) {
            res_b += 1;
            continue;
        }
        swap(&mut tmp_val, &mut vec[len - 1]);
        if valid_seq(&vec) {
            res_b += 1;
            continue;
        }
        if len == 2 {
            vec[0] = tmp_val;
            if valid_seq(&vec) {
                res_b += 1;
            }
        }
        
    }
    println!("res_a: {}, res_b: {}", res_a, res_a + res_b);
}