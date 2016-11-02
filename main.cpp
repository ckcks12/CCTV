#include <iostream>
#include <opencv2/opencv.hpp>
#include "BlobDetector.h"

using namespace cv;
using namespace std;


template<typename T>
Mat drawGraph(vector<T> vec, int width = 640, int height = 360, Scalar color = Scalar(0, 0, 255))
{
    Mat mat(Size(width, height), CV_8UC3);
    T max = 0;
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

template<typename T>
vector<T> filter(vector<T> src, vector<T> kernel)
{
    assert(kernel.size() % 2 != 0);

    vector<T> vec;
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
        T val = 0;

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

int main() {
    const int THIEF_CAM_NO = 2;
    const int ALBA_CAM_NO = 1;
    const int INPUT = 2; // default 0;

    VideoCapture cam1, cam2;
    BlobDetector bd;
    int input;


//    cam1.open(THIEF_CAM_NO);
//    cam1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
//    cam1.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
//    cam1.set(CV_CAP_PROP_FOCUS, 0);
//    cam1.set(CV_CAP_PROP_AUTO_EXPOSURE, false);
//    cam1.set(CV_CAP_PROP_AUTOGRAB, false);

    if( ! INPUT )
    {
        cout << "===== SAFE TASK =====" << endl;
        cout << "1. 웹캠" << endl;
        cout << "2. 사진" << endl;
        cout << endl;

        cout << ">>> ";
        cin >> input;
    }
    else
    {
        input = INPUT;
    }

    // 웹캠
    if( input == 1 )
    {

    }
    // 사진
    else if( input == 2 )
    {
        Mat mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png", 0);
        resize(mat, mat, Size(640, 360));
        Mat original_mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png");
        resize(original_mat, original_mat, Size(640, 360));
        bd.deleteNoise(mat);

        dilate(mat, mat, Mat(), Point(-1, -1), 3);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchies;

        findContours(mat, contours, hierarchies, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        size_t nAreaCnt = contours.size();
        cvtColor(mat, mat, CV_GRAY2BGR);
//        for(int i=0; i< nAreaCnt; i++){
//            Scalar color = Scalar(rand()%255, rand()%255, rand()%255 );//랜덤 색 만들기
//            int thickness = 2;//CV_FILLED=내부 채움
//            thickness = -1;
//            drawContours( mat, contours, i, color, thickness , 8);//외곽선 그리기
//        }
        Scalar color = Scalar(rand()%255, rand()%255, rand()%255);
//        drawContours(mat, contours, 3 , color, -1);
        Moments m = moments(contours[3]);
        Point c(m.m10/m.m00, m.m01/m.m00);


        double max = -1;

        vector<double> dists;
        for( int i=0; i<contours[3].size(); i++ )
        {
            double dist = norm(c - contours[3][i]);
            dists.push_back(dist);
            if( dist > max )
                max = dist;
        }

        vector<double> dists2 = filter<double>(dists, vector<double>({0.5, 0.9, 1, 0.9, 0.5}));

        Mat graph = drawGraph<double>(dists);
        Mat graph2 = drawGraph<double>(dists2);

        bool increase = true;
        for( int i=1; i<dists2.size(); i++ )
        {
            if( increase && dists2[i-1] > dists2[i] )
            {
                Point p1 = Point(0, 360 - (int)((dists2[i] / max) * 360));
                Point p2 = Point(640, 360 - (int)((dists2[i] / max) * 360));
                line(graph2, p1, p2, Scalar(0, 255, 0), 1);
                circle(original_mat, contours[3][i], 3, Scalar(0, 255, 0), 3);
                increase = false;
            }
            else if( ! increase && dists2[i-1] < dists2[i] )
            {
                increase = true;
            }
        }

        imshow("original_mat", original_mat);
        imshow("graph1", graph);
        imshow("graph2", graph2);
        waitKey(0);
    }
    // 그외
    else
    {
        cout << "idiot..." << endl;
    }

    return 0;
}