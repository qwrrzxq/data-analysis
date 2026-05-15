#include <iostream>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "MouseHandler.hpp"
#include "Display.hpp"

// Callback для слайдера яскравості
void onBrightnessChange(int value, void* userdata) {
    KeyProcessor* keyProc = static_cast<KeyProcessor*>(userdata);
    keyProc->setBrightness(value - 100);  // Діапазон: -100..+100
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Lab 6 - OpenCV Video Processing" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  0-7    : Switch processing mode" << std::endl;
    std::cout << "           0=Normal, 1=Invert, 2=Gray" << std::endl;
    std::cout << "           3=Blur, 4=Canny, 5=Sobel" << std::endl;
    std::cout << "           6=Binary, 7=Glitch" << std::endl;
    std::cout << "  +/-    : Zoom in/out" << std::endl;
    std::cout << "  h      : Flip horizontal" << std::endl;
    std::cout << "  v      : Flip vertical" << std::endl;
    std::cout << "  i      : Toggle info overlay" << std::endl;
    std::cout << "  r      : Reset all settings" << std::endl;
    std::cout << "  Mouse  : Draw rectangles (LMB)" << std::endl;
    std::cout << "  Slider : Adjust brightness" << std::endl;
    std::cout << "  q/ESC  : Quit" << std::endl;
    std::cout << std::endl;

    // Створюємо всі класи
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Failed to open camera!" << std::endl;
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    MouseHandler mouseHandler;
    Display display("Lab 6 - OpenCV");

    // Реєструємо mouse callback
    cv::setMouseCallback(display.getWindowName(), MouseHandler::mouseCallback, &mouseHandler);

    // Створюємо слайдер яскравості
    int brightnessSlider = 100;  // Початкове значення (середина = 0 яскравість)
    cv::createTrackbar("Brightness", display.getWindowName(),
                       &brightnessSlider, 200, onBrightnessChange, &keyProcessor);

    // Головний цикл
    while (true) {
        // 1. Читаємо кадр з камери
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            std::cerr << "Empty frame, skipping..." << std::endl;
            continue;
        }

        // 2. Обробляємо кадр
        cv::Mat processed = frameProcessor.process(frame, keyProcessor);

        // 3. Малюємо прямокутники від миші
        mouseHandler.drawOverlay(processed);

        // 4. Відображаємо результат
        display.show(processed);

        // 5. Обробляємо натискання клавіш
        int key = cv::waitKey(16);  // ~60 FPS
        if (!keyProcessor.processKey(key)) {
            break;  // Вихід
        }

        // Очищуємо прямокутники по 'c'
        if (key == 'c' || key == 'C') {
            mouseHandler.clear();
            std::cout << "Rectangles cleared" << std::endl;
        }
    }

    std::cout << "Exiting..." << std::endl;
    return 0;
}
