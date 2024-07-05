#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

using namespace std;

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

namespace py = pybind11;

PYBIND11_MODULE(segment_tree, m) {
    py::class_<SegmentTree2D>(m, "SegmentTree2D")
        .def(py::init<vector<vector<int>>&>())
        .def("update", &SegmentTree2D::update)
        .def("sum", &SegmentTree2D::sum);
}
