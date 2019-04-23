#include <iostream>
#include <set>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_naive_point_location.h>

#include "curve.hpp"

typedef CGAL::Cartesian<CGAL::Exact_rational>         Kernel;
typedef CGAL::Arr_circle_segment_traits_2<Kernel>     Traits_2;
typedef Traits_2::Point_2 Point_2;
typedef Traits_2::CoordNT CoordNT;
typedef Traits_2::X_monotone_curve_2 Segment_2;
typedef Kernel::Circle_2 Circle_2;
typedef Traits_2::Curve_2 Curve_2;
typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;
typedef CGAL::Arr_naive_point_location<Arrangement_2> Naive_pl;

using namespace std;

void arrangement_creatation(std::vector<Point_2> query_curve, double radius);
/**
int main()
{
    double radius = 0.5;
    std::vector<Point_2> query_curve;
    
    query_curve.push_back(Point_2(1, 2));
    query_curve.push_back(Point_2(2, 3));
    query_curve.push_back(Point_2(3, 2));
    query_curve.push_back(Point_2(4, 3));

    
    arrangement_creatation(query_curve, radius);
    
}
**/

std::vector<bool> column_creatation(std::vector<Point_2> query_curve, Point_2 p, double radius){
    std::vector<bool> column;
    for (unsigned i = 0; i < query_curve.size(); i++){
        int distance = 0;
        distance = pow((CGAL::to_double(query_curve[i].x()) - CGAL::to_double(p.x())),2) + pow((CGAL::to_double(query_curve[i].y())-CGAL::to_double(p.y())),2);
        distance = sqrt(distance);
        if (distance <= radius)
            column.push_back(true);
        else
            column.push_back(false);
    }
    return column;
}

Point_2 mid_point_creatation(Point_2 left, Point_2 right){
    double x = abs(CGAL::to_double(left.x()) + CGAL::to_double(right.x()));
    double y = abs(CGAL::to_double(left.y()) + CGAL::to_double(right.y()));
    Kernel::Point_2 point(x/2, y/2);
    Point_2 result(point.x(), point.y());
    std::cout<< result << std::endl;
    return result;
}

void print_column_set(std::set<std::vector<bool>> set){
    for(std::set<std::vector<bool>>::iterator it = set.begin(); it != set.end(); ++it){
        std::vector<bool> temp = *it;
        for(unsigned i = 0 ; i < temp.size(); i++){
            std::cout<<temp[i]<<"  ";
        }
        std::cout<<std::endl;
    }
}

void arrangement_creatation(std::vector<Point_2> query_curve, double radius){
    
    Arrangement_2 arr;
    
    std::set<std::vector<bool>> column_set;
    
    // insert all circles which are centered at each point of query_curve.
    CGAL::Exact_rational sqr_r = CGAL::Exact_rational(pow(radius, 2));
    for(unsigned i = 0; i < query_curve.size(); i ++){
        
        Circle_2 circ = Circle_2(Kernel::Point_2(CGAL::to_double(query_curve[i].x()), CGAL::to_double(query_curve[i].y())), sqr_r, CGAL::CLOCKWISE);
        Curve_2 cv = Curve_2(circ);
        insert(arr, cv);
    }

    Naive_pl   naive_pl(arr);
    // face to column
    Arrangement_2::Face_const_iterator fit;
    for (fit = arr.faces_begin(); fit != arr.faces_end(); ++fit){
        if (fit -> is_unbounded() == false){
            std::vector<Point_2> face_point_vector;
            Arrangement_2::Ccb_halfedge_const_circulator  circ = fit->outer_ccb();
            Arrangement_2::Ccb_halfedge_const_circulator  curr = circ;
            Arrangement_2::Halfedge_const_handle          he;
            
            face_point_vector.push_back(curr->source()->point());
            do
            {
                he = curr;
                face_point_vector.push_back(he->target()->point());
                ++curr;
            } while (curr != circ);
         
            // find the point inside the face.
            // 1. find the first and the mid point in the vector.
            Point_2 left = face_point_vector[0];
            int mid = (face_point_vector.size() - 1)/2;
            Point_2 right = face_point_vector[mid];
            //std::cout<<mid << "    mid  "<<std::endl;
            
            // 2.
            bool inside = false;
            do{
                Point_2 mid_point = mid_point_creatation(left, right);
                //std::cout << "left:   " << left << std::endl;
                //std::cout << "right:   " << right << std::endl;
                //std::cout << "mid_point:   " << mid_point << std::endl;
                CGAL::Arr_point_location_result<Arrangement_2>::Type obj = naive_pl.locate(mid_point);
                Arrangement_2::Face_const_handle* f;
                if (f = boost::get<Arrangement_2::Face_const_handle>(&obj)){// determine inside a face
                    if(fit == *f){
                        std::cout<<"   true  " << std::endl;
                        inside = true;
                    }
                }
                left = mid_point;
            } while(inside == false);
            std::vector<bool> column;
            column = column_creatation(query_curve, left, radius);
            column_set.insert(column);
        }
    }
    print_column_set(column_set);
    // print arrangement.
    // print_arrangement (arr);
    
}


void convert(string filepath) {
    ifstream myfile (filepath.c_str());
    vector<Point_2> p;

    if(myfile.is_open())
    {
        string header;
        getline(myfile, header);

        double x, y;
        int pointIndex, curveIndex;
        while(myfile >> x >> y >> pointIndex >> curveIndex)
        {
            double mapWidth = 600;
            double mapHeight = 800;

            double x_mapped, y_mapped;
            double latRad, mercN;

            //get x value
            //x_mapped = (x+180)*(mapWidth/360);
            x_mapped = fmod(mapWidth*(x+180)/360, (mapWidth+(mapWidth)/2));

            //convert from degree to radians
            latRad = y*M_PI/180;

            //get y
            mercN = log(tan(M_PI/4)+(latRad/2));
            y_mapped = (mapHeight/2)-(mapWidth*mercN/(2*M_PI));


            //cout << x << " " << y << endl;
            //p.push_back(Point_2(x,y));
            p.push_back(Point_2(x_mapped, y_mapped));
        }
        myfile.close();
    }

    //turn off scientific notation
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(3);

    for(Point_2 point : p){
        cout << point << endl;
    }
}

vector<curve> get_curves(string filename) {
    // Get the directory from the filename
    string directory = "";
    size_t slash_index = filename.rfind('/');
    if (slash_index != string::npos) {
        directory = filename.substr(0, slash_index + 1);
    }

    vector<curve> curves;

    ifstream dataset(filename.c_str());
    if (dataset.is_open()) {
        string datafile;

        while (dataset >> datafile) {
            double x, y;
            int pointIndex, curveIndex;
            vector<Point2> points;

            // Make sure we are looking in the right directory for the data
            string full_path = directory + datafile;
            ifstream datain(full_path.c_str());

            // First line in the file is a header
            string header;
            getline(datain, header);

            // Get the coordinates in each line
            while (datain >> x >> y >> pointIndex >> curveIndex) {
                points.push_back(Point2(x, y));
            }

            datain.close();
            curves.push_back(curve(points));
        }
    }

    dataset.close();
    return curves;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " dataset.txt" << endl;
        return 1;
    }

    vector<curve> curves = get_curves(argv[1]);
    for (curve c : curves) {
        c.print();
    }

    return 0;
}
