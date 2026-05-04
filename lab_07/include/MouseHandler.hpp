#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class MouseHandler {
public:
    MouseHandler();
    static void mouseCallback(int event, int x, int y, int flags, void* userdata);
    void drawOverlay(cv::Mat& frame) const;
    void clear();

private:
    struct Rect {
        cv::Point start;
        cv::Point end;
    };

    void onMouse(int event, int x, int y, int flags);
    bool drawing;
    cv::Point startPoint;
    cv::Point currentPoint;
    std::vector<Rect> rects;
};
