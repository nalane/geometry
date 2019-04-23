#include <vector>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point2;

class curve {
private:    
    std::vector<Point2> points;

public:
    curve(const std::vector<Point2>& points) : points(points) {}
    std::vector<std::vector<bool>> withinFrechetDistance(const curve& rhs, double ro);

    void print();
};