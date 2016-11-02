#include <iostream>
#include <opencv2/opencv.hpp>
#include "BlobDetector.h"
#include "GraphTool.h"
#include "FilterTool.h"
#include "StarSkeleton.h"


using namespace cv;
using namespace std;

int main() {
    const int THIEF_CAM_NO = 2;
    const int ALBA_CAM_NO = 1;
    const int INPUT = 2; // default 0;
    const int WIDTH = 640;
    const int HEIGHT = 360;

    VideoCapture cam1, cam2;
    BlobDetector bd;
    StarSkeleton skeleton;
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
    else if( input == 2 ) {
        Mat mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png", 0);
        resize(mat, mat, Size(WIDTH, HEIGHT));

        Mat original_mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png");
        resize(original_mat, original_mat, Size(WIDTH, HEIGHT));

        // delete noise
        bd.deleteNoise(mat);

        // retrieve contours
        vector<vector<Point>> contours;
        contours = bd.getContoursBiggerFirst(mat);
        assert(contours.size() > 0);

        // contours[0] is the biggest blob
        vector<Point> contour = contours[0];
        vector<double> dist = skeleton.getDistanceFromCentroid(contour);
        assert(dist.size() > 0);

        // get star skeleton features
        vector<Point> features = skeleton.getFeatures(contour, dist);
        assert(features.size() > 5);

        for( int i=0; i<5; i++ )
        {
            circle(original_mat, features[i], 3, Scalar(0, 255, 0), 3);
        }

        imshow("original_mat", original_mat);
        waitKey(0);
    }
    // 그외
    else
    {
        cout << "idiot..." << endl;
    }

    return 0;
}