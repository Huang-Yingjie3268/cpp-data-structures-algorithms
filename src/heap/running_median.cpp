// Every left value <= every right value; left has the same size or one more.
// Only odd-length prefix medians are printed, as in the original problem.
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int length;
    if (!(cin >> length))
        return 0;

    priority_queue<int> leftheap;
    priority_queue<int, vector<int>, greater<int>> rightheap;

    for (int i = 0; i < length; i++) {
        int newnum;
        cin >> newnum;

        if (leftheap.empty() || newnum <= leftheap.top()) {
            leftheap.push(newnum);
        }

        else {
            rightheap.push(newnum);
        }

        if (leftheap.size() < rightheap.size()) {
            const int change = rightheap.top();
            leftheap.push(change);
            rightheap.pop();
        } else if (leftheap.size() >= rightheap.size() + 2) {
            const int temp = leftheap.top();
            leftheap.pop();
            rightheap.push(temp);
        }

        if ((leftheap.size() + rightheap.size()) % 2 == 1) {
            cout << leftheap.top() << '\n';
        }
    }

    return 0;
}
