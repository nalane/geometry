#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <float.h>

#include "curve.hpp"
#include "cgal.hpp"

using namespace std;

const double DIM = 100;

vector<bool> column_creation(vector<Trait_Point_2> query_curve, Trait_Point_2 p, double radius){
    vector<bool> column;
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

Trait_Point_2 mid_point_creation(Trait_Point_2 left, Trait_Point_2 right){
    //cout << "(" << left << " -> " << right << ")\n";

    double x = CGAL::to_double(left.x()) + CGAL::to_double(right.x());
    double y = CGAL::to_double(left.y()) + CGAL::to_double(right.y());

    Double_Point_2 point(x/2, y/2);
    Trait_Point_2 result(point.x(), point.y());

    //cout<< result << endl;
    return result;
}

void print_column_set(set<vector<bool>> columns){
    for (vector<bool> temp : columns) {
        for(unsigned i = 0 ; i < temp.size(); i++){
            cout << temp[i] << "  " << endl;
        }
        cout<<endl;
    }
}

set<vector<bool>> arrangement_creation(vector<Trait_Point_2> query_curve, double radius){
    Arrangement_2 arr;

    set<vector<bool>> column_set;

    // insert all circles which are centered at each point of query_curve.
    CGAL::Exact_rational sqr_r = CGAL::Exact_rational(pow(radius, 2));
    for(unsigned i = 0; i < query_curve.size(); i ++){

        Circle_2 circ = Circle_2(Double_Point_2(CGAL::to_double(query_curve[i].x()), CGAL::to_double(query_curve[i].y())), sqr_r, CGAL::CLOCKWISE);
        Curve_2 cv = Curve_2(circ);
        insert(arr, cv);
    }

    Naive_pl naive_pl(arr);
    // face to column
    Arrangement_2::Face_const_iterator fit;
    int counter = 0;
    for (fit = arr.faces_begin(); fit != arr.faces_end(); ++fit){
        if (fit -> is_unbounded() == false){
            cout << counter << ": ";
            counter++;

            vector<Trait_Point_2> face_point_vector;
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

            int vector_length = face_point_vector.size() - 1;
            cout << vector_length << " vertices, ";

            // 2.
            bool inside = false;
            int index_one = 0;
            int index_two = 1;
            Trait_Point_2 mid_point;
            while(inside == false){
                Trait_Point_2 left = face_point_vector[index_one];
                Trait_Point_2 right = face_point_vector[index_two];
                mid_point = mid_point_creation(left, right);

                CGAL::Arr_point_location_result<Arrangement_2>::Type obj = naive_pl.locate(mid_point);
                Arrangement_2::Face_const_handle* f;
                if (f = boost::get<Arrangement_2::Face_const_handle>(&obj)){
                    if(fit == *f){
                        //cout<<"   true  " << endl;
                        inside = true;
                        cout << mid_point << endl;
                        break;
                    }
                }
                index_two += 1;
                if (index_two == vector_length){
                    index_one += 1;
                    index_two = index_one + 1;
                }
                if (index_one == vector_length - 1){
                    cout << "No point found in face" << endl;
                    //return column_set;
                    break;
                }
            }

            if (inside) {
                vector<bool> column;
                column = column_creation(query_curve, mid_point, radius);
                column_set.insert(column);
            }
        }
    }
    //print_column_set(column_set);
    return column_set;
    // print arrangement.
    // print_arrangement (arr);
}

vector<Trait_Point_2> point_convertor(curve curve){
    vector<Trait_Point_2> result;
    vector<Double_Point_2> points = curve.get_curve();
    for (unsigned i = 0; i < points.size(); i++){
        result.push_back(Trait_Point_2(points[i].x(), points[i].y()));
    }
    return result;
}

map<size_t, curve> get_curves(string filename, size_t stride) {
    // Get the directory from the filename
    string directory = "";
    size_t slash_index = filename.rfind('/');
    if (slash_index != string::npos) {
        directory = filename.substr(0, slash_index + 1);
    }

    map<size_t, vector<Double_Point_2>> curves;

    double minX = DBL_MAX;
    double minY = DBL_MAX;
    double maxX = -DBL_MAX;
    double maxY = -DBL_MAX;

    ifstream dataset(filename.c_str());
    if (dataset.is_open()) {
        string datafile;

        while (dataset >> datafile) {
            double x, y;
            size_t pointIndex, curveIndex;
            vector<Double_Point_2> points;

            // Make sure we are looking in the right directory for the data
            string full_path = directory + datafile;
            ifstream datain(full_path.c_str());

            // First line in the file is a header
            string header;
            getline(datain, header);

            // Get the coordinates in each line
            while (datain >> x >> y >> pointIndex >> curveIndex) {
                if (pointIndex % stride == 0) {
                    points.push_back(Double_Point_2(x, y));
                    if (x < minX)
                        minX = x;
                    if (y < minY)
                        minY = y;
                    if (x > maxX)
                        maxX = x;
                    if (y > maxY)
                        maxY = y;
                }
            }

            //cout << points.size();

            datain.close();
            curves[curveIndex] = points;
        }
    }

    dataset.close();

    maxX -= minX;
    maxY -= minY;
    double max = maxX > maxY ? maxX : maxY;

    double conversion_factor = DIM / max;
    map<size_t, curve> c;
    for (auto p : curves) {
        vector<Double_Point_2> points;
        for (Double_Point_2 point : p.second) {
            CGAL::Exact_rational x = (point.x() - minX) * conversion_factor;
            CGAL::Exact_rational y = (point.y() - minY) * conversion_factor;
            points.push_back(Double_Point_2(x, y));
        }
        c[p.first] = curve(points);
    }

    return c;
}

bool filter(set<vector<bool>> column_set_result, vector<vector<bool>> arr){
    //loop through the matrix to make sure that each column is in the set
    for(vector<bool> column : arr){
        set<vector<bool>>::iterator it = column_set_result.find(column);
        if (it == column_set_result.end())
            return false;
    }
  //we found a match, if none of the for loop iterations failed
  return true;
}

int main(int argc, char** argv) {
    // Default parameters
    string datafile = "../data/dataset.txt";
    int queryIndex = 0;
    int stride = 1;
    int numCurves = -1;
    double frechetDistance = 100.0;

    // Get modified parameters
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-d") == 0)
                datafile = argv[++i];
            else if (strcmp(argv[i], "-q") == 0)
                queryIndex = atoi(argv[++i]);
            else if (strcmp(argv[i], "-f") == 0)
                frechetDistance = stod(argv[++i]);
            else if (strcmp(argv[i], "-s") == 0)
                stride = atoi(argv[++i]);
            else if (strcmp(argv[i], "-n") == 0)
                numCurves = atoi(argv[++i]);
        }
    }

    // Read the curves in from the files
    map<size_t, curve> curves = get_curves(datafile, stride);

    // Get query curve
    curve q = curves[queryIndex];
    curves.erase(queryIndex);

    // Limit number of curves
    if (numCurves > 0) {
        map<size_t, curve>::iterator it = curves.upper_bound(numCurves);
        curves.erase(it, curves.end());
    }

    // kernel -> trait_2
    vector<Trait_Point_2> query_curve_converted =  point_convertor(q);

    // get column set
    set<vector<bool>> column_set_result =
        arrangement_creation(query_curve_converted, frechetDistance);

    // Print which curves satisfy the filter
    cout << "The folowing curves are within a Frechet distance " <<
        frechetDistance << " of curve " << queryIndex << "\n";
    for (auto p : curves) {
        // Get the free space matrix of the current curve with the query curve
        vector<vector<bool>> matrix = p.second.free_space_matrix(q, frechetDistance);

        // Apply filter
        if (filter(column_set_result, matrix)) {
            cout << p.first << "\n";
        }
        else{
          //cout << "Not valid" << endl;
        }
    }

    return 0;
}
