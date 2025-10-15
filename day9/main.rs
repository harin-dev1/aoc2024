use std::io;

fn get_file_compactness(file_id: u32, file_size_in_blocks: u32, blk_offset: u32) -> u64 {
    // Sum of (blk_offset + k) * file_id for k = 0..file_size_in_blocks
    // = file_id * sum(blk_offset + k) for k = 0..n-1
    // = file_id * (n * blk_offset + n*(n-1)/2)
    if file_size_in_blocks == 0 {
        return 0;
    }
    let n = file_size_in_blocks as u64;
    let offset = blk_offset as u64;
    let id = file_id as u64;
    id * (n * offset + n * (n - 1) / 2)
}

fn get_disk_compactness_b(disk_map: &str) -> u64 {
    let chars: Vec<char> = disk_map.chars().collect();
    let mut res = 0u64;
    let mut j = chars.len() - 1;
    let mut chars_copy = chars.clone();
    let mut blk_offsets = vec![0; chars.len()];
    for i in 1..chars.len() {
        blk_offsets[i] = blk_offsets[i - 1] + (chars[i - 1].to_digit(10).unwrap() as u32);
    }
    while j > 0 {
        let file_size_in_blocks = chars_copy[j].to_digit(10).unwrap() as u32;
        let file_id = j as u32 / 2;
        for i in 1..j {
            if i % 2 == 0 {
                continue;
            }
            let free_space_in_blocks = chars_copy[i].to_digit(10).unwrap() as u32;
            if free_space_in_blocks >= file_size_in_blocks {
                res += get_file_compactness(file_id, file_size_in_blocks, blk_offsets[i]);
                blk_offsets[i] += file_size_in_blocks;
                chars_copy[i] = (free_space_in_blocks - file_size_in_blocks).to_string().chars().next().unwrap();
                chars_copy[j] = '0';
                break;
            }
        }
        j -= 2;
    }
    for i in 0..chars_copy.len() {
        if i % 2 == 0 {
            let file_size_in_blocks = chars_copy[i].to_digit(10).unwrap() as u32;
            let file_id = i as u32 / 2;
            res += get_file_compactness(file_id, file_size_in_blocks, blk_offsets[i]);
        }
    }
    res
}


fn get_disk_compactness_a(disk_map: &str) -> u64 {
    let chars: Vec<char> = disk_map.chars().collect();
    let mut res = 0u64;
    let mut i = 0;
    let mut j = chars.len() - 1;
    let mut blk_offset = 0;
    let mut remaining_file_blocks = 0u32;
    let mut remaining_free_blocks = 0u32;

    while i < j {
        if i % 2 == 0 {
            let file_size_in_blocks = chars[i].to_digit(10).unwrap();
            let file_id = i as u32 / 2;
            res += get_file_compactness(file_id, file_size_in_blocks, blk_offset);
            blk_offset += file_size_in_blocks;
            i += 1;
        } else {
            if remaining_file_blocks == 0 {
                remaining_file_blocks = chars[j].to_digit(10).unwrap();
            }
            
            if remaining_free_blocks == 0 {
                remaining_free_blocks = chars[i].to_digit(10).unwrap();
            }
            
            let file_id = j as u32 / 2;
            let num_blocks = std::cmp::min(remaining_file_blocks, remaining_free_blocks);
            res += get_file_compactness(file_id, num_blocks, blk_offset);
            blk_offset += num_blocks;
            
            remaining_file_blocks -= num_blocks;
            remaining_free_blocks -= num_blocks;
            
            if remaining_file_blocks == 0 {
                j -= 2;
            }
            if remaining_free_blocks == 0 {
                i += 1;
            }
        }
    }

    if remaining_file_blocks != 0 {
        let file_id = j as u32 / 2;
        res += get_file_compactness(file_id, remaining_file_blocks, blk_offset);
    }
    res
}

fn main() {
    let mut disk_map = String::new();
    io::stdin().read_line(&mut disk_map).unwrap();
    let disk_map = disk_map.trim();

    let res_a = get_disk_compactness_a(&disk_map);
    let res_b = get_disk_compactness_b(&disk_map);

    println!("{}", res_a);
    println!("{}", res_b);
}