use std::collections::HashMap;
use std::io::{self, BufRead};

#[derive(Debug, Clone, Copy, PartialEq)]
enum Antenna {
    None,
    A,
    B,
}

type Grid = Vec<String>;
type AntennaGrid = Vec<Vec<Antenna>>;
type AntennaMap = HashMap<char, Vec<(usize, usize)>>;
type Position = (usize, usize);

// Read input grid from stdin
fn read_grid() -> Grid {
    let stdin = io::stdin();
    let mut grid = Vec::new();
    
    for line in stdin.lock().lines() {
        if let Ok(line) = line {
            grid.push(line);
        }
    }
    
    grid
}

// Build map of antenna positions by frequency
fn build_antenna_map(map: &Grid) -> AntennaMap {
    let mut antenna_map = HashMap::new();
    
    for (i, row) in map.iter().enumerate() {
        for (j, ch) in row.chars().enumerate() {
            if ch != '.' {
                antenna_map.entry(ch).or_insert_with(Vec::new).push((i, j));
            }
        }
    }
    
    antenna_map
}

// Check if position is within grid bounds
fn is_in_bounds(map: &Grid, x: isize, y: isize) -> bool {
    x >= 0 && (x as usize) < map.len() && y >= 0 && (y as usize) < map[0].len()
}

// Set antenna node if position is empty
fn set_antenna_node(antenodes: &mut AntennaGrid, x: usize, y: usize, antenna_type: Antenna) {
    if antenna_type == Antenna::A || antenodes[x][y] == Antenna::None {
        antenodes[x][y] = antenna_type;
    }
}

// Process antenna pairs to find antinodes
fn process_antenna_pair(
    map: &Grid,
    antenodes: &mut AntennaGrid,
    pos1: Position,
    pos2: Position,
) {
    let (x1, y1) = (pos1.0 as isize, pos1.1 as isize);
    let (x2, y2) = (pos2.0 as isize, pos2.1 as isize);
    
    let dx = x2 - x1;
    let dy = y2 - y1;

    // Part B: Mark all positions along the line (including antenna positions)
    let mut nx1 = x1;
    let mut ny1 = y1;
    let mut nx2 = x2;
    let mut ny2 = y2;

    // Extend backwards from first antenna
    while is_in_bounds(map, nx1, ny1) {
        set_antenna_node(antenodes, nx1 as usize, ny1 as usize, Antenna::B);
        nx1 -= dx;
        ny1 -= dy;
    }

    // Extend forwards from second antenna
    while is_in_bounds(map, nx2, ny2) {
        set_antenna_node(antenodes, nx2 as usize, ny2 as usize, Antenna::B);
        nx2 += dx;
        ny2 += dy;
    }

    // Part A: Mark specific antinode positions
    let nx1 = x1 - dx;
    let ny1 = y1 - dy;
    let nx2 = x2 + dx;
    let ny2 = y2 + dy;

    if is_in_bounds(map, nx1, ny1) {
        antenodes[nx1 as usize][ny1 as usize] = Antenna::A;
    }
    if is_in_bounds(map, nx2, ny2) {
        antenodes[nx2 as usize][ny2 as usize] = Antenna::A;
    }
}

// Process all antennas to find antinodes
fn process_antennas(map: &Grid, antenodes: &mut AntennaGrid, antenna_map: &AntennaMap) {
    for (_frequency, nodes) in antenna_map {
        if nodes.len() <= 1 {
            continue;
        }

        // Process all pairs of antennas with same frequency
        for i in 0..nodes.len() {
            for j in (i + 1)..nodes.len() {
                process_antenna_pair(map, antenodes, nodes[i], nodes[j]);
            }
        }
    }
}

// Count results for both parts
fn count_results(antenodes: &AntennaGrid) -> (i32, i32) {
    let mut res_a = 0;
    let mut res_b = 0;
    
    for row in antenodes {
        for &cell in row {
            match cell {
                Antenna::A => {
                    res_a += 1;
                    res_b += 1;
                }
                Antenna::B => {
                    res_b += 1;
                }
                Antenna::None => {}
            }
        }
    }
    
    (res_a, res_b)
}

fn main() {
    let map = read_grid();
    if map.is_empty() {
        return;
    }

    let mut antenodes = vec![vec![Antenna::None; map[0].len()]; map.len()];
    let antenna_map = build_antenna_map(&map);
    
    process_antennas(&map, &mut antenodes, &antenna_map);
    
    let (res_a, res_b) = count_results(&antenodes);
    println!("{}", res_a);
    println!("{}", res_b);
}