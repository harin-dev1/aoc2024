use std::io::{self, BufRead};
use regex::Regex;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let re_a = Regex::new(r"mul\((\d+),(\d+)\)")?;
    let re_b = Regex::new(r"do\(\)|don't\(\)|mul\((\d+),(\d+)\)")?;
    
    let stdin = io::stdin();
    let mut res_a = 0u64;
    let mut res_b = 0u64;
    let mut is_enabled = true;
    
    for line in stdin.lock().lines() {
        let line = line?;
        
        // Part A: Sum all mul(x,y) operations
        for caps in re_a.captures_iter(&line) {
            let a: u64 = caps[1].parse()?;
            let b: u64 = caps[2].parse()?;
            res_a += a * b;
        }
        
        // Part B: Process do(), don't(), and mul(x,y) operations
        for caps in re_b.captures_iter(&line) {
            let full_match = &caps[0];
            
            match full_match {
                "do()" => is_enabled = true,
                "don't()" => is_enabled = false,
                _ if is_enabled => {
                    // This is a mul(x,y) operation and we're enabled
                    let a: u64 = caps[1].parse()?;
                    let b: u64 = caps[2].parse()?;
                    res_b += a * b;
                }
                _ => {} // mul(x,y) but disabled, do nothing
            }
        }
    }
    
    println!("{}", res_a);
    println!("{}", res_b);
    
    Ok(())
} 