# Geometry
This is Peng Zou's, Ya Lan Yin's, and Nate Lane's final project for Montana State University's Computational Geometry course (CSCI 591, Spring 2019). For this project, we implemented the algorithm for listing what curves are within a certain Frechet distance of a query curve described in "On the complexity of range searching among curves" by Peyman Afshani and Anne Driemel.

## Building
This project depends on the CGAL library and uses CMake as its build system.
1. cd into the project directory
2. mkdir build
3. cd build
4. cmake ..
5. make

## Execution
This program has the following parameters:  
-d filepath: The dataset file to use. The default value is "../data/dataset.txt" (The format of this file is given in the next section).  
-n nCurves: The number of curves from the dataset to use. Sometimes it's helpful to limit the number of curves to be processed. The default is all curves listed in the dataset.  
-s stride: Take only every "stride"th point in each curve. That is, only put the point into the curve if the index of the point mod stride is 0. The default value is 1 (i.e., take every point in the curve).  
-q queryIndex: The index of the curve to use as a query curve. Default value: 0  
-f frechetDistance: The Frechet distance to use. Note that the curves will be scaled so that their points are in the range (0, 100), but this value will not be. The default value is 100.

## Input file formats
The dataset file should contain a list of files, each of which is a description of a single curve. Each curve file should have the following format:

x y k tid  
<x-coordinate for point 0> <y-coordinate for point 0> 0 <curve id>  
<x-coordinate for point 1> <y-coordinate for point 1> 1 <curve id>  
...  
<x-coordinate for point n - 1> <y-coordinate for point n -1> <n - 1> <curve id>

An example dataset can be found at http://www.martinwerner.de/files/dataset-sample.tgz. An explanation of this datset can be found at http://sigspatial2017.sigspatial.org/giscup2017/download.
