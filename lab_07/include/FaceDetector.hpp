#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

class FaceDetector {
public:
    FaceDetector(const std::string& prototxt, const std::string& model,
                 float confidenceThreshold = 0.5f);
    ~FaceDetector();

    void submitFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getDetections() const;
    void setSimulateDelay(bool enabled);
    bool isSimulatingDelay() const;
    bool isLoaded() const;

private:
    void workerLoop();

    cv::dnn::Net net;
    float threshold;
    bool modelLoaded;

    std::thread worker;
    mutable std::mutex mtx;
    std::condition_variable cv_;
    std::atomic<bool> running;
    std::atomic<bool> simulateDelay;
    bool hasNewFrame;

    cv::Mat inputFrame;
    std::vector<cv::Rect> faces;
};
