#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int cameraIndex) {
    cap.open(cameraIndex);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open camera " << cameraIndex << std::endl;
    } else {
        std::cout << "Camera " << cameraIndex << " opened successfully" << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}
