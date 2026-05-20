# Лабораторна робота №6

## C++ OpenCV — Обробка відео з камери

### Опис

Програма на C++ з використанням OpenCV, яка читає відео з камери в реальному часі та застосовує різні режими обробки зображень. Керування через клавіатуру та мишу.

### Вимоги до системи

- **ОС:** Ubuntu 20.04+ / Debian 11+
- **Компілятор:** GCC 9+ (g++)
- **CMake:** 3.10+
- **OpenCV:** 4.x (встановлюється через `preinstall.sh`)
- **Камера:** веб-камера (USB або вбудована)

### Інструкція із запуску

```bash
# 1. Встановити залежності
./preinstall.sh

# 2. Зібрати проєкт
./build.sh

# 3. Запустити програму
./run.sh
```

### Керування

| Клавіша | Дія |
|---------|-----|
| `0` | Нормальний режим |
| `1` | Інверсія кольорів |
| `2` | Градації сірого |
| `3` | Gaussian Blur |
| `4` | Canny Edge Detection |
| `5` | Фільтр Собеля |
| `6` | Бінаризація |
| `7` | Glitch Effect (зсув каналів RGB) |
| `+` / `-` | Зум |
| `h` | Горизонтальне відображення |
| `v` | Вертикальне відображення |
| `i` | Показати/сховати інформаційний оверлей |
| `r` | Скидання всіх налаштувань |
| `c` | Очистити намальовані прямокутники |
| `q` / `ESC` | Вихід |
| **ЛКМ** | Малювання прямокутників |
| **Slider** | Яскравість |

### Архітектура

| Клас | Файл | Опис |
|------|------|------|
| `CameraProvider` | CameraProvider.hpp/cpp | Захоплення відео з камери |
| `KeyProcessor` | KeyProcessor.hpp/cpp | Обробка клавіатури, enum режимів |
| `FrameProcessor` | FrameProcessor.hpp/cpp | Обробка зображень (8 режимів) |
| `MouseHandler` | MouseHandler.hpp/cpp | Обробка миші, малювання прямокутників |
| `Display` | Display.hpp/cpp | Відображення результату |

### Файлова структура

```
lab_06/
├── CMakeLists.txt
├── preinstall.sh
├── build.sh
├── run.sh
├── README.md
├── include/
│   ├── CameraProvider.hpp
│   ├── KeyProcessor.hpp
│   ├── FrameProcessor.hpp
│   ├── MouseHandler.hpp
│   └── Display.hpp
├── src/
│   ├── main.cpp
│   ├── CameraProvider.cpp
│   ├── KeyProcessor.cpp
│   ├── FrameProcessor.cpp
│   ├── MouseHandler.cpp
│   └── Display.cpp
└── build/         (генерується автоматично)
```
