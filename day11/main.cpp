#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <cstdint>
using namespace std;

// Memoization cache
unordered_map<string, unordered_map<uint32_t, uint64_t>> memo;

uint64_t dfs(string const &value, uint32_t blink_count) {
    // Check if result is already cached
    if (memo.count(value) && memo[value].count(blink_count)) {
        return memo[value][blink_count];
    }
    
    uint64_t result;
    
    if (blink_count == 0) {
        result = 1;
    } else if (value == "0") {
        result = dfs("1", blink_count - 1);
    } else if (value.size() % 2 == 0) {
        string first_half = value.substr(0, value.size() / 2);
        string second_half = value.substr(value.size() / 2);

        // Trim leading zeros from first_half
        size_t pos1 = first_half.find_first_not_of('0');
        if (pos1 != string::npos) {
            first_half = first_half.substr(pos1);
        } else {
            first_half = "0";
        }

        // Trim leading zeros from second_half
        size_t pos2 = second_half.find_first_not_of('0');
        if (pos2 != string::npos) {
            second_half = second_half.substr(pos2);
        } else {
            second_half = "0";
        }
        
        result = dfs(first_half, blink_count - 1) + dfs(second_half, blink_count - 1);
    } else {
        uint64_t val = stoull(value);
        val *= 2024;
        result = dfs(to_string(val), blink_count - 1);
    }
    
    // Cache the result before returning
    memo[value][blink_count] = result;
    return result;
}

int main() {
    string line;
    vector<string> input;
    getline(cin, line);
    istringstream iss(line);
    input = vector<string>{istream_iterator<string>(iss), istream_iterator<string>()};
    uint64_t res_a = 0;
    uint64_t res_b = 0;
    for (auto& word : input) {
        res_a += dfs(word, 25);
        res_b += dfs(word, 75);
    }
    cout << res_a << endl;
    cout << res_b << endl;
    return 0;
}