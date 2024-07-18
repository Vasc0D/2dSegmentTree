#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

const int k = 2;

// A structure to represent node of kd tree
struct Node
{
    int point[k]; // To store k dimensional point
    Node *left, *right;
};

// A method to create a node of K D tree
struct Node* newNode(int arr[])
{
    struct Node* temp = new Node;

    for (int i = 0; i < k; i++)
        temp->point[i] = arr[i];

    temp->left = temp->right = NULL;
    return temp;
}

// Inserts a new node and returns root of modified tree
// The parameter depth is used to decide axis of comparison
Node *insertRec(Node *root, int point[], unsigned depth)
{
    // Tree is empty?
    if (root == NULL)
        return newNode(point);

    // Calculate current dimension (cd) of comparison
    unsigned cd = depth % k;

    // Compare the new point with root on current dimension 'cd'
    // and decide the left or right subtree
    if (point[cd] < (root->point[cd]))
        root->left = insertRec(root->left, point, depth + 1);
    else
        root->right = insertRec(root->right, point, depth + 1);

    return root;
}

// Function to insert a new point with given point in
// KD Tree and return new root. It mainly uses above recursive
// function "insertRec()"
Node* insert(Node *root, int point[])
{
    return insertRec(root, point, 0);
}

// A utility method to determine if two Points are same
// in K Dimensional space
bool arePointsSame(int point1[], int point2[])
{
    // Compare individual pointinate values
    for (int i = 0; i < k; ++i)
        if (point1[i] != point2[i])
            return false;

    return true;
}

// Searches a Point represented by "point[]" in the K D tree.
// The parameter depth is used to determine current axis.
bool searchRec(Node* root, int point[], unsigned depth)
{
    // Base cases
    if (root == NULL)
        return false;
    if (arePointsSame(root->point, point))
        return true;

    // Current dimension is computed using current depth and total
    // dimensions (k)
    unsigned cd = depth % k;

    // Compare point with root with respect to cd (Current dimension)
    if (point[cd] < root->point[cd])
        return searchRec(root->left, point, depth + 1);

    return searchRec(root->right, point, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses
// searchRec()
bool search(Node* root, int point[])
{
    // Pass current depth as 0
    return searchRec(root, point, 0);
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
        Node* root = nullptr;
        for (auto &point : data) {
            root = insert(root, point.data());
        }
        auto end = high_resolution_clock::now();
        build_duration_total += duration_cast<nanoseconds>(end - start).count();
        getrusage(RUSAGE_SELF, &usage);
        build_memory_total += usage.ru_maxrss;

        start = high_resolution_clock::now();
        int point[2] = {5000, 5000};
        bool result = search(root, point);
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
        cout << "Size: " << size << " points" << endl;
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
