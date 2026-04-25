#include "FrameProcessor.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

FrameProcessor::FrameProcessor()
    : frameCount(0), lastTime(0), currentFps(0) {
    lastTime = static_cast<double>(cv::getTickCount());
}

cv::Mat FrameProcessor::process(const cv::Mat& frame, const KeyProcessor& keyProc) {
    if (frame.empty()) return frame;

    cv::Mat result = frame.clone();

    // Застосовуємо режим обробки
    switch (keyProc.getMode()) {
        case Mode::INVERT:        result = applyInvert(result); break;
        case Mode::GRAYSCALE:     result = applyGrayscale(result); break;
        case Mode::GAUSSIAN_BLUR: result = applyGaussianBlur(result); break;
        case Mode::CANNY:         result = applyCanny(result); break;
        case Mode::SOBEL:         result = applySobel(result); break;
        case Mode::BINARY:        result = applyBinary(result); break;
        case Mode::GLITCH:        result = applyGlitch(result); break;
        case Mode::NORMAL:
        default: break;
    }

    // Яскравість
    result = applyBrightness(result, keyProc.getBrightness());

    // Відображення (flip)
    result = applyFlip(result, keyProc.shouldFlipH(), keyProc.shouldFlipV());

    // Зум
    if (keyProc.getZoom() != 1.0) {
        result = applyZoom(result, keyProc.getZoom());
    }

    // Інформаційний оверлей
    if (keyProc.shouldShowInfo()) {
        drawInfo(result, keyProc, currentFps);
    }

    // Оновлюємо FPS лічильник
    frameCount++;
    double currentTime = static_cast<double>(cv::getTickCount());
    double elapsed = (currentTime - lastTime) / cv::getTickFrequency();
    if (elapsed >= 1.0) {
        currentFps = static_cast<int>(frameCount / elapsed);
        frameCount = 0;
        lastTime = currentTime;
    }

    return result;
}

cv::Mat FrameProcessor::applyInvert(const cv::Mat& frame) {
    cv::Mat result;
    cv::bitwise_not(frame, result);
    return result;
}

cv::Mat FrameProcessor::applyGrayscale(const cv::Mat& frame) {
    cv::Mat gray, result;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, result, cv::COLOR_GRAY2BGR);
    return result;
}

cv::Mat FrameProcessor::applyGaussianBlur(const cv::Mat& frame) {
    cv::Mat result;
    cv::GaussianBlur(frame, result, cv::Size(15, 15), 0);
    return result;
}

cv::Mat FrameProcessor::applyCanny(const cv::Mat& frame) {
    cv::Mat gray, edges, result;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 50, 150);
    cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
    return result;
}

cv::Mat FrameProcessor::applySobel(const cv::Mat& frame) {
    cv::Mat gray, sobelX, sobelY, result;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Sobel(gray, sobelX, CV_16S, 1, 0, 3);
    cv::Sobel(gray, sobelY, CV_16S, 0, 1, 3);

    cv::Mat absX, absY;
    cv::convertScaleAbs(sobelX, absX);
    cv::convertScaleAbs(sobelY, absY);

    cv::Mat combined;
    cv::addWeighted(absX, 0.5, absY, 0.5, 0, combined);
    cv::cvtColor(combined, result, cv::COLOR_GRAY2BGR);
    return result;
}

cv::Mat FrameProcessor::applyBinary(const cv::Mat& frame) {
    cv::Mat gray, binary, result;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY);
    cv::cvtColor(binary, result, cv::COLOR_GRAY2BGR);
    return result;
}

cv::Mat FrameProcessor::applyGlitch(const cv::Mat& frame) {
    cv::Mat result = frame.clone();
    std::vector<cv::Mat> channels;
    cv::split(result, channels);

    int shift = 10;
    // Зсуваємо R канал вправо
    cv::Mat shiftedR = cv::Mat::zeros(channels[2].size(), channels[2].type());
    channels[2](cv::Rect(0, 0, channels[2].cols - shift, channels[2].rows))
        .copyTo(shiftedR(cv::Rect(shift, 0, channels[2].cols - shift, channels[2].rows)));
    channels[2] = shiftedR;

    // Зсуваємо B канал вліво
    cv::Mat shiftedB = cv::Mat::zeros(channels[0].size(), channels[0].type());
    channels[0](cv::Rect(shift, 0, channels[0].cols - shift, channels[0].rows))
        .copyTo(shiftedB(cv::Rect(0, 0, channels[0].cols - shift, channels[0].rows)));
    channels[0] = shiftedB;

    cv::merge(channels, result);
    return result;
}

cv::Mat FrameProcessor::applyZoom(const cv::Mat& frame, double zoom) {
    int newW = static_cast<int>(frame.cols * zoom);
    int newH = static_cast<int>(frame.rows * zoom);

    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(newW, newH));

    // Обрізаємо до оригінального розміру (центр)
    int startX = std::max(0, (newW - frame.cols) / 2);
    int startY = std::max(0, (newH - frame.rows) / 2);
    int cropW = std::min(frame.cols, newW);
    int cropH = std::min(frame.rows, newH);

    cv::Mat result = cv::Mat::zeros(frame.size(), frame.type());
    cv::Rect srcRect(startX, startY, std::min(cropW, resized.cols - startX), std::min(cropH, resized.rows - startY));
    cv::Rect dstRect(0, 0, srcRect.width, srcRect.height);

    if (srcRect.width > 0 && srcRect.height > 0) {
        resized(srcRect).copyTo(result(dstRect));
    }

    return result;
}

cv::Mat FrameProcessor::applyFlip(const cv::Mat& frame, bool flipH, bool flipV) {
    cv::Mat result = frame.clone();
    if (flipH && flipV) {
        cv::flip(result, result, -1);  // Both
    } else if (flipH) {
        cv::flip(result, result, 1);   // Horizontal
    } else if (flipV) {
        cv::flip(result, result, 0);   // Vertical
    }
    return result;
}

cv::Mat FrameProcessor::applyBrightness(const cv::Mat& frame, int brightness) {
    if (brightness == 0) return frame;
    cv::Mat result;
    frame.convertTo(result, -1, 1.0, brightness);
    return result;
}

void FrameProcessor::drawInfo(cv::Mat& frame, const KeyProcessor& keyProc, int fps) {
    int y = 25;
    int lineH = 22;
    cv::Scalar color(0, 255, 0);
    double scale = 0.55;
    int thickness = 1;

    auto putText = [&](const std::string& text) {
        cv::putText(frame, text, cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX,
                    scale, cv::Scalar(0, 0, 0), thickness + 2);  // shadow
        cv::putText(frame, text, cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX,
                    scale, color, thickness);
        y += lineH;
    };

    putText("FPS: " + std::to_string(fps));
    putText("Mode: " + keyProc.getModeName());
    putText("Zoom: " + std::to_string(keyProc.getZoom()).substr(0, 4) + "x");
    putText("Brightness: " + std::to_string(keyProc.getBrightness()));
    putText("Flip H:" + std::string(keyProc.shouldFlipH() ? "ON" : "OFF") +
            " V:" + std::string(keyProc.shouldFlipV() ? "ON" : "OFF"));

    // Середня інтенсивність пікселів по каналах
    cv::Scalar mean = cv::mean(frame);
    std::ostringstream ss;
    ss << "Mean RGB: " << std::fixed << std::setprecision(1)
       << mean[2] << " / " << mean[1] << " / " << mean[0];
    putText(ss.str());

    // Підказка
    y = frame.rows - 15;
    cv::putText(frame, "Keys: 0-7 modes | +/- zoom | h/v flip | i info | r reset | q quit",
                cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(200, 200, 200), 1);
}
