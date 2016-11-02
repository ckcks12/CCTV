//
// Created by lec on 2016. 11. 2..
//

#include "GraphTool.h"

//template<typename T>
//Mat GraphTool::drawGraph(vector<T> vec, int width, int height, Scalar color) {
//    Mat mat(Size(width, height), CV_8UC3);
//    T max = 0;
//    for( size_t i=0; i<vec.size(); i++ )
//    {
//        if( vec[i] >= max )
//            max = vec[i];
//    }
//
//    int step = (int)floor(width / vec.size());
//    int x1 = 0;
//    int y1 = height - (int)((vec[0] / max) * height);
//    line(mat, Point(x1, y1), Point(x1, y1), color, 2);
//
//
//    for( size_t i=0; i<vec.size(); i++ )
//    {
//        int y2 = height - (int)((vec[i] / max) * height);
//        int x2 = x1 + step;
//        line(mat, Point(x1, y1), Point(x2, y2), color, 2);
//        x1 = x2;
//        y1 = y2;
//    }
//
//    return mat;
//}


Mat GraphTool::drawGraph(vector<double> vec, int width, int height, Scalar color) {
    Mat mat(Size(width, height), CV_8UC3, Scalar(0, 0, 0));
    double max = 0;
    for( size_t i=0; i<vec.size(); i++ )
    {
        if( vec[i] >= max )
            max = vec[i];
    }

    int step = (int)floor(width / vec.size());
    int x1 = 0;
    int y1 = height - (int)((vec[0] / max) * height);
    line(mat, Point(x1, y1), Point(x1, y1), color, 2);


    for( size_t i=0; i<vec.size(); i++ )
    {
        int y2 = height - (int)((vec[i] / max) * height);
        int x2 = x1 + step;
        line(mat, Point(x1, y1), Point(x2, y2), color, 2);
        x1 = x2;
        y1 = y2;
    }

    return mat;
}