#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "curve.hpp"
#include "cgal.hpp"

using namespace std;

std::vector<bool> column_creation(std::vector<Trait_Point_2> query_curve, Trait_Point_2 p, double radius){
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

Trait_Point_2 mid_point_creatation(Trait_Point_2 left, Trait_Point_2 right){
    double x = abs(CGAL::to_double(left.x()) + CGAL::to_double(right.x()));
    double y = abs(CGAL::to_double(left.y()) + CGAL::to_double(right.y()));
    Double_Point_2 point(x/2, y/2);
    Trait_Point_2 result(point.x(), point.y());
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

void arrangement_creation(std::vector<Trait_Point_2> query_curve, double radius){
    
    Arrangement_2 arr;
    
    std::set<std::vector<bool>> column_set;
    
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
    for (fit = arr.faces_begin(); fit != arr.faces_end(); ++fit){
        if (fit -> is_unbounded() == false){
            std::vector<Trait_Point_2> face_point_vector;
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
            Trait_Point_2 left = face_point_vector[0];
            int mid = (face_point_vector.size() - 1)/2;
            Trait_Point_2 right = face_point_vector[mid];
            //std::cout<<mid << "    mid  "<<std::endl;
            
            // 2.
            bool inside = false;
            do{
                Trait_Point_2 mid_point = mid_point_creatation(left, right);
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
            column = column_creation(query_curve, left, radius);
            column_set.insert(column);
        }
    }
    print_column_set(column_set);
    // print arrangement.
    // print_arrangement (arr);
    
}

std::vector<std::vector<Trait_Point_2>> point_convertor(std::vector<curve> curves){
    std::vector<std::vector<Trait_Point_2>> result;
    for(unsigned i = 0; i < curves.size(); i++){
        std::vector<Double_Point_2> curve = curves[i].get_curve();
        std::vector<Trait_Point_2> temp;
        for(unsigned j = 0 ; j < curve.size(); j++){
            temp.push_back(Trait_Point_2(curve[j].x(), curve[j].y()));
        }
        result.push_back(temp);
    }
    return result;
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
            vector<Double_Point_2> points;

            // Make sure we are looking in the right directory for the data
            string full_path = directory + datafile;
            ifstream datain(full_path.c_str());

            // First line in the file is a header
            string header;
            getline(datain, header);

            // Get the coordinates in each line
            while (datain >> x >> y >> pointIndex >> curveIndex) {
                points.push_back(Double_Point_2(x, y));
            }

            datain.close();
            curves.push_back(curve(points));
        }
    }

    dataset.close();
    return curves;
}

int main(int argc, char** argv) {
    string datafile = "../data/dataset.txt";
    int queryIndex = 0;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-d") == 0)
                datafile = argv[++i];
            else if (strcmp(argv[i], "-q") == 0)
                queryIndex = atoi(argv[++i]);
        }
    }

    vector<curve> curves = get_curves(datafile);

    // Step 1.5: Get query curve
    // Step 2: free space: map<curve, matrix>
    // Step 3: kernel -> trait_2
    // Step 4: get column set
    // Step 5: Filter

    for (curve c : curves) {
        c.print();
    }

    return 0;
}
