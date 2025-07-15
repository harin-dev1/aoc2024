#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main() {
    regex re_a(R"(mul\((\d+),(\d+)\))");
    regex re_b(R"(do\(\)|don't\(\)|mul\((\d+),(\d+)\))");

    string s;
    uint64_t res_a = 0;
    uint64_t res_b = 0;
    bool is_enabled = true;
    while (getline(cin, s)) {
        for (auto it = sregex_iterator(s.begin(), s.end(), re_a); it != sregex_iterator(); it++) {
            auto match = *it;
            int a = stoi(match[1].str());
            int b = stoi(match[2].str());
            res_a += a * b;
        }
        for (auto it = sregex_iterator(s.begin(), s.end(), re_b); it != sregex_iterator(); it++) {
            auto match = *it;
            if (match[0].str() == "do()") {
                is_enabled = true;
            } else if (match[0].str() == "don't()") {
                is_enabled = false;
            } else if (is_enabled) {
                int a = stoi(match[1].str());
                int b = stoi(match[2].str());
                res_b += a * b;
            }
        }
    }
    cout << res_a << endl;
    cout << res_b << endl;
}