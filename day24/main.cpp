#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <regex>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <set>
using namespace std;
map<string, string> swapped_operands;

unordered_map<string, int> read_initial_values() {
    unordered_map<string, int> initial_gates;
    string line;
    regex re(R"(([xy]\d\d):\s(\d))");
    smatch match;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        if (regex_match(line, match, re)) {
            initial_gates[match[1].str()] = stoi(match[2].str());
        }
    }
    return initial_gates;
}

unordered_map<string, string> read_operations() {
    unordered_map<string, string> operations;
    string line;
    // This regex matches lines representing operations in the form:
    //    "<operand1> <OPERATION> <operand2> -> <result>"
    // where <operand1> and <operand2> are word characters (letters, digits, underscores),
    // <OPERATION> is one of "AND", "OR", or "XOR",
    // and <result> is a single word (the destination of the operation).
    //
    // Regex breakdown:
    // (\w+\s[AND|OR|XOR]\s\w+)  -- matches the operation portion (e.g., "a AND b"), captured as group 1
    // \s->\s                    -- matches the arrow surrounded by spaces
    // (\w+)                     -- captures the result wire/variable as group 2
    regex re(R"((\w+\s(?:AND|OR|XOR)\s\w+)\s->\s(\w+))");
    smatch match;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        //std::cout << line << std::endl;
        if (regex_match(line, match, re)) {
            operations[match[2].str()] = match[1].str();
        }
    }
    return operations;
}

int evaluate_operation(string operation, unordered_map<string, int> const &wires) {
    istringstream iss(operation);
    vector<string> operands;
    string operand;
    while (iss >> operand) {
        operands.push_back(operand);
    }
    int a = wires.at(operands.at(0));
    int b = wires.at(operands.at(2));
    if (operands.at(1) == "AND") {
        return a & b;
    } else if (operands.at(1) == "OR") {
        return a | b;
    } else if (operands.at(1) == "XOR") {
        return a ^ b;
    } else {
        assert(false);
    }
}

using graph = unordered_map<string, vector<string>>;
void dfs(string vertex, graph const &dependencies, unordered_set<string> &visited, stack<string> &rev_post_order) {
    visited.insert(vertex);
    if (dependencies.find(vertex) == dependencies.end()) {
        rev_post_order.push(vertex);
        return;
    }
    for (auto neighbor : dependencies.at(vertex)) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, dependencies, visited, rev_post_order);
        }
    }
    rev_post_order.push(vertex);
}

vector<string> topological_sort(graph const &dependencies, unordered_map<string, int> const &wires) {
    vector<string> top_sort;
    stack<string> rev_post_order;
    unordered_set<string> visited;
    for (auto [key, value] : wires) {
        dfs(key, dependencies, visited, rev_post_order);
    }
    while (!rev_post_order.empty()) {
        top_sort.push_back(rev_post_order.top());
        rev_post_order.pop();
    }
    return top_sort;
}

graph read_dependencies(unordered_map<string, string> const &operations) {
    unordered_map<string, vector<string>> dependencies;
    for (auto [wire, operation] : operations) {
        istringstream iss(operation);
        vector<string> operands;
        string operand;
        while (iss >> operand) {
            operands.push_back(operand);
        }
        dependencies[operands.at(0)].push_back(wire);
        dependencies[operands.at(2)].push_back(wire);
    }
    return dependencies;
}

uint64_t evaluate_operations(graph const &dependencies, unordered_map<string, int> wires, unordered_map<string, string> const &operations) {
    vector<string> const top_sort = topological_sort(dependencies, wires);

    for (string const &vertex : top_sort) {
        if (wires.find(vertex) != wires.end()) {
            continue;
        }
        int value = evaluate_operation(operations.at(vertex), wires);
        wires[vertex] = value;
    }
    uint64_t res_a = 0;
    for (auto &[key, value] : wires) {
        if (key.at(0) == 'z') {
            if (value != 0) {
                uint32_t idx = stoi(key.substr(1, 2));
                res_a |= (1ull << idx);
            }
        }
    }
    return res_a;
}

string get_out_wire(string op1, string op2, string operation, unordered_map<string, string> const &expression_map) {
    op1 = swapped_operands.find(op1) != swapped_operands.end() ? swapped_operands.at(op1) : op1;
    op2 = swapped_operands.find(op2) != swapped_operands.end() ? swapped_operands.at(op2) : op2;
    string op1_op2 = op1 + " " + operation + " " + op2;
    string op2_op1 = op2 + " " + operation + " " + op1;
    //cout << op1_op2 << " " << op2_op1 << endl;
    if (expression_map.find(op1_op2) != expression_map.end()) {
        return expression_map.at(op1_op2);
    }
    return expression_map.find(op2_op1) != expression_map.end() ? expression_map.at(op2_op1) : "";
}

