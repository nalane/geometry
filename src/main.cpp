#include <iostream>
#include <vector>
#include <CGAL/Simple_cartesian.h>

using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point2;

class curve {
private:    
    vector<Point2> points;

public:
    curve(const vector<Point2>& points) : points(points) {}
    bool withinFrechetDistance(const curve& rhs, double ro);
};

bool curve::withinFrechetDistance(const curve& rhs, double ro) {
    bool arr[points.size()][rhs.points.size()];
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < rhs.points.size(); j++) {
            arr[i][j] = CGAL::squared_distance(points[i], rhs.points[j]) < ro * ro;
        }
    }
    return true;
}

int main() {
    cout << "Hello World" << endl;
    return 0;
}
