#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int left, right;
    int sum;
    Node() : left(0), right(0), sum(0) {}
};

class SegmentTree {
private:
    vector<Node> tree;

    void build(int node, int left, int right, const vector<int>& A) {
        if (left == right) {
            tree[node].sum = A[left];
            return;
        }

        int mid = (left + right) / 2;
        tree[node].left = tree.size();
        tree.push_back(Node());
        tree[node].right = tree.size();
        tree.push_back(Node());

        build(tree[node].left, left, mid, A);
        build(tree[node].right, mid + 1, right, A);

        tree[node].sum = tree[tree[node].left].sum + tree[tree[node].right].sum;
    }

    void update(int node, int left, int right, int index, int value) {
        if (left == right) {
            tree[node].sum = value;
            return;
        }

        int mid = (left + right) / 2;
        if (index <= mid) {
            tree.push_back(Node());
            tree[node].left = tree.size() - 1;
            update(tree[node].left, left, mid, index, value);
        } else {
            tree.push_back(Node());
            tree[node].right = tree.size() - 1;
            update(tree[node].right, mid + 1, right, index, value);
        }

        tree[node].sum = tree[tree[node].left].sum + tree[tree[node].right].sum;
    }

    int query(int node, int left, int right, int queryLeft, int queryRight) {
        if (queryRight < left || queryLeft > right) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return tree[node].sum;
        }

        int mid = (left + right) / 2;
        int leftSum = query(tree[node].left, left, mid, queryLeft, queryRight);
        int rightSum = query(tree[node].right, mid + 1, right, queryLeft, queryRight);

        return leftSum + rightSum;
    }

public:
    SegmentTree(vector<int>& A) {
        if (!A.empty()) {
            tree.push_back(Node());
            build(0, 0, A.size() - 1, A);
        }
    }

    void update(int index, int value) {
        update(0, 0, tree.size() - 1, index, value);
    }

    int query(int queryLeft, int queryRight) {
        return query(0, 0, tree.size() - 1, queryLeft, queryRight);
    }
};

vector<int> countZeroFountains(vector<int>& A, vector<vector<int>>& B) {
    int N = A.size();
    SegmentTree tree(A);
    vector<int> zeroFountainsCount;

    for (int i = 0; i < B.size(); i++) {
        int L = B[i][0] - 1;
        int R = B[i][1] - 1;
        int K = B[i][2];

        for (int j = L; j <= R; j++) {
            int currentWaterLevel = tree.query(j, j);
            int newWaterLevel = max(0, currentWaterLevel - K);
            tree.update(j, newWaterLevel);
        }

        zeroFountainsCount.push_back(tree.query(0, N - 1));
    }

    return zeroFountainsCount;
}

int main() {
    vector<int> A1 = {5, 7, 3};
    vector<vector<int>> B1 = {{1, 2, 1}, {2, 3, 1}, {1, 3, 4}};
    vector<int> output1 = countZeroFountains(A1, B1);
    for (int count : output1) {
        cout << count << " ";
    }
    cout << endl;  // Output should be 0 0 2

    vector<int> A2 = {1};
    vector<vector<int>> B2 = {{1, 1, 10}};
    vector<int> output2 = countZeroFountains(A2, B2);
    for (int count : output2) {
        cout << count << " ";
    }
    cout << endl;  // Output should be 1

    return 0;
}
