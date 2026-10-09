// Sieve once; BFS visits only four-digit prime states.
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

vector<bool> prime(10000, true);

void build_prime_sieve() {
    prime[0] = false;
    prime[1] = false;

    for (int i = 2; i * i < 10000; i++) {
        if (prime[i]) {
            for (int j = i * i; j < 10000; j = j + i) {
                prime[j] = false;
            }
        }
    }
}

int shortest_prime_path(int start, int end) {
    if (start < 1000 || start > 9999 || end < 1000 || end > 9999 || !prime[start] || !prime[end])
        return -1;
    queue<pair<int, int>> q;
    vector<bool> visited(10000, false);

    q.push({start, 0});
    visited[start] = true;
    while (!q.empty()) {
        int value = q.front().first;
        int distance = q.front().second;
        q.pop();

        if (value == end) {
            return distance;
        }

        string digits = to_string(value);
        for (int i = 0; i < 4; i++) {
            char original_digit = digits[i];
            for (char j = '0'; j <= '9'; j++) {
                if (j == original_digit) {
                    continue;
                }
                if (i == 0 && j == '0') {
                    continue;
                }
                digits[i] = j;
                int next = stoi(digits);
                if (prime[next] && !visited[next]) {
                    visited[next] = true;
                    q.push({next, distance + 1});
                }
            }
            digits[i] = original_digit;
        }
    }
    return -1;
}
int main() {
    build_prime_sieve();
    int cases;
    if (!(cin >> cases))
        return 0;
    for (int i = 0; i < cases; i++) {
        int start, end;
        if (!(cin >> start >> end))
            return 1;

        int result = shortest_prime_path(start, end);
        if (result == -1) {
            cout << "Impossible" << '\n';
        } else {
            cout << result << '\n';
        }
    }
    return 0;
}
