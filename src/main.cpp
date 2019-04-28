#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

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

std::set<std::vector<bool>> arrangement_creation(std::vector<Trait_Point_2> query_curve, double radius){

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

      // 2.
      bool inside = false;
      int index_one = 0;
      int index_two = 1;
      int vector_length = face_point_vector.size() - 1;
      Trait_Point_2 mid_point;
      while(inside == false){
        Trait_Point_2 left = face_point_vector[index_one];
        Trait_Point_2 right = face_point_vector[index_two];
        mid_point = mid_point_creatation(left, right);

        CGAL::Arr_point_location_result<Arrangement_2>::Type obj = naive_pl.locate(mid_point);
        Arrangement_2::Face_const_handle* f;
        if (f = boost::get<Arrangement_2::Face_const_handle>(&obj)){
          if(fit == *f){
            //std::cout<<"   true  " << std::endl;
            inside = true;
          }
        }
        index_two += 1;
        if (index_two = vector_length){
          index_one += 1;
          index_two = index_one + 1;
        }
        if (index_one == vector_length){
          std::cout<< "big error!!!!!!"<<std::endl;
          return column_set;
        }

      }

      std::vector<bool> column;
      column = column_creation(query_curve, mid_point, radius);
      column_set.insert(column);

    }
  }
    print_column_set(column_set);
    return column_set;
    // print arrangement.
    // print_arrangement (arr);
}

std::vector<Trait_Point_2> point_convertor(curve curve){
  std::vector<Trait_Point_2> result;
  std::vector<Double_Point_2> points = curve.get_curve();
  for (unsigned i = 0; i < points.size(); i++){
    result.push_back(Trait_Point_2(points[i].x(), points[i].y()));
  }
  return result;
}

map<size_t, curve> get_curves(string filename) {
    // Get the directory from the filename
    string directory = "";
    size_t slash_index = filename.rfind('/');
    if (slash_index != string::npos) {
        directory = filename.substr(0, slash_index + 1);
    }

    map<size_t, curve> curves;

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
                points.push_back(Double_Point_2(x, y));
                cout << points.back() << endl;
            }

            cout << points.size();

            datain.close();
            curves[curveIndex] = curve(points);
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

    map<size_t, curve> curves = get_curves(datafile);

    // Step 1.5: Get query curve
    curve q = curves[queryIndex];

    //convert the query curve to Trait_Point_2
    std::vector<Trait_Point_2> query_curve_converted =  point_convertor(q);

    //create arrangement
    std::set<std::vector<bool>> column_set_result = arrangement_creation(query_curve_converted, 10.0);

    //get the free_space_matrix for the query curve
    //vector<vector<bool>> arr = q.free_space_matrix(q, 10.0);


    curves.erase(queryIndex);

    // Step 2: free space: map<curve, matrix>
    // Step 3: kernel -> trait_2
    // Step 4: get column set
    // Step 5: Filter

    //for (auto p : curves) {
        //p.second.print();
    //}

    return 0;
}
