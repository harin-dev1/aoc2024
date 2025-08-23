#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

enum class Antenna {
    None,
    A,
    B
};

using Grid = std::vector<std::string>;
using AntennaGrid = std::vector<std::vector<Antenna>>;
using AntennaMap = std::unordered_map<char, std::vector<std::pair<int, int>>>;
using Position = std::pair<int, int>;

// Read input grid from stdin
Grid readGrid() {
    Grid map;
    std::string line;
    while (std::getline(std::cin, line)) {
        map.push_back(line);
    }
    return map;
}

// Build map of antenna positions by frequency
AntennaMap buildAntennaMap(const Grid& map) {
    AntennaMap antenna_map;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] != '.') {
                antenna_map[map[i][j]].push_back({i, j});
            }
        }
    }
    return antenna_map;
}

// Check if position is within grid bounds
bool isInBounds(const Grid& map, int x, int y) {
    return x >= 0 && x < map.size() && y >= 0 && y < map[0].size();
}

// Set antenna node if position is empty
void setAntennaNode(AntennaGrid& antenodes, int x, int y, Antenna type) {
    if (type == Antenna::A || antenodes[x][y] == Antenna::None) {
        antenodes[x][y] = type;
    }
}

// Process antenna pairs to find antinodes
void processAntennaPair(const Grid& map, AntennaGrid& antenodes, 
                       const Position& pos1, const Position& pos2) {
    int x1 = pos1.first, y1 = pos1.second;
    int x2 = pos2.first, y2 = pos2.second;
    
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Part B: Mark all positions along the line (including antenna positions)
    int nx1 = x1, ny1 = y1;
    int nx2 = x2, ny2 = y2;

    // Extend backwards from first antenna
    while (isInBounds(map, nx1, ny1)) {
        setAntennaNode(antenodes, nx1, ny1, Antenna::B);
        nx1 -= dx;
        ny1 -= dy;
    }

    // Extend forwards from second antenna
    while (isInBounds(map, nx2, ny2)) {
        setAntennaNode(antenodes, nx2, ny2, Antenna::B);
        nx2 += dx;
        ny2 += dy;
    }

    // Part A: Mark specific antinode positions
    nx1 = x1 - dx;
    ny1 = y1 - dy;
    nx2 = x2 + dx;
    ny2 = y2 + dy;

    if (isInBounds(map, nx1, ny1)) {
        antenodes[nx1][ny1] = Antenna::A;
    }
    if (isInBounds(map, nx2, ny2)) {
        antenodes[nx2][ny2] = Antenna::A;
    }
}

// Process all antennas to find antinodes
void processAntennas(const Grid& map, AntennaGrid& antenodes, const AntennaMap& antenna_map) {
    for (const auto& [frequency, nodes] : antenna_map) {
        if (nodes.size() <= 1) {
            continue;
        }

        // Process all pairs of antennas with same frequency
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = i + 1; j < nodes.size(); j++) {
                processAntennaPair(map, antenodes, nodes[i], nodes[j]);
            }
        }
    }
}

// Count results for both parts
std::pair<int, int> countResults(const AntennaGrid& antenodes) {
    int res_a = 0, res_b = 0;
    for (int i = 0; i < antenodes.size(); i++) {
        for (int j = 0; j < antenodes[i].size(); j++) {
            if (antenodes[i][j] == Antenna::A) {
                res_a++;
                res_b++;
            } else if (antenodes[i][j] == Antenna::B) {
                res_b++;
            }
        }
    }
    return {res_a, res_b};
}

int main() {
    Grid map = readGrid();
    if (map.empty()) return 0;

    AntennaGrid antenodes(map.size(), std::vector<Antenna>(map[0].size(), Antenna::None));
    AntennaMap antenna_map = buildAntennaMap(map);
    
    processAntennas(map, antenodes, antenna_map);
    
    auto [res_a, res_b] = countResults(antenodes);
    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;

    return 0;
}