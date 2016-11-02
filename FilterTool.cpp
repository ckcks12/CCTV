//
// Created by lec on 2016. 11. 2..
//

#include "FilterTool.h"


//template<typename T>
//vector<T> FilterTool::filter(vector<T> src, vector<T> kernel) {
//    assert(kernel.size() % 2 != 0);
//
//    vector<T> vec;
//    double norm = 0;
//    int padding = 0;
//
//    for( size_t i=0; i<kernel.size(); i++ )
//    {
//        norm += kernel[i];
//    }
//    if( norm == 0 )
//        norm = 1;
//    else
//        norm = (double)1/norm;
//
//    padding = (int)floor(kernel.size() / 2);
//    for( size_t i=0; i<padding; i++ )
//    {
//        vec.push_back(src[i]);
//    }
//    for( int i=padding; i<src.size()-padding; i++ )
//    {
//        T val = 0;
//
//        for( int j=-padding; j<=padding; j++ )
//        {
//            val += src[i+j] * kernel[padding + j];
//        }
//        val = val * norm;
//        vec.push_back(val);
//    }
//    for( size_t i=src.size()-padding; i<src.size(); i++ )
//    {
//        vec.push_back(src[i]);
//    }
//
//    return vec;
//}

vector<double> FilterTool::filter(vector<double> &src, vector<double> kernel) {
    assert(kernel.size() % 2 != 0);

    vector<double> vec;
    double norm = 0;
    int padding = 0;

    for( size_t i=0; i<kernel.size(); i++ )
    {
        norm += kernel[i];
    }
    if( norm == 0 )
        norm = 1;
    else
        norm = (double)1/norm;

    padding = (int)floor(kernel.size() / 2);
    for( size_t i=0; i<padding; i++ )
    {
        vec.push_back(src[i]);
    }
    for( int i=padding; i<src.size()-padding; i++ )
    {
        double val = 0;

        for( int j=-padding; j<=padding; j++ )
        {
            val += src[i+j] * kernel[padding + j];
        }
        val = val * norm;
        vec.push_back(val);
    }
    for( size_t i=src.size()-padding; i<src.size(); i++ )
    {
        vec.push_back(src[i]);
    }

    return vec;
}
