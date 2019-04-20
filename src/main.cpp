#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <CGAL/Simple_cartesian.h>
#include "curve.hpp"

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

using namespace std;

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