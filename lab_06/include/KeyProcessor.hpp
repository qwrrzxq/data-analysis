#pragma once
#include <opencv2/opencv.hpp>

enum class Mode {
    NORMAL,
    INVERT,
    GRAYSCALE,
    GAUSSIAN_BLUR,
    CANNY,
    SOBEL,
    BINARY,
    GLITCH,
    MODE_COUNT  // кількість режимів
};

class KeyProcessor {
public:
    KeyProcessor();

    // Обробляє натиснуту клавішу, повертає false якщо треба вийти
    bool processKey(int key);

    Mode getMode() const;
    std::string getModeName() const;

    // Параметри, керовані стрілочками
    int getOffsetX() const;
    int getOffsetY() const;
    double getZoom() const;
    int getBrightness() const;
    void setBrightness(int val);

    bool shouldFlipH() const;
    bool shouldFlipV() const;
    bool shouldShowInfo() const;

private:
    Mode currentMode;
    int offsetX, offsetY;
    double zoom;
    int brightness;
    bool flipH, flipV;
    bool showInfo;
};
