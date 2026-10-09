// Front is most recent; back is the eviction candidate. Initial values are distinct.
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <unordered_map>
using namespace std;

int main() {
    int n, m;
    while (cin >> n >> m) {
        list<int> recency;
        unordered_map<int, list<int>::iterator> location;
        location.reserve(n * 2);

        for (int i = 0; i < n; i++) {
            int num;
            cin >> num;

            recency.push_back(num);
            location[num] = prev(recency.end());
        }
        string hits;
        for (int i = 0; i < m; i++) {
            int value;
            cin >> value;
            auto f = location.find(value);
            if (f != location.end()) {
                recency.erase(f->second);
                recency.push_front(value);
                location[value] = recency.begin();
                hits += '1';
            } else {
                int last = recency.back();
                recency.pop_back();
                location.erase(last);
                recency.push_front(value);
                location[value] = recency.begin();
                hits += '0';
            }
        }
        cout << hits << '\n';

        int i = 0;
        for (auto x : recency) {
            if (i > 0) {
                cout << ' ';
            }
            cout << x;
            i++;
        }
        cout << '\n';
    }

    return 0;
}
