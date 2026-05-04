#include "FaceDetector.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

FaceDetector::FaceDetector(const std::string& prototxt, const std::string& model,
                           float confidenceThreshold)
    : threshold(confidenceThreshold), modelLoaded(false),
      running(true), simulateDelay(false), hasNewFrame(false) {

    std::ifstream f1(prototxt), f2(model);
    if (!f1.good() || !f2.good()) {
        std::cerr << "Model files not found! Run ./preinstall.sh first." << std::endl;
        running = false;
        return;
    }
    f1.close();
    f2.close();

    net = cv::dnn::readNetFromCaffe(prototxt, model);
    if (net.empty()) {
        std::cerr << "Failed to load face detection model!" << std::endl;
        running = false;
        return;
    }

    modelLoaded = true;
    std::cout << "Face detection model loaded" << std::endl;
    worker = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    cv_.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
}

bool FaceDetector::isLoaded() const {
    return modelLoaded;
}

void FaceDetector::setSimulateDelay(bool enabled) {
    simulateDelay = enabled;
}

bool FaceDetector::isSimulatingDelay() const {
    return simulateDelay;
}

void FaceDetector::submitFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mtx);
    inputFrame = frame.clone();
    hasNewFrame = true;
    cv_.notify_one();
}

std::vector<cv::Rect> FaceDetector::getDetections() const {
    std::lock_guard<std::mutex> lock(mtx);
    return faces;
}

void FaceDetector::drawDetections(cv::Mat& frame) const {
    std::vector<cv::Rect> currentFaces = getDetections();
    for (const auto& face : currentFaces) {
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
        cv::putText(frame, "Face", cv::Point(face.x, face.y - 8),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    }
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat frame;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv_.wait(lock, [this] { return hasNewFrame || !running; });
            if (!running) break;
            frame = inputFrame.clone();
            hasNewFrame = false;
        }

        if (simulateDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300),
                                               cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(blob);
        cv::Mat output = net.forward();

        cv::Mat detections(output.size[2], output.size[3], CV_32F, output.ptr<float>());

        std::vector<cv::Rect> newFaces;
        for (int i = 0; i < detections.rows; i++) {
            float confidence = detections.at<float>(i, 2);
            if (confidence > threshold) {
                int x1 = static_cast<int>(detections.at<float>(i, 3) * frame.cols);
                int y1 = static_cast<int>(detections.at<float>(i, 4) * frame.rows);
                int x2 = static_cast<int>(detections.at<float>(i, 5) * frame.cols);
                int y2 = static_cast<int>(detections.at<float>(i, 6) * frame.rows);

                x1 = std::max(0, x1);
                y1 = std::max(0, y1);
                x2 = std::min(frame.cols, x2);
                y2 = std::min(frame.rows, y2);

                if (x2 > x1 && y2 > y1) {
                    newFaces.push_back(cv::Rect(x1, y1, x2 - x1, y2 - y1));
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            faces = newFaces;
        }
    }
}
