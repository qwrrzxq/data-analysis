#pragma once
#include <string>

enum class Mode {
    NORMAL, INVERT, GRAYSCALE, GAUSSIAN_BLUR,
    CANNY, SOBEL, BINARY, GLITCH
};

class KeyProcessor {
public:
    KeyProcessor();
    bool processKey(int key);
    Mode getMode() const;
    std::string getModeName() const;
    int getOffsetX() const;
    int getOffsetY() const;
    double getZoom() const;
    int getBrightness() const;
    void setBrightness(int val);
    bool shouldFlipH() const;
    bool shouldFlipV() const;
    bool shouldShowInfo() const;
    bool shouldDetectFaces() const;
    bool shouldSimulateDelay() const;

private:
    Mode currentMode;
    int offsetX, offsetY;
    double zoom;
    int brightness;
    bool flipH, flipV, showInfo;
    bool detectFaces;
    bool simulateDelay;
};
