#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    FrameProcessor();

    // Обробляє кадр відповідно до режиму
    cv::Mat process(const cv::Mat& frame, const KeyProcessor& keyProc);

private:
    cv::Mat applyInvert(const cv::Mat& frame);
    cv::Mat applyGrayscale(const cv::Mat& frame);
    cv::Mat applyGaussianBlur(const cv::Mat& frame);
    cv::Mat applyCanny(const cv::Mat& frame);
    cv::Mat applySobel(const cv::Mat& frame);
    cv::Mat applyBinary(const cv::Mat& frame);
    cv::Mat applyGlitch(const cv::Mat& frame);

    cv::Mat applyZoom(const cv::Mat& frame, double zoom);
    cv::Mat applyFlip(const cv::Mat& frame, bool flipH, bool flipV);
    cv::Mat applyBrightness(const cv::Mat& frame, int brightness);
    void drawInfo(cv::Mat& frame, const KeyProcessor& keyProc, int fps);

    int frameCount;
    double lastTime;
    int currentFps;
};
