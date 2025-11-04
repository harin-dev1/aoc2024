use std::io;
use std::collections::HashMap;

fn dfs(val: String, blink_count: u32, memo: &mut HashMap<String, HashMap<u32, u64>>) -> u64 {
    // Check cache
    if let Some(inner) = memo.get(&val) {
        if let Some(&cached) = inner.get(&blink_count) {
            return cached;
        }
    }

    let result: u64;
    if blink_count == 0 {
        result = 1;
    } else if val == "0" {
        result = dfs("1".to_string(), blink_count - 1, memo);
    } else if val.len() % 2 == 0 {
        let first_half = val[..val.len() / 2].to_string();
        let second_half = val[val.len() / 2..].to_string();
        let first_half = first_half.trim_start_matches('0');
        let second_half = second_half.trim_start_matches('0');
        let first_half = if first_half.is_empty() { "0" } else { first_half };
        let second_half = if second_half.is_empty() { "0" } else { second_half };
        result = dfs(first_half.to_string(), blink_count - 1, memo) + dfs(second_half.to_string(), blink_count - 1, memo);
    } else {
        let num_val: u64 = val.parse().unwrap();
        let new_val = num_val * 2024;
        result = dfs(new_val.to_string(), blink_count - 1, memo);
    }

    // Cache the result
    memo.entry(val).or_insert_with(HashMap::new).insert(blink_count, result);
    result
}

fn main() {
    let mut line = String::new();
    io::stdin().read_line(&mut line).expect("Failed to read input");
    let input: Vec<&str> = line.split_whitespace().collect();

    let mut memo: HashMap<String, HashMap<u32, u64>> = HashMap::new();
    
    let mut res_a = 0u64;
    let mut res_b = 0u64;
    for val in input {
        res_a += dfs(val.to_string(), 25, &mut memo);
        res_b += dfs(val.to_string(), 75, &mut memo);
    }

    println!("{}", res_a);
    println!("{}", res_b);
}