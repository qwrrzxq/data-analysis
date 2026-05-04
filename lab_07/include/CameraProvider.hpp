#pragma once
#include <opencv2/opencv.hpp>

class CameraProvider {
public:
    CameraProvider(int cameraIndex);
    ~CameraProvider();
    bool isOpened() const;
    cv::Mat getFrame();

private:
    cv::VideoCapture cap;
};
