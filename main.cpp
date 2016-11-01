#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cam1, cam2;
    const int THIEF_CAM_NO = 2;
    const int ALBA_CAM_NO = 1;

    cam1.open(THIEF_CAM_NO);
    cam1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cam1.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
    cam1.set(CV_CAP_PROP_FOCUS, 0);
    cam1.set(CV_CAP_PROP_AUTO_EXPOSURE, false);
    cam1.set(CV_CAP_PROP_AUTOGRAB, false);

    BackgroundSubtractorMOG2 bgsub;
    Mat bg_mat;
    do
    {
        cam1 >> bg_mat;
        imshow("select background", bg_mat);
    } while( waitKey(33) != 32 );
    destroyWindow("select background");

    Mat fg_mat, fg_mat2, fg_mat3;
    SimpleBlobDetector::Params detector_params;
    vector<KeyPoint> keypoints;
    detector_params.minDistBetweenBlobs = 50.0f;
    detector_params.filterByInertia = false;
    detector_params.filterByConvexity = false;
    detector_params.filterByColor = false;
    detector_params.filterByCircularity = false;
    detector_params.filterByArea = true;
    detector_params.minArea = 500.0f;
    detector_params.maxArea = 640*360;
    SimpleBlobDetector detector(detector_params);
    Mat kernel = Mat();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchies;

    do
    {
        cam1 >> fg_mat;
        bgsub.operator()(fg_mat, fg_mat, 0);
        fg_mat.copyTo(fg_mat2);
//        dilate(fg_mat2, fg_mat2, kernel, Point(-1, -1), 3);
        erode(fg_mat2, fg_mat2, kernel, Point(-1, -1), 1);
        morphologyEx(fg_mat2, fg_mat2, MORPH_CLOSE, kernel);
//        dilate(fg_mat2, fg_mat2, kernel, Point(-1, -1), 3);
        threshold(fg_mat2, fg_mat2, 254, 255, CV_THRESH_BINARY);
//        detector.detect(fg_mat, keypoints);
//        drawKeypoints(fg_mat, keypoints, fg_mat, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        imshow("fg_mat2", fg_mat2);
        findContours(fg_mat2, contours, hierarchies, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        size_t nAreaCnt = contours.size();
        //_DbgStr(_T("cont=%d ,hier=%d"),nAreaCnt, hierarchy.size());
        fg_mat3 = Mat(fg_mat.size(), CV_8UC3);
        for(int i=0; i< nAreaCnt; i++){
            Scalar color = Scalar(0, 0, 255 );//랜덤 색 만들기
            int thickness = 2;//CV_FILLED=내부 채움
            drawContours( fg_mat3, contours, i, color, thickness , 8, hierarchies );//외곽선 그리기
        }
//        imshow("fg_mat2", fg_mat2);

    } while( waitKey(33) != 32 );

//
//    SimpleBlobDetector detector;
//    vector<KeyPoint> keypoints;
//
//    threshold(mat, mat, 100, 255, THRESH_BINARY_INV);
//
//    detector.detect(mat, keypoints);
//
//    Mat mat2;
//    drawKeypoints(mat, keypoints, mat2, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//
//    cout << keypoints.size() << endl;
//
//    imshow("hello", mat2);
//    waitKey(0);
    return 0;



//    cv::Mat mat;
//
//
//    mat = cv::imread("/Users/lec/Desktop/SafeDesk/FindingContours.png");
//    cv::threshold(mat, mat, 100, 255, cv::THRESH_BINARY);
//
//    CvBlobs blobs;
//    IplImage* img_label = cvCreateImage(cvSize(mat.cols, mat.rows), IPL_DEPTH_LABEL, 1);
//    //IplImage* img_data = cvCreateImage(cvSize(mat.cols, mat.rows), 8, 3);
//    IplImage* img_data = new IplImage(mat);
//    cvLabel(img_data, img_label, blobs);
//    cvRenderBlobs(img_label, blobs, img_data, img_data);
//
//    cv::namedWindow("hello", 0);
//    cvShowImage("hello", img_data);
//
//
//    cv::waitKey(0);
//
////    delete img_label;
//    delete img_data;
//    return 0;
}