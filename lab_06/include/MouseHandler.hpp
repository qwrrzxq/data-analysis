#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

struct DrawnRect {
    cv::Point start;
    cv::Point end;
};

class MouseHandler {
public:
    MouseHandler();

    // Статичний callback для OpenCV
    static void mouseCallback(int event, int x, int y, int flags, void* userdata);

    // Малює всі прямокутники на кадрі
    void drawOverlay(cv::Mat& frame) const;

    // Очищає всі прямокутники
    void clear();

private:
    void onMouse(int event, int x, int y, int flags);

    bool drawing;
    cv::Point startPoint;
    cv::Point currentPoint;
    std::vector<DrawnRect> rects;
};
