#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_set>

bool has_cycle(std::vector<std::string> map, std::pair<int, int> start) {
    std::vector<std::vector<int>> visited_dir(map.size(), std::vector<int>(map[0].size(), -1));
    std::pair<int, int> cur_pos = start;
    int cur_dir = 0;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    visited_dir[start.first][start.second] = 0;
    while (true) {
        std::pair<int, int> next_pos = {cur_pos.first + directions[cur_dir].first, cur_pos.second + directions[cur_dir].second};
        if (next_pos.first < 0 || next_pos.first >= map.size() || next_pos.second < 0 || next_pos.second >= map[0].size()) {
            return false;
        }
        if (map[next_pos.first][next_pos.second] == '#') {
            cur_dir = (cur_dir + 1) % 4;
        } else {
            if (visited_dir[next_pos.first][next_pos.second] == cur_dir) {
                return true;
            }
            visited_dir[next_pos.first][next_pos.second] = cur_dir;
            cur_pos = next_pos;
        }
    }
}

int main() {
    std::vector<std::string> map;
    std::string line;
    while (std::getline(std::cin, line)) {
        map.push_back(line);
    }
    std::pair<int, int> start;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '^') {
                start = {i, j};
                break;
            }
        }
    }
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
    visited[start.first][start.second] = true;
    uint32_t res_a = 1;
    std::pair<int, int> cur_pos = start;
    uint32_t cur_dir = 0;
    while (true) {
        std::pair<int, int> next_pos = {cur_pos.first + directions[cur_dir].first, cur_pos.second + directions[cur_dir].second};
        if (next_pos.first < 0 || next_pos.first >= map.size() || next_pos.second < 0 || next_pos.second >= map[0].size()) {
            break;
        }
        if (map[next_pos.first][next_pos.second] == '#') {
            cur_dir = (cur_dir + 1) % 4;
        } else {
            if (!visited[next_pos.first][next_pos.second]) {
                visited[next_pos.first][next_pos.second] = true;
                res_a++;
            }
            cur_pos = next_pos;
        }
    }
    uint32_t res_b = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '.') {
                map[i][j] = '#';
                if (has_cycle(map, start)) {
                    res_b++;
                }
                map[i][j] = '.';
            }
        }
    }
    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;
    return 0;
}