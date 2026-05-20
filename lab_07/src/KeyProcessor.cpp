#include "KeyProcessor.hpp"
#include <iostream>

KeyProcessor::KeyProcessor()
    : currentMode(Mode::NORMAL), offsetX(0), offsetY(0),
      zoom(1.0), brightness(0), flipH(false), flipV(false),
      showInfo(true), detectFaces(false), simulateDelay(false) {}

bool KeyProcessor::processKey(int key) {
    if (key == -1) return true;

    switch (key) {
        case 27:
        case 'q':
        case 'Q':
            return false;

        case '0': currentMode = Mode::NORMAL;        std::cout << "Mode: Normal" << std::endl; break;
        case '1': currentMode = Mode::INVERT;        std::cout << "Mode: Invert" << std::endl; break;
        case '2': currentMode = Mode::GRAYSCALE;     std::cout << "Mode: Grayscale" << std::endl; break;
        case '3': currentMode = Mode::GAUSSIAN_BLUR; std::cout << "Mode: Gaussian Blur" << std::endl; break;
        case '4': currentMode = Mode::CANNY;         std::cout << "Mode: Canny" << std::endl; break;
        case '5': currentMode = Mode::SOBEL;         std::cout << "Mode: Sobel" << std::endl; break;
        case '6': currentMode = Mode::BINARY;        std::cout << "Mode: Binary" << std::endl; break;
        case '7': currentMode = Mode::GLITCH;        std::cout << "Mode: Glitch" << std::endl; break;

        case 'f':
        case 'F':
            detectFaces = !detectFaces;
            std::cout << "Face detection: " << (detectFaces ? "ON" : "OFF") << std::endl;
            break;

        case 'd':
        case 'D':
            simulateDelay = !simulateDelay;
            std::cout << "Simulate delay (500ms): " << (simulateDelay ? "ON" : "OFF") << std::endl;
            break;

        case 2:  offsetX -= 10; break;
        case 3:  offsetX += 10; break;
        case 0:  offsetY -= 10; break;
        case 1:  offsetY += 10; break;

        case '+':
        case '=':
            zoom = std::min(zoom + 0.1, 3.0); break;
        case '-':
        case '_':
            zoom = std::max(zoom - 0.1, 0.3); break;

        case 'h': flipH = !flipH; std::cout << "Flip H: " << (flipH ? "ON" : "OFF") << std::endl; break;
        case 'v': flipV = !flipV; std::cout << "Flip V: " << (flipV ? "ON" : "OFF") << std::endl; break;
        case 'i': showInfo = !showInfo; std::cout << "Info: " << (showInfo ? "ON" : "OFF") << std::endl; break;

        case 'r':
        case 'R':
            currentMode = Mode::NORMAL;
            offsetX = offsetY = 0;
            zoom = 1.0;
            brightness = 0;
            flipH = flipV = false;
            showInfo = true;
            detectFaces = false;
            simulateDelay = false;
            std::cout << "Reset to defaults" << std::endl;
            break;
    }
    return true;
}

Mode KeyProcessor::getMode() const { return currentMode; }

std::string KeyProcessor::getModeName() const {
    switch (currentMode) {
        case Mode::NORMAL:        return "Normal";
        case Mode::INVERT:        return "Invert";
        case Mode::GRAYSCALE:     return "Grayscale";
        case Mode::GAUSSIAN_BLUR: return "Gaussian Blur";
        case Mode::CANNY:         return "Canny Edge";
        case Mode::SOBEL:         return "Sobel Filter";
        case Mode::BINARY:        return "Binary";
        case Mode::GLITCH:        return "Glitch";
        default:                  return "Unknown";
    }
}

int KeyProcessor::getOffsetX() const { return offsetX; }
int KeyProcessor::getOffsetY() const { return offsetY; }
double KeyProcessor::getZoom() const { return zoom; }
int KeyProcessor::getBrightness() const { return brightness; }
void KeyProcessor::setBrightness(int val) { brightness = val; }
bool KeyProcessor::shouldFlipH() const { return flipH; }
bool KeyProcessor::shouldFlipV() const { return flipV; }
bool KeyProcessor::shouldShowInfo() const { return showInfo; }
bool KeyProcessor::shouldDetectFaces() const { return detectFaces; }
bool KeyProcessor::shouldSimulateDelay() const { return simulateDelay; }