bool is_expression_valid(string op1, string op2, string operation, unordered_map<string, string> const &expression_map) {
    string op1_op2 = op1 + " " + operation + " " + op2;
    string op2_op1 = op2 + " " + operation + " " + op1;
    
    return expression_map.find(op1_op2) != expression_map.end() || expression_map.find(op2_op1) != expression_map.end();
}

pair<string, string> get_mismatch_operands(string op1, string op2, string operation, unordered_map<string, string> const &expression_map) {
    for (auto [key, value] : expression_map) {
        string op1_as_prefix = op1 + " " + operation + " ";
        string op1_as_suffix = " " + operation + " " + op1;
        string op2_as_prefix = op2 + " " + operation + " ";
        string op2_as_suffix = " " + operation + " " + op2;
        if (key.find(op1_as_prefix) != string::npos) {
            return make_pair(key.substr(op1_as_prefix.length()), op2);
        }
        if (key.find(op1_as_suffix) != string::npos) {
            return make_pair(key.substr(0, 3), op2);
        }
        if (key.find(op2_as_prefix) != string::npos) {
            return make_pair(op1, key.substr(op2_as_prefix.length()));
        }
        if (key.find(op2_as_suffix) != string::npos) {
            return make_pair(op1, key.substr(0, 3));
        }
    }
    assert(false);
    return make_pair("", "");
}

// Helper function to record a swap between two wires
void record_swap(const string& wire1, const string& wire2) {
    swapped_operands[wire1] = wire2;
    swapped_operands[wire2] = wire1;
}

// Check if a wire exists for the given operation, and record mismatch if not
string check_wire_or_record_mismatch(const string& op1, const string& op2, 
                                      const string& operation, 
                                      const unordered_map<string, string>& expression_map) {
    string wire = get_out_wire(op1, op2, operation, expression_map);
    if (wire == "") {
        pair<string, string> mismatch = get_mismatch_operands(op1, op2, operation, expression_map);
        record_swap(mismatch.first, mismatch.second);
    }
    return wire;
}

// Verify and fix a single bit position in the adder
string verify_adder_bit(uint32_t bit_index, const string& carry_in, 
                        const unordered_map<string, string>& expression_map) {
    string op_num = bit_index < 10 ? "0" + to_string(bit_index) : to_string(bit_index);
    string op1 = "x" + op_num;
    string op2 = "y" + op_num;
    string expected_output = "z" + op_num;
    
    // Check sum = x[i] XOR y[i]
    string sum_output = get_out_wire(op1, op2, "XOR", expression_map);
    
    // Check result = sum XOR carry_in
    string output_result = get_out_wire(sum_output, carry_in, "XOR", expression_map);
    if (output_result == "") {
        pair<string, string> mismatch = get_mismatch_operands(sum_output, carry_in, "XOR", expression_map);
        record_swap(mismatch.first, mismatch.second);
    } else if (output_result != expected_output) {
        record_swap(expected_output, output_result);
    }
    
    // Check carry_out_op1 = x[i] AND y[i]
    string carry_out_op1 = check_wire_or_record_mismatch(op1, op2, "AND", expression_map);
    
    // Check carry_out_op2 = carry_in AND sum
    string carry_out_op2 = check_wire_or_record_mismatch(carry_in, sum_output, "AND", expression_map);
    
    // Check carry_out = carry_out_op1 OR carry_out_op2
    string carry_out = check_wire_or_record_mismatch(carry_out_op1, carry_out_op2, "OR", expression_map);
    
    return carry_out;
}

// Print deduplicated and sorted swapped wires
void print_swapped_wires() {
    bool first = true;
    for (auto [key, value] : swapped_operands) {
        if (!first) cout << ",";
        cout << key;
        first = false;
    }
    cout << endl;
}

int main() {
    unordered_map<string, int> wires = read_initial_values();
    unordered_map<string, string> operations = read_operations();
    graph const dependencies = read_dependencies(operations);
    
    cout << evaluate_operations(dependencies, wires, operations) << endl;

    unordered_map<string, string> expression_map;
    for (auto& [wire, operation] : operations) {
        expression_map[operation] = wire;
    }
    
    // Verify ripple-carry adder structure and find swapped wires
    uint32_t num_res_bits = wires.size() / 2 + 1;
    
    // Initial carry from x00 AND y00
    string carry_in = get_out_wire("x00", "y00", "AND", expression_map);
    
    // Verify each bit position
    for (uint32_t i = 1; i < num_res_bits - 1; i++) {
        carry_in = verify_adder_bit(i, carry_in, expression_map);
    }

    print_swapped_wires();
    return 0;
}