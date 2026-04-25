#include "MouseHandler.hpp"

MouseHandler::MouseHandler() : drawing(false) {}

void MouseHandler::mouseCallback(int event, int x, int y, int flags, void* userdata) {
    MouseHandler* handler = static_cast<MouseHandler*>(userdata);
    handler->onMouse(event, x, y, flags);
}

void MouseHandler::onMouse(int event, int x, int y, int flags) {
    switch (event) {
        case cv::EVENT_LBUTTONDOWN:
            drawing = true;
            startPoint = cv::Point(x, y);
            currentPoint = startPoint;
            break;

        case cv::EVENT_MOUSEMOVE:
            if (drawing) {
                currentPoint = cv::Point(x, y);
            }
            break;

        case cv::EVENT_LBUTTONUP:
            if (drawing) {
                drawing = false;
                rects.push_back({startPoint, cv::Point(x, y)});
            }
            break;
    }
}

void MouseHandler::drawOverlay(cv::Mat& frame) const {
    // Малюємо всі збережені прямокутники
    for (const auto& rect : rects) {
        cv::rectangle(frame, rect.start, rect.end, cv::Scalar(0, 255, 0), 2);
    }

    // Малюємо поточний прямокутник (під час малювання)
    if (drawing) {
        cv::rectangle(frame, startPoint, currentPoint, cv::Scalar(0, 255, 255), 2);
    }
}

void MouseHandler::clear() {
    rects.clear();
}
