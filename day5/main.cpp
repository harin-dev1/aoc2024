#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <set>

using namespace std;

class Graph {
    public:
    Graph(set<pair<int, int>> rules, vector<int> &vertices) : m_vertices(vertices.begin(), vertices.end()) {
        for (auto rule : rules) {
            if (m_vertices.find(rule.first) != m_vertices.end() && m_vertices.find(rule.second) != m_vertices.end()) {
                m_graph[rule.first].push_back(rule.second);
            }
        }
        for (int vertex : vertices) {
            m_visited[vertex] = false;
        }
        find_topo_order();
        vector<int> topo_order;
        while (!m_topo_order.empty()) {
            topo_order.push_back(m_topo_order.top());
            m_topo_order.pop();
        }
        m_middle_vertex = topo_order[topo_order.size() / 2];
    }
    int get_middle_vertex() {
        return m_middle_vertex;
    }
    private:
    void find_topo_order() {
        for (int vertex : m_vertices) {
            if (!m_visited[vertex]) {
                dfs(vertex);
            }
        }   
    }
    void dfs(int vertex) {
        m_visited[vertex] = true;
        for (int neighbor : m_graph[vertex]) {
            if (!m_visited[neighbor]) {
                dfs(neighbor);
            }
        }
        m_topo_order.push(vertex);
    }
    unordered_map<int, vector<int>> m_graph;
    unordered_set<int> m_vertices;
    unordered_map<int, bool> m_visited;
    stack<int> m_topo_order;
    int m_middle_vertex;
};

int main() {
    set<pair<int, int>> rules;
    string line;
    uint64_t res_a = 0;
    uint64_t res_b = 0;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        regex re(R"((\d+)\|(\d+))");
        smatch match;
        if (regex_match(line, match, re)) {
            rules.insert(make_pair(stoi(match[1].str()), stoi(match[2].str())));
        }
    }
    while (getline(cin, line)) {
        stringstream ss(line);
        int a;
        vector<int> order;
        while (ss >> a) {
            order.push_back(a);
            ss.ignore(1);
        }
        bool valid = true;
        for (int i = 0; i < order.size(); i++) {
            for (int j = i + 1; j < order.size(); j++) {
                if (rules.find(make_pair(order[j], order[i])) != rules.end()) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            res_a += order[order.size() / 2];
        } else {
            Graph graph(rules, order);
            res_b += graph.get_middle_vertex();
        }
    }
    cout << res_a << endl;
    cout << res_b << endl;
}