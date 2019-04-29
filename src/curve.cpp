#include <iostream>

#include "curve.hpp"

using namespace std;

vector<vector<bool>> curve::free_space_matrix(const curve& q, double ro) {
    vector<vector<bool>> arr(points.size());
    for (auto& a : arr) {
        a = vector<bool>(q.points.size());
    }

    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < q.points.size(); j++) {
            arr[i][j] = CGAL::squared_distance(points[i], q.points[j]) < ro * ro;
        }
    }

    /*
    cout << "Free space matrix:" << endl;
    for (int i = 0; i < q.points.size(); i++) {
        cout << "(";
        for (int j = 0; j < points.size(); j++) {
            cout << arr[j][i] << ", ";
        }
        cout << ")" << endl;
    }
    */

    return arr;
}

std::vector<Double_Point_2> curve::get_curve(){
    return points;
}

void curve::print() {
    for (Double_Point_2 p : points) {
        cout << p << "\n";
    }
}