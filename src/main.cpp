#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

using namespace std;

void convert(string filepath) {
    string line;
    ifstream myfile (filepath.c_str());
    vector<Point_2> p;

    if(myfile.is_open())
    {
        while(getline(myfile, line))
        {
            const double pi = 3.1415926535897;
            double mapWidth = 600;
            double mapHeight = 800;

            double x, y;
            double x_mapped, y_mapped;
            double latRad, mercN;

            //cout << line << '\n' << endl;
            myfile >> x;
            myfile >> y;

            //turn off scientific notation
            cout.setf(ios::fixed, ios::floatfield);
            cout.setf(ios::showpoint);

            cout.precision(3);

            //get x value
            //x_mapped = (x+180)*(mapWidth/360);
            x_mapped = fmod(mapWidth*(x+180)/360, (mapWidth+(mapWidth)/2));

            //convert from degree to radians
            latRad = y*pi/180;

            //get y
            mercN = log(tan(pi/4)+(latRad/2));
            y_mapped = (mapHeight/2)-(mapWidth*mercN/(2*pi));


            //cout << x << " " << y << endl;
            //p.push_back(Point_2(x,y));
            p.push_back(Point_2(x_mapped, y_mapped));
        }
        myfile.close();
    }

    for(Point_2 point : p){
        cout << point << endl;
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " file.dat" << endl;
        return 1;
    }

    convert(argv[1]);
    return 0;
}
