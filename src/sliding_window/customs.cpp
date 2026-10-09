// FIFO expiration and hash frequencies maintain the open 24-hour time window.
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
using namespace std;

int main() {
    int record_count;
    if (!(cin >> record_count))
        return 0;
    queue<pair<long long, int>> passengers;
    unordered_map<int, int> frequency;
    int distinct_nations = 0;
    for (int record = 0; record < record_count; ++record) {
        long long arrival;
        int passenger_count;
        cin >> arrival >> passenger_count;
        for (int j = 0; j < passenger_count; ++j) {
            int nation;
            cin >> nation;
            passengers.push({arrival, nation});
            if (++frequency[nation] == 1)
                ++distinct_nations;
        }
        // A timestamp exactly 86400 seconds old is outside the window.
        while (!passengers.empty() && arrival - passengers.front().first >= 86400) {
            const int nation = passengers.front().second;
            passengers.pop();
            if (--frequency[nation] == 0) {
                --distinct_nations;
                frequency.erase(nation);
            }
        }
        cout << distinct_nations << '\n';
    }
}
