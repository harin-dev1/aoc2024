#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

bool can_evaluate_to(std::vector<uint64_t> const &nums, uint64_t target,
    uint64_t current, uint64_t index, bool can_use_concat) {
    if (index == nums.size()) {
        return current == target;
    }

    if (current > target) {
        return false;
    }

    return can_evaluate_to(nums, target, current + nums[index], index + 1, can_use_concat) || 
    can_evaluate_to(nums, target, current * nums[index], index + 1, can_use_concat) ||
    (can_use_concat && can_evaluate_to(nums, target,
        std::stoull(std::to_string(current) + std::to_string(nums[index])), 
        index + 1, can_use_concat));
    
}
int main() {
    std::regex re(R"(^(\d+):\s(.+)$)");
    uint64_t res_a = 0;
    uint64_t res_b = 0;

    std::string line;
    while (std::getline(std::cin, line)) {
        uint64_t test_num;
        std::vector<uint64_t> nums;
        std::smatch match;
        if (std::regex_match(line, match, re)) {
            test_num = std::stoull(match[1]);
            std::stringstream ss(match[2]);
            std::string num;
            while (std::getline(ss, num, ' ')) {
                nums.push_back(std::stoull(num));
            }
            if (can_evaluate_to(nums, test_num, nums[0], 1, false)) {
                res_a += test_num;
            } else if (can_evaluate_to(nums, test_num, nums[0], 1, true)) {
                res_b += test_num;
            }
        }
    }

    std::cout << res_a << std::endl;
    std::cout << res_a + res_b << std::endl;

    return 0;
}

