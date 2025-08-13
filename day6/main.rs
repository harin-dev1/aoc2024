use std::io::{self, BufRead};

fn has_loop(map: &Vec<Vec<char>>, start: (usize, usize)) -> bool {
    let mut cur_pos = start;
    let mut cur_dir = 0;
    let dirs: [(i32, i32); 4] = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let mut visited: Vec<Vec<i32>> =
        vec![vec![-1; map[0].len()]; map.len()];
    visited[start.0][start.1] = 0;

    loop {
        let next_pos = (
            cur_pos.0 as i32 + dirs[cur_dir].0,
            cur_pos.1 as i32 + dirs[cur_dir].1,
        );

        if next_pos.0 < 0
            || next_pos.0 as usize >= map.len()
            || next_pos.1 < 0
            || next_pos.1 as usize >= map[0].len()
        {
            break;
        }
        if map[next_pos.0 as usize][next_pos.1 as usize] == '#' {
            cur_dir = (cur_dir + 1) % 4;
        } else {
            let next_pos = (next_pos.0 as usize, next_pos.1 as usize);
            if visited[next_pos.0][next_pos.1] == cur_dir as i32 {
                return true;
            }
            visited[next_pos.0][next_pos.1] = cur_dir as i32;
            cur_pos = next_pos;
        }
    }
    return false;
}

fn main() {
    let mut map = Vec::new();

    for line in io::stdin().lock().lines() {
        map.push(line.unwrap().chars().collect::<Vec<char>>());
    }

    let mut start = (0, 0);
    'outer: for (i, val) in map.iter().enumerate() {
        for (j, &c) in val.iter().enumerate() {
            if c == '^' {
                start = (i, j);
                break 'outer;
            }
        }
    }

    let mut res_a = 1u32;
    let mut cur_pos = start;
    let mut cur_dir = 0;
    let dirs: [(i32, i32); 4] = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let mut visited: Vec<Vec<bool>> =
        vec![vec![false; map[0].len()]; map.len()];
    visited[start.0][start.1] = true;

    loop {
        let next_pos = (
            cur_pos.0 as i32 + dirs[cur_dir].0,
            cur_pos.1 as i32 + dirs[cur_dir].1,
        );

        if next_pos.0 < 0
            || next_pos.0 as usize >= map.len()
            || next_pos.1 < 0
            || next_pos.1 as usize >= map[0].len()
        {
            break;
        }
        let next_pos = (next_pos.0 as usize, next_pos.1 as usize);
        if map[next_pos.0][next_pos.1] == '#' {
            cur_dir = (cur_dir + 1) % 4;
        } else {
            if !visited[next_pos.0][next_pos.1] {
                visited[next_pos.0][next_pos.1] = true;
                res_a += 1;
            }
            cur_pos = next_pos;
        }
    }

    let mut res_b = 0;
    for i in 0..map.len() {
        for j in 0..map[0].len() {
            if map[i][j] == '^' || map[i][j] == '#' {
                continue;
            }
            let original = map[i][j];
            map[i][j] = '#';
            if has_loop(&map, start) {
                res_b += 1;
            }
            map[i][j] = original;
        }
    }

    println!("{}", res_a);
    println!("{}", res_b);
}