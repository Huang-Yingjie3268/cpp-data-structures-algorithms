// Uniform regions are leaves; split on demand and merge four equal leaf children.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Node {
    int color;       // color
    int node_count;  // active descendants + itself
    int children[4]; // index of children
};

int append_node(int color, vector<Node> &tree) {
    Node node;
    node.color = color;
    node.node_count = 1;

    for (int i = 0; i < 4; i++) {
        node.children[i] = -1; // leaf
    }

    tree.push_back(node);
    return static_cast<int>(tree.size()) - 1; // index
}

int is_uniform(int top, int left, const vector<vector<int>> &pixels, int length) {
    int check = pixels[top][left];
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (check == pixels[top + i][left + j]) {
                continue;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

int build_tree(const vector<vector<int>> &pixels, vector<Node> &tree, int len, int top, int left) {
    if (is_uniform(top, left, pixels, len)) {
        return append_node(pixels[top][left], tree);
    } else {
        int newlength = len / 2;
        int index = append_node(-1, tree);
        tree[index].children[0] = build_tree(pixels, tree, newlength, top, left);
        tree[index].children[1] = build_tree(pixels, tree, newlength, top, left + newlength);
        tree[index].children[2] = build_tree(pixels, tree, newlength, top + newlength, left);
        tree[index].children[3] =
            build_tree(pixels, tree, newlength, top + newlength, left + newlength);
        tree[index].node_count =
            tree[tree[index].children[0]].node_count + tree[tree[index].children[1]].node_count +
            tree[tree[index].children[2]].node_count + tree[tree[index].children[3]].node_count + 1;

        return index;
    }
}

void flip_pixel(
    vector<Node> &tree, int x, int y, int top, int left, int length,
    int node_index) { // xy is the position need to flip, ab is the position of the top left corner
    // leaf
    if (length == 1) {
        tree[node_index].color = 1 - tree[node_index].color; // only 0, 1
        tree[node_index].node_count = 1;
        return;
    }

    if (tree[node_index].color != -1) {
        int color = tree[node_index].color;
        tree[node_index].color = -1;
        for (int i = 0; i < 4; i++) {
            tree[node_index].children[i] = append_node(color, tree);
        }
    }

    int newlen = length / 2;
    int child;

    if (x < top + newlen) {
        if (y < left + newlen) {
            child = 0; // top left
        } else {
            child = 1; // top right
        }
    } else {
        if (y < left + newlen) {
            child = 2; // bottom left
        } else {
            child = 3; // bottom right
        }
    }

    int newa, newb;
    newa = top;
    newb = left;

    if (child == 1) {
        newb = left + newlen;
    } else if (child == 2) {
        newa = top + newlen;
    } else if (child == 3) {
        newa = top + newlen;
        newb = left + newlen;
    } else {
        newa = top;
        newb = left;
    }

    flip_pixel(tree, x, y, newa, newb, newlen, tree[node_index].children[child]);
    int cindex1 = tree[node_index].children[0];
    int cindex2 = tree[node_index].children[1];
    int cindex3 = tree[node_index].children[2];
    int cindex4 = tree[node_index].children[3];

    if (tree[cindex1].color != -1 && tree[cindex1].color == tree[cindex2].color &&
        tree[cindex2].color == tree[cindex3].color && tree[cindex3].color == tree[cindex4].color) {
        tree[node_index].color = tree[cindex1].color;
        tree[node_index].node_count = 1; // union
        for (int i = 0; i < 4; i++) {
            tree[node_index].children[i] = -1;
        }
    } else {
        tree[node_index].color = -1;
        tree[node_index].node_count = 1 + tree[cindex1].node_count + tree[cindex2].node_count +
                                      tree[cindex3].node_count + tree[cindex4].node_count;
    }
}

int main() {
    int cases;
    if (!(cin >> cases))
        return 0;

    for (int i = 0; i < cases; i++) {
        int k, n;
        cin >> k;
        n = 1 << k;
        vector<vector<int>> pixels(n, vector<int>(n));
        string row;
        vector<Node> tree;
        // Indices remain stable when the node pool grows; merged descendants stay allocated.

        for (int j = 0; j < n; j++) {
            cin >> row;
            for (int col = 0; col < n; col++)
                pixels[j][col] = row[col] - '0';
        }

        int root = build_tree(pixels, tree, n, 0, 0);
        int fliptimes;
        cin >> fliptimes;
        for (int j = 0; j < fliptimes; j++) {
            int x, y;
            cin >> x >> y;
            x--;
            y--;
            flip_pixel(tree, x, y, 0, 0, n, root);
            cout << tree[root].node_count << '\n';
        }
    }
    return 0;
}
