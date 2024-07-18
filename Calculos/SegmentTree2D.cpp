#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>

using namespace std;
using namespace chrono;

class SegmentTree2D {
private:
    int n, m;
    vector<vector<int>> tree;
    vector<vector<int>> data;

    void build_y(int vx, int lx, int rx, int vy, int ly, int ry) {
        if (ly == ry) {
            if (lx == rx) {
                tree[vx][vy] = data[lx][ly];
            } else {
                tree[vx][vy] = tree[vx * 2][vy] + tree[vx * 2 + 1][vy];
            }
        } else {
            int my = (ly + ry) / 2;
            build_y(vx, lx, rx, vy * 2, ly, my);
            build_y(vx, lx, rx, vy * 2 + 1, my + 1, ry);
            tree[vx][vy] = tree[vx][vy * 2] + tree[vx][vy * 2 + 1];
        }
    }

    void build_x(int vx, int lx, int rx) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            build_x(vx * 2, lx, mx);
            build_x(vx * 2 + 1, mx + 1, rx);
        }
        build_y(vx, lx, rx, 1, 0, m - 1);
    }

    void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val) {
        if (ly == ry) {
            if (lx == rx) {
                tree[vx][vy] = new_val;
            } else {
                tree[vx][vy] = tree[vx * 2][vy] + tree[vx * 2 + 1][vy];
            }
        } else {
            int my = (ly + ry) / 2;
            if (y <= my) {
                update_y(vx, lx, rx, vy * 2, ly, my, x, y, new_val);
            } else {
                update_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, new_val);
            }
            tree[vx][vy] = tree[vx][vy * 2] + tree[vx][vy * 2 + 1];
        }
    }

    void update_x(int vx, int lx, int rx, int x, int y, int new_val) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            if (x <= mx) {
                update_x(vx * 2, lx, mx, x, y, new_val);
            } else {
                update_x(vx * 2 + 1, mx + 1, rx, x, y, new_val);
            }
        }
        update_y(vx, lx, rx, 1, 0, m - 1, x, y, new_val);
    }

    int sum_y(int vx, int vy, int tly, int try_, int ly, int ry) {
        if (ly > ry) {
            return 0;
        }
        if (ly == tly && ry == try_) {
            return tree[vx][vy];
        }
        int tmy = (tly + try_) / 2;
        return sum_y(vx, vy * 2, tly, tmy, ly, min(ry, tmy)) +
               sum_y(vx, vy * 2 + 1, tmy + 1, try_, max(ly, tmy + 1), ry);
    }

    int sum_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
        if (lx > rx) {
            return 0;
        }
        if (lx == tlx && rx == trx) {
            return sum_y(vx, 1, 0, m - 1, ly, ry);
        }
        int tmx = (tlx + trx) / 2;
        return sum_x(vx * 2, tlx, tmx, lx, min(rx, tmx), ly, ry) +
               sum_x(vx * 2 + 1, tmx + 1, trx, max(lx, tmx + 1), rx, ly, ry);
    }

public:
    SegmentTree2D(vector<vector<int>> &input_data) {
        n = input_data.size();
        m = input_data[0].size();
        data = input_data;
        tree.resize(4 * n, vector<int>(4 * m));
        build_x(1, 0, n - 1);
    }

    void update(int x, int y, int new_val) {
        update_x(1, 0, n - 1, x, y, new_val);
    }

    int sum(int lx, int rx, int ly, int ry) {
        return sum_x(1, 0, n - 1, lx, rx, ly, ry);
    }
};

// Función para medir el tiempo y la memoria
void measure_time_and_memory(vector<vector<int>> &data, int iterations) {
    struct rusage usage;
    long build_duration = 0;
    long query_duration = 0;

    for (int i = 0; i < iterations; ++i) {
        auto start = high_resolution_clock::now();
        SegmentTree2D st(data);
        auto end = high_resolution_clock::now();
        build_duration += duration_cast<nanoseconds>(end - start).count();

        start = high_resolution_clock::now();
        int result = st.sum(0, data.size() - 1, 0, data[0].size() - 1);
        end = high_resolution_clock::now();
        query_duration += duration_cast<nanoseconds>(end - start).count();
    }

    build_duration /= iterations;
    query_duration /= iterations;
    getrusage(RUSAGE_SELF, &usage);
    long build_memory = usage.ru_maxrss;
    long query_memory = usage.ru_maxrss;

    cout << "Build time: " << build_duration / 1e6 << " ms" << endl;
    cout << "Build memory: " << build_memory << " KB" << endl;
    cout << "Query time: " << query_duration / 1e6 << " ms" << endl;
    cout << "Query memory: " << query_memory << " KB" << endl;
}

int main() {
    vector<int> sizes = {10, 100, 1000, 10000};

    for (int size : sizes) {
        cout << "Size: " << size << "x" << size << endl;
        vector<vector<int>> data(size, vector<int>(size, 1));
        measure_time_and_memory(data, 30);
        cout << endl;
    }

    return 0;
}
