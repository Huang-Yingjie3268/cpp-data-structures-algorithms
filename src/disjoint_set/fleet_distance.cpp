// Directional DSU: append x's fleet after y's fleet; offsets count ships to the root.
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

class FleetDisjointSet {
    vector<int> parent;
    vector<int> distance_to_root;
    vector<int> fleet_size;

  public:
    explicit FleetDisjointSet(int n)
        : parent(n + 1), distance_to_root(n + 1, 0), fleet_size(n + 1, 1) {
        for (int i = 0; i <= n; ++i)
            parent[i] = i;
    }

    int find_root(int x) {
        int root = x;
        int total_distance = 0;
        while (parent[root] != root) {
            total_distance += distance_to_root[root];
            root = parent[root];
        }
        // Same weighted path compression as the recursive coursework version.
        // Two passes avoid call-stack overflow on a chain of 30000 ships.
        while (parent[x] != x) {
            const int next = parent[x];
            const int edge_distance = distance_to_root[x];
            parent[x] = root;
            distance_to_root[x] = total_distance;
            total_distance -= edge_distance;
            x = next;
        }
        return root;
    }

    void append_fleet(int x, int y) {
        const int root_x = find_root(x);
        const int root_y = find_root(y);
        if (root_x == root_y)
            return;
        distance_to_root[root_x] = fleet_size[root_y];
        fleet_size[root_y] += fleet_size[root_x];
        parent[root_x] = root_y;
    }

    int ships_between(int x, int y) {
        const int root_x = find_root(x);
        const int root_y = find_root(y);
        if (root_x != root_y)
            return -1;
        if (x == y)
            return 0;
        return abs(distance_to_root[x] - distance_to_root[y]) - 1;
    }
};

int main() {
    FleetDisjointSet fleets(30000);
    int query_count;
    if (!(cin >> query_count))
        return 0;
    for (int q = 0; q < query_count; ++q) {
        char command;
        int x, y;
        cin >> command >> x >> y;
        if (command == 'M')
            fleets.append_fleet(x, y);
        else if (command == 'C')
            cout << fleets.ships_between(x, y) << '\n';
    }
}
