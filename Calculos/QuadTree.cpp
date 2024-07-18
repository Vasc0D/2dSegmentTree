#include <cmath>
#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Estructura para representar un punto
struct Point {
    int x;
    int y;
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point() : x(0), y(0) {}
};

// Estructura para representar un nodo
struct Node {
    Point pos;
    int data;
    Node(Point _pos, int _data) : pos(_pos), data(_data) {}
    Node() : data(0) {}
};

// Clase QuadTree principal
class Quad {
    Point topLeft;
    Point botRight;
    Node* n;
    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;

public:
    Quad() : n(nullptr), topLeftTree(nullptr), topRightTree(nullptr), botLeftTree(nullptr), botRightTree(nullptr) {}
    Quad(Point topL, Point botR) : topLeft(topL), botRight(botR), n(nullptr), topLeftTree(nullptr), topRightTree(nullptr), botLeftTree(nullptr), botRightTree(nullptr) {}

    void insert(Node*);
    Node* search(Point);
    bool inBoundary(Point);
};

// Insertar un nodo en el QuadTree
void Quad::insert(Node* node) {
    if (node == nullptr) return;

    // El QuadTree actual no puede contener el nodo
    if (!inBoundary(node->pos)) return;

    // Estamos en un quad de área unitaria, no podemos subdividir más
    if (abs(topLeft.x - botRight.x) <= 1 && abs(topLeft.y - botRight.y) <= 1) {
        if (n == nullptr) n = node;
        return;
    }

    // Dividir en los cuadrantes correspondientes
    if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topLeftTree == nullptr)
                topLeftTree = new Quad(Point(topLeft.x, topLeft.y), Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        } else {
            if (botLeftTree == nullptr)
                botLeftTree = new Quad(Point(topLeft.x, (topLeft.y + botRight.y) / 2), Point((topLeft.x + botRight.x) / 2, botRight.y));
            botLeftTree->insert(node);
        }
    } else {
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topRightTree == nullptr)
                topRightTree = new Quad(Point((topLeft.x + botRight.x) / 2, topLeft.y), Point(botRight.x, (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        } else {
            if (botRightTree == nullptr)
                botRightTree = new Quad(Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2), Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}

// Buscar un nodo en el QuadTree
Node* Quad::search(Point p) {
    if (!inBoundary(p)) return nullptr;

    // Estamos en un quad de longitud unitaria, no podemos subdividir más
    if (n != nullptr) return n;

    // Buscar en los cuadrantes correspondientes
    if ((topLeft.x + botRight.x) / 2 >= p.x) {
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topLeftTree == nullptr) return nullptr;
            return topLeftTree->search(p);
        } else {
            if (botLeftTree == nullptr) return nullptr;
            return botLeftTree->search(p);
        }
    } else {
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topRightTree == nullptr) return nullptr;
            return topRightTree->search(p);
        } else {
            if (botRightTree == nullptr) return nullptr;
            return botRightTree->search(p);
        }
    }
}

// Verificar si un punto está dentro del límite del QuadTree
bool Quad::inBoundary(Point p) {
    return (p.x >= topLeft.x && p.x <= botRight.x && p.y >= topLeft.y && p.y <= botRight.y);
}

// Función para medir el tiempo y la memoria con promedios
void measure_time_and_memory(vector<vector<int>> &data, int iterations) {
    struct rusage usage;
    long build_duration_total = 0;
    long query_duration_total = 0;
    long build_memory_total = 0;
    long query_memory_total = 0;

    for (int i = 0; i < iterations; ++i) {
        auto start = high_resolution_clock::now();
        Quad center(Point(0, 0), Point(10000, 10000));
        for (auto &point : data) {
            center.insert(new Node(Point(point[0], point[1]), rand() % 100));
        }
        auto end = high_resolution_clock::now();
        build_duration_total += duration_cast<nanoseconds>(end - start).count();
        getrusage(RUSAGE_SELF, &usage);
        build_memory_total += usage.ru_maxrss;

        start = high_resolution_clock::now();
        Node* result = center.search(Point(5000, 5000));
        end = high_resolution_clock::now();
        query_duration_total += duration_cast<nanoseconds>(end - start).count();
        getrusage(RUSAGE_SELF, &usage);
        query_memory_total += usage.ru_maxrss;
    }

    long build_duration_avg = build_duration_total / iterations;
    long query_duration_avg = query_duration_total / iterations;
    long build_memory_avg = build_memory_total / iterations;
    long query_memory_avg = query_memory_total / iterations;

    cout << "Build time: " << build_duration_avg / 1e6 << " ms" << endl;
    cout << "Build memory: " << build_memory_avg << " KB" << endl;
    cout << "Query time: " << query_duration_avg / 1e6 << " ms" << endl;
    cout << "Query memory: " << query_memory_avg << " KB" << endl;
}

int main() {
    vector<int> sizes = {10, 100, 1000, 10000};

    for (int size : sizes) {
        cout << "Size: " << size << " nodes" << endl;
        vector<vector<int>> data(size, vector<int>(2));
        for (int i = 0; i < size; ++i) {
            data[i][0] = rand() % 10000;
            data[i][1] = rand() % 10000;
        }
        measure_time_and_memory(data, 30);
        cout << endl;
    }

    return 0;
}
