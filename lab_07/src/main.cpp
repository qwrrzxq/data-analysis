#include <iostream>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "MouseHandler.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"

void onBrightnessChange(int value, void* userdata) {
    KeyProcessor* keyProc = static_cast<KeyProcessor*>(userdata);
    keyProc->setBrightness(value - 100);
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Lab 7 - OpenCV Face Detection" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  0-7  : Switch processing mode" << std::endl;
    std::cout << "  f    : Toggle face detection" << std::endl;
    std::cout << "  d    : Toggle delay simulation (500ms)" << std::endl;
    std::cout << "  +/-  : Zoom in/out" << std::endl;
    std::cout << "  h/v  : Flip horizontal/vertical" << std::endl;
    std::cout << "  i    : Toggle info overlay" << std::endl;
    std::cout << "  r    : Reset all settings" << std::endl;
    std::cout << "  q/ESC: Quit" << std::endl;
    std::cout << std::endl;

    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Failed to open camera!" << std::endl;
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    MouseHandler mouseHandler;
    Display display("Lab 7 - Face Detection");

    FaceDetector faceDetector("models/deploy.prototxt",
                              "models/res10_300x300_ssd_iter_140000.caffemodel");

    if (!faceDetector.isLoaded()) {
        std::cerr << "Warning: face detection unavailable" << std::endl;
    }

    cv::setMouseCallback(display.getWindowName(), MouseHandler::mouseCallback, &mouseHandler);

    int brightnessSlider = 100;
    cv::createTrackbar("Brightness", display.getWindowName(),
                       &brightnessSlider, 200, onBrightnessChange, &keyProcessor);

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            std::cerr << "Empty frame, skipping..." << std::endl;
            continue;
        }

        if (keyProcessor.shouldDetectFaces() && faceDetector.isLoaded()) {
            faceDetector.setSimulateDelay(keyProcessor.shouldSimulateDelay());
            faceDetector.submitFrame(frame);
        }

        cv::Mat processed = frameProcessor.process(frame, keyProcessor);

        if (keyProcessor.shouldDetectFaces() && faceDetector.isLoaded()) {
            faceDetector.drawDetections(processed);
        }

        mouseHandler.drawOverlay(processed);
        display.show(processed);

        int key = cv::waitKey(16);
        if (!keyProcessor.processKey(key)) {
            break;
        }

        if (key == 'c' || key == 'C') {
            mouseHandler.clear();
            std::cout << "Rectangles cleared" << std::endl;
        }
    }

    std::cout << "Exiting..." << std::endl;
    return 0;
}
