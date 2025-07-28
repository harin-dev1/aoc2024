#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

uint32_t get_count_a(std::vector<std::string> const &input, uint32_t i, uint32_t j, std::string const &search_string) {
    uint32_t count = 0;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) {
                continue;
            }

            uint32_t k = 0;
            for (; k < search_string.size(); k++) {
                if (i + di * k < 0 || i + di * k >= input.size() || j + dj * k < 0 || j + dj * k >= input[i + di * k].size()) {
                    break;
                }

                if (input[i + di * k][j + dj * k] != search_string[k]) {
                    break;
                }
            }

            if (k == search_string.size()) {
                count++;
            }
        }
    }

    return count;
}

bool is_valid_b(std::vector<std::string> const &input, uint32_t i, uint32_t j) {
    if (input[i][j] != 'A') {
        return false;
    }

    if (i < 1 || i >= input.size() - 1 || j < 1 || j >= input[i].size() - 1) {
        return false;
    }

    std::string a;
    a += input[i - 1][j - 1];
    a += input[i + 1][j + 1];
    std::string b;
    b += input[i - 1][j + 1];
    b += input[i + 1][j - 1];  

    return (a == "MS" || a == "SM") && (b == "MS" || b == "SM");
}

int main() {
    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }

    std::string search_string = "XMAS";
    uint32_t res_a = 0;
    uint32_t res_b = 0;
    for (uint32_t i = 0; i < input.size(); i++) {
        for (uint32_t j = 0; j < input[i].size(); j++) {
            res_a += get_count_a(input, i, j, search_string);
            if (is_valid_b(input, i, j)) {
                //std::cout << "B: " << i << " " << j << std::endl;
                res_b++;
            }
        }
    }

    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;
    return 0;
}