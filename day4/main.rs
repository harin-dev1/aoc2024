use std::io;

fn get_count_a(input: &Vec<String>, i : usize, j : usize) -> i32{
    let mut count = 0;
    let search_string = "XMAS";
    for di in -1i32..=1 {
        for dj in -1i32..=1 {
            if di == 0 && dj == 0 {
                continue;
            }
            let mut found = true;
            for (idx, c) in search_string.chars().enumerate() {
                let ni = i as i32 + idx as i32 * di;
                let nj = j as i32 + idx as i32 * dj;
                if ni < 0 || ni >= input.len() as i32 || nj < 0 || nj >= input[ni as usize].chars().count() as i32 {   
                    found = false;
                    break;
                }
                if c != input[ni as usize].chars().nth(nj as usize).unwrap() {
                    found = false;
                    break;
                }
            }
            if found {
                count += 1;
            }
        }
    }
    count
}

fn is_valid_b(input: &Vec<String>, i : usize, j : usize) -> bool {
    if (i as i32) < 1 || (i as i32) >= (input.len() as i32 - 1) || (j as i32) < 1 || (j as i32) >= (input[i as usize].chars().count() as i32 - 1)    {
        return false;
    }
    let iminus1 = i as i32 - 1;
    let iplus1 = i as i32 + 1;
    let jminus1 = j as i32 - 1;
    let jplus1 = j as i32 + 1;
    
    let mut s1 : String = String::new();
    s1.push(input[iminus1 as usize].chars().nth(jminus1 as usize).unwrap());
    s1.push(input[iplus1 as usize].chars().nth(jplus1 as usize).unwrap());

    let mut s2 : String = String::new();
    s2.push(input[iminus1 as usize].chars().nth(jplus1 as usize).unwrap());
    s2.push(input[iplus1 as usize].chars().nth(jminus1 as usize).unwrap());

    if (s1 == "MS" || s1 == "SM") && (s2 == "MS" || s2 == "SM") {
        return true;
    }
    false
}

fn main() {
    let mut input: Vec<String> = Vec::new();

    for line in io::stdin().lines() {
        input.push(line.unwrap());
    }

    let mut res_a = 0;
    let mut res_b = 0;
    for i in 0..input.len() {
        for j in 0..input[i].len() {
            if input[i].chars().nth(j).unwrap() == 'X' {
                res_a += get_count_a(&input, i, j);
            }
            if input[i].chars().nth(j).unwrap() == 'A' {
                if is_valid_b(&input, i, j) {
                    res_b += 1;
                }
            }
        }
    }
    println!("{}", res_a);
    println!("{}", res_b);
}