#include "curve.hpp"

using namespace std;

vector<vector<bool>> curve::withinFrechetDistance(const curve& rhs, double ro) {
    vector<vector<bool>> arr(points.size());
    for (auto& a : arr) {
        a = vector<bool>(rhs.points.size());
    }

    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < rhs.points.size(); j++) {
            arr[i][j] = CGAL::squared_distance(points[i], rhs.points[j]) < ro * ro;
        }
    }
    return arr;
}