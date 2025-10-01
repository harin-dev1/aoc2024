#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

uint64_t get_file_compactness(uint32_t file_id, uint32_t file_size_in_blocks, uint32_t blk_offset) {
    uint64_t res = 0;
    while (file_size_in_blocks > 0) {
        res += blk_offset * file_id;
        blk_offset++;
        file_size_in_blocks--;
    }
    return res;
}

uint64_t get_disk_compactness_b(std::string disk_map) {
    uint64_t res = 0;
    uint32_t i = 0;
    uint32_t blk_offset = 0;
    uint32_t j = disk_map.size() - 1;
    std::string disk_map_copy = disk_map;
    std::vector<uint32_t> blk_offsets(disk_map.size(), 0);
    for (uint32_t i = 1; i < disk_map.size(); i++) {
        blk_offsets[i] = blk_offsets[i - 1] + (static_cast<uint32_t>(disk_map[i - 1]) - '0');
    }
    while (j > 0) {
        uint32_t file_size_in_blocks = static_cast<uint32_t>(disk_map_copy[j]) - '0';
        uint32_t file_id = j / 2;
        for (uint32_t i = 1; i < j; i += 2) {
            uint32_t free_space_in_blocks = static_cast<uint32_t>(disk_map_copy[i]) - '0';
            if (free_space_in_blocks >= file_size_in_blocks) {
                res += get_file_compactness(file_id, file_size_in_blocks, blk_offsets[i]);
                blk_offsets[i] += file_size_in_blocks;
                disk_map_copy[i] = static_cast<char>('0' + (free_space_in_blocks - file_size_in_blocks));
                disk_map_copy[j] = '0';
                break;
            }
        }
        j -= 2;
    }
    for (uint32_t i = 0; i < disk_map.size(); i += 2) {
        uint32_t file_size_in_blocks = static_cast<uint32_t>(disk_map_copy[i]) - '0';
        uint32_t file_id = i / 2;
        res += get_file_compactness(file_id, file_size_in_blocks, blk_offsets[i]);
    }
    return res;
}

uint64_t get_disk_compactness_a(std::string disk_map) {
    uint64_t res = 0;
    uint32_t i = 0;
    uint32_t j = disk_map.size() - 1;
    uint32_t blk_offset = 0;
    while (i <= j) {
        if (i % 2 == 0) {
            uint32_t file_size_in_blocks = static_cast<uint32_t>(disk_map[i]) - '0';
            uint32_t file_id = i / 2;
            res += get_file_compactness(file_id, file_size_in_blocks, blk_offset);
            blk_offset += file_size_in_blocks;
            ++i;
        } else {
            uint32_t file_size_in_blocks = static_cast<uint32_t>(disk_map[j]) - '0';
            uint32_t file_id = j / 2;
            uint32_t free_space_in_blocks = static_cast<uint32_t>(disk_map[i]) - '0';
            uint32_t num_blocks = std::min(file_size_in_blocks, free_space_in_blocks);
            res += get_file_compactness(file_id, num_blocks, blk_offset);
            blk_offset += num_blocks;
            if (free_space_in_blocks == file_size_in_blocks) {
                j -= 2;
                ++i;
            } else if (free_space_in_blocks < file_size_in_blocks) {
                i++;
                disk_map[j] = static_cast<char>('0' + (file_size_in_blocks - num_blocks));
            } else {
                j -= 2;
                disk_map[i] = static_cast<char>('0' + (free_space_in_blocks - num_blocks));
            }
        }
    }

    return res;
}

int main() {
    std::string disk_map;
    std::cin >> disk_map;

    uint64_t res_a = get_disk_compactness_a(disk_map);
    uint64_t res_b = get_disk_compactness_b(disk_map);

    std::cout << res_a << '\n';
    std::cout << res_b << '\n';
    return 0;
}