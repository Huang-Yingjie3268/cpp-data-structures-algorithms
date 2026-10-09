// Enumerate column subsets; for each subset choose each row orientation independently.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void flip_column(vector<string> &s, int row, int col) {
    for (int i = 0; i < row; i++) {
        if (s[i][col] == '0') {
            s[i][col] = '1';
        } else {
            s[i][col] = '0';
        }
    }
}

int main() {
    int n, m;
    while (cin >> n >> m) {

        vector<string> grid(n);

        for (int i = 0; i < n; i++) {
            cin >> grid[i];
        }

        int best_heads = 0;
        for (int i = 0; i < (1 << m); i++) {
            int total = 0;

            for (int j = 0; j < m; j++) {
                if (i & (1 << j)) {
                    flip_column(grid, n, j);
                }
            }

            for (int p = 0; p < n; p++) {
                int heads = 0;
                for (int q = 0; q < m; q++) {
                    if (grid[p][q] == '1') {
                        heads++;
                    }
                }
                if (heads < m - heads) {
                    heads = m - heads;
                }
                total += heads;
            }

            if (total > best_heads) {
                best_heads = total;
            }

            for (int j = 0; j < m; j++) {
                if (i & (1 << j)) {
                    flip_column(grid, n, j);
                }
            }
        }

        cout << best_heads << '\n';
    }
    return 0;
}
