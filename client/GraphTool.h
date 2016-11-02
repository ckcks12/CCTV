//
// Created by lec on 2016. 11. 2..
//

#ifndef SAFEDESK_GRAPHTOOL_H
#define SAFEDESK_GRAPHTOOL_H

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

class GraphTool {
public:
    template<typename T>
            Mat drawGraph(vector<T> vec, int width, int height, Scalar color);
};


#endif //SAFEDESK_GRAPHTOOL_H
