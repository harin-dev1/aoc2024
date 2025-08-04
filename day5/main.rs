use std::io::{self, BufRead};
use regex::Regex;
use std::collections::{HashSet, HashMap};

struct Graph {
    graph : HashMap<u32, Vec<u32>>,
    vertices : HashSet<u32>,
}

impl Graph {
    fn new(rules : &HashSet<(u32, u32)>, vertices : &Vec<u32>) -> Self {
        let mut graph = HashMap::new(); 

        let vertices_set : HashSet<u32> = vertices.iter().cloned().collect();
        for &(from, to) in rules {
            if vertices_set.contains(&from) && vertices_set.contains(&to) {
                graph.entry(from).or_insert_with(Vec::new).push(to);
            }
        }

        Graph{
            graph : graph,
            vertices : vertices_set,
        }
    }

    fn find_topo_order(&self) -> u32 {
        let mut visited = HashSet::new();
        let mut topo_order= Vec::new();

        for &vertex in &self.vertices {
            if !visited.contains(&vertex) {
                self.dfs(vertex, &mut visited, &mut topo_order);
            }
        }
        topo_order.reverse();
        topo_order[topo_order.len() / 2]
    }

    fn dfs(&self, vertex : u32, visited : &mut HashSet<u32>, topo_order : &mut Vec<u32>) {
        visited.insert(vertex);
        if let Some(neighbors) = self.graph.get(&vertex) {
            for &neighbor in neighbors {
                if !visited.contains(&neighbor) {
                    self.dfs(neighbor, visited, topo_order);
                }
            }
        }
        topo_order.push(vertex);
    }
}

fn main() {
    let mut res_a = 0;
    let mut res_b = 0;
    let mut rules = HashSet::<(u32, u32)>::new();
    let reg_rules = Regex::new(r"^(\d+)\|(\d+)$").unwrap();
    
    // Read rules
    for line in io::stdin().lock().lines() {
        let line = line.unwrap();
        if line.is_empty() {
            break;
        }
        let caps = reg_rules.captures(&line).unwrap();
        let a: u32 = caps[1].parse().unwrap();
        let b: u32 = caps[2].parse().unwrap();
        rules.insert((a, b));
    }
    
    // Process sequences
    for line in io::stdin().lock().lines() {
        let line = line.unwrap();
        let order: Vec<u32> = line.split(',')
            .map(|x| x.parse::<u32>().unwrap())
            .collect();
        
        let mut is_valid = true;
        for i in 0..order.len() {
            for j in (i + 1)..order.len() {
                if rules.contains(&(order[j], order[i])) {
                    is_valid = false;
                    break;
                }
            }
            if !is_valid {
                break;
            }
        }
        
        if is_valid {
            res_a += order[order.len() / 2];
        } else {
            let graph = Graph::new(&rules, &order);
            res_b += graph.find_topo_order();
        }
    }

    println!("{}", res_a);
    println!("{}", res_b);
}