//
// Created by lec on 2016. 11. 2..
//

#ifndef SAFEDESK_FILTERTOOL_H
#define SAFEDESK_FILTERTOOL_H

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

class FilterTool {
public:
    // too poor c++ development environment. template function should be defined within headerfile like inline function.
//    template<typename T>
//    vector<T> filter(vector<T> src, vector<T> kernel);
    vector<double> filter(vector<double> &src, vector<double> kernel);
};


#endif //SAFEDESK_FILTERTOOL_H
