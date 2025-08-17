use std::io::{self, BufRead};
use regex::Regex;

fn can_evaluate_to(nums: &Vec<u64>, target: u64,
current: u64, index: u64, use_concat: bool) -> bool {
    if index == nums.len() as u64 {
        return current == target;
    }

    if current > target {
        return false;
    }

    let concat_val: u64 = format!("{}{}", current, nums[index as usize]).parse().unwrap();
    can_evaluate_to(nums, target, current * nums[index as usize], index + 1, use_concat) ||
    can_evaluate_to(nums, target, current + nums[index as usize], index + 1, use_concat) ||
    (use_concat && can_evaluate_to(nums, target, concat_val, index + 1, use_concat))
}

fn main() {
    let reg_rule = Regex::new(r"^(\d+):\s(.+)$").unwrap();
    let mut res_a = 0u64;
    let mut res_b = 0u64;

    for line in io::stdin().lock().lines() {
        let line = line.unwrap();
        let caps = reg_rule.captures(&line).unwrap();
        let test_num: u64 = caps[1].parse().unwrap();
        let nums: Vec<u64> = caps.get(2).unwrap().as_str().split_whitespace().map(|x| x.parse::<u64>().unwrap()).collect();
        if can_evaluate_to(&nums, test_num, nums[0], 1, false) {
            res_a += test_num;
        } else if can_evaluate_to(&nums, test_num, nums[0], 1, true) {
            res_b += test_num;
        }
    }

    println!("{}", res_a);
    println!("{}", res_a + res_b);
}