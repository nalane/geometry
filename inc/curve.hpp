#pragma once

#include <vector>
#include "cgal.hpp"

class curve {
private:    
    std::vector<Double_Point_2> points;

public:
    curve(const std::vector<Double_Point_2>& points) : points(points) {}
    std::vector<std::vector<bool>> free_space_matrix(const curve& q, double ro);

    void print();
};