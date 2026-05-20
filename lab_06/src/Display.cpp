#include "Display.hpp"

Display::Display(const std::string& windowName) : windowName(windowName) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

Display::~Display() {
    cv::destroyWindow(windowName);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow(windowName, frame);
    }
}

std::string Display::getWindowName() const {
    return windowName;
}
