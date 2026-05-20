"""
Лабораторна робота №4 — Візуалізація даних 2
Інтерактивна програма для візуалізації гармоніки з шумом та фільтрацією.

Інструкція для користувача:
===========================
1. Запустіть програму: python lab4_harmonic.py
2. Використовуйте слайдери для зміни параметрів:
   - Amplitude — амплітуда гармоніки
   - Frequency — частота гармоніки (ω)
   - Phase — фазовий зсув (φ)
   - Noise Mean — середнє значення шуму
   - Noise Covariance — дисперсія шуму
   - Cutoff Frequency — частота зрізу фільтра
3. Чекбокс "Show Noise" — вмикає/вимикає відображення шуму
4. Кнопка "Reset" — відновлює початкові параметри
5. На графіку відображаються:
   - Оранжева лінія — зашумлена гармоніка (якщо Show Noise увімкнено)
   - Червона лінія — чиста гармоніка
   - Синя пунктирна — відфільтрована гармоніка (результат фільтрації)
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy.signal import butter, filtfilt


# ==================== Параметри ====================
# Часовий діапазон
t = np.linspace(0, 10, 1000)

# Початкові значення параметрів
INIT_AMPLITUDE = 1.0
INIT_FREQUENCY = 0.5
INIT_PHASE = 0.0
INIT_NOISE_MEAN = 0.0
INIT_NOISE_COVARIANCE = 0.2
INIT_CUTOFF_FREQ = 3.0
INIT_SHOW_NOISE = True

# Генеруємо початковий шум (зберігаємо окремо, щоб не перегенеровувати)
current_noise = np.random.normal(INIT_NOISE_MEAN, np.sqrt(INIT_NOISE_COVARIANCE), len(t))


# ==================== Функції ====================

def harmonic_with_noise(t, amplitude, frequency, phase, noise_mean,
                        noise_covariance, show_noise):
    """
    Генерує гармоніку y(t) = A * sin(ωt + φ) з можливим накладенням шуму.

    Параметри:
    ----------
    t : array — часовий ряд
    amplitude : float — амплітуда гармоніки (A)
    frequency : float — частота гармоніки (ω)
    phase : float — фазовий зсув (φ)
    noise_mean : float — середнє значення шуму
    noise_covariance : float — дисперсія шуму
    show_noise : bool — чи показувати шум

    Повертає:
    ---------
    clean : array — чиста гармоніка
    noisy : array — гармоніка з шумом (або чиста, якщо show_noise=False)
    """
    clean = amplitude * np.sin(2 * np.pi * frequency * t + phase)

    if show_noise:
        noisy = clean + current_noise
    else:
        noisy = clean.copy()

    return clean, noisy


def apply_filter(signal, cutoff_freq, fs=100, order=4):
    """
    Застосовує фільтр Баттерворта (lowpass) до сигналу.

    Параметри:
    ----------
    signal : array — вхідний сигнал
    cutoff_freq : float — частота зрізу (Гц)
    fs : float — частота дискретизації
    order : int — порядок фільтра

    Повертає:
    ---------
    filtered : array — відфільтрований сигнал
    """
    nyquist = fs / 2
    # Обмежуємо cutoff щоб не перевищував частоту Найквіста
    normalized_cutoff = min(cutoff_freq / nyquist, 0.99)
    b, a = butter(order, normalized_cutoff, btype='low')
    filtered = filtfilt(b, a, signal)
    return filtered


# ==================== Створення інтерфейсу ====================

# Створюємо фігуру та осі для графіка
fig, ax = plt.subplots(figsize=(12, 7))
fig.subplots_adjust(left=0.1, bottom=0.45, right=0.95, top=0.95)
fig.canvas.manager.set_window_title('Лабораторна робота №4 — Гармоніка з шумом та фільтрацією')

# Початкові дані
clean_signal, noisy_signal = harmonic_with_noise(
    t, INIT_AMPLITUDE, INIT_FREQUENCY, INIT_PHASE,
    INIT_NOISE_MEAN, INIT_NOISE_COVARIANCE, INIT_SHOW_NOISE
)
filtered_signal = apply_filter(noisy_signal, INIT_CUTOFF_FREQ)

# Малюємо початкові графіки
line_noisy, = ax.plot(t, noisy_signal, color='orange', alpha=0.7,
                      linewidth=0.8, label='Зашумлена гармоніка')
line_clean, = ax.plot(t, clean_signal, color='red', linewidth=2,
                      label='Чиста гармоніка')
line_filtered, = ax.plot(t, filtered_signal, color='blue', linewidth=2,
                         linestyle='--', label='Відфільтрована')

ax.set_xlim(0, 10)
ax.set_ylim(-2.0, 2.0)
ax.set_xlabel('Час (t)')
ax.set_ylabel('Амплітуда')
ax.set_title('Гармоніка з шумом та фільтрацією (Баттерворт)')
ax.legend(loc='upper right')
ax.grid(True, alpha=0.3)


# ==================== Слайдери ====================

# Кольори слайдерів
slider_color = '#3498db'
slider_bg = '#ecf0f1'

# Амплітуда
ax_amp = fig.add_axes([0.15, 0.33, 0.55, 0.025])
slider_amp = Slider(ax_amp, 'Amplitude', 0.1, 2.0,
                    valinit=INIT_AMPLITUDE, color=slider_color)

# Частота
ax_freq = fig.add_axes([0.15, 0.28, 0.55, 0.025])
slider_freq = Slider(ax_freq, 'Frequency', 0.05, 2.0,
                     valinit=INIT_FREQUENCY, color=slider_color)

# Фаза
ax_phase = fig.add_axes([0.15, 0.23, 0.55, 0.025])
slider_phase = Slider(ax_phase, 'Phase', 0.0, 2 * np.pi,
                      valinit=INIT_PHASE, color=slider_color)

# Середнє шуму
ax_noise_mean = fig.add_axes([0.15, 0.16, 0.55, 0.025])
slider_noise_mean = Slider(ax_noise_mean, 'Noise Mean', -1.0, 1.0,
                           valinit=INIT_NOISE_MEAN, color='#e74c3c')

# Дисперсія шуму
ax_noise_cov = fig.add_axes([0.15, 0.11, 0.55, 0.025])
slider_noise_cov = Slider(ax_noise_cov, 'Noise Covariance', 0.0, 1.0,
                          valinit=INIT_NOISE_COVARIANCE, color='#e74c3c')

# Частота зрізу фільтра
ax_cutoff = fig.add_axes([0.15, 0.06, 0.55, 0.025])
slider_cutoff = Slider(ax_cutoff, 'Cutoff Frequency', 0.5, 10.0,
                       valinit=INIT_CUTOFF_FREQ, color='#2ecc71')


# ==================== Чекбокс та кнопка ====================

# Чекбокс "Show Noise"
ax_checkbox = fig.add_axes([0.8, 0.05, 0.15, 0.07])
checkbox = CheckButtons(ax_checkbox, ['Show Noise'], [INIT_SHOW_NOISE])

# Кнопка "Reset"
ax_reset = fig.add_axes([0.8, 0.14, 0.1, 0.04])
button_reset = Button(ax_reset, 'Reset', color=slider_bg, hovercolor='#bdc3c7')


# ==================== Логіка оновлення ====================

# Зберігаємо попередні значення шуму для порівняння
prev_noise_params = {'mean': INIT_NOISE_MEAN, 'cov': INIT_NOISE_COVARIANCE}
show_noise_flag = [INIT_SHOW_NOISE]  # список щоб можна було змінювати в closure


def update_harmonic(val):
    """Оновлює графік при зміні параметрів гармоніки (без перегенерації шуму)."""
    clean, noisy = harmonic_with_noise(
        t, slider_amp.val, slider_freq.val, slider_phase.val,
        slider_noise_mean.val, slider_noise_cov.val, show_noise_flag[0]
    )
    filtered = apply_filter(noisy if show_noise_flag[0] else clean + current_noise,
                            slider_cutoff.val)

    line_clean.set_ydata(clean)
    line_noisy.set_ydata(noisy)
    line_filtered.set_ydata(filtered)

    # Автомасштабування по Y
    all_vals = np.concatenate([clean, noisy, filtered])
    y_min, y_max = np.min(all_vals) - 0.2, np.max(all_vals) + 0.2
    ax.set_ylim(y_min, y_max)

    fig.canvas.draw_idle()


def update_noise(val):
    """Оновлює шум при зміні параметрів шуму (перегенерація шуму)."""
    global current_noise

    new_mean = slider_noise_mean.val
    new_cov = slider_noise_cov.val

    # Перегенеровуємо шум тільки якщо параметри шуму змінились
    if (new_mean != prev_noise_params['mean'] or
            new_cov != prev_noise_params['cov']):
        current_noise = np.random.normal(new_mean, np.sqrt(max(new_cov, 0.001)), len(t))
        prev_noise_params['mean'] = new_mean
        prev_noise_params['cov'] = new_cov

    update_harmonic(val)


def toggle_noise(label):
    """Перемикає відображення шуму."""
    show_noise_flag[0] = not show_noise_flag[0]
    line_noisy.set_visible(show_noise_flag[0])
    update_harmonic(None)


def reset(event):
    """Відновлює початкові параметри."""
    global current_noise

    slider_amp.reset()
    slider_freq.reset()
    slider_phase.reset()
    slider_noise_mean.reset()
    slider_noise_cov.reset()
    slider_cutoff.reset()

    # Відновлюємо шум
    current_noise = np.random.normal(INIT_NOISE_MEAN,
                                     np.sqrt(INIT_NOISE_COVARIANCE), len(t))
    prev_noise_params['mean'] = INIT_NOISE_MEAN
    prev_noise_params['cov'] = INIT_NOISE_COVARIANCE

    # Відновлюємо чекбокс
    if not show_noise_flag[0]:
        show_noise_flag[0] = True
        line_noisy.set_visible(True)
        # Оновлюємо стан чекбокса візуально
        checkbox.set_active(0)

    update_harmonic(None)


# ==================== Підключення обробників ====================

# Параметри гармоніки — НЕ перегенеровують шум
slider_amp.on_changed(update_harmonic)
slider_freq.on_changed(update_harmonic)
slider_phase.on_changed(update_harmonic)

# Параметри шуму — перегенеровують шум
slider_noise_mean.on_changed(update_noise)
slider_noise_cov.on_changed(update_noise)

# Частота зрізу фільтра
slider_cutoff.on_changed(update_harmonic)

# Чекбокс та кнопка
checkbox.on_clicked(toggle_noise)
button_reset.on_clicked(reset)


# ==================== Запуск ====================
print("=" * 60)
print("  Лабораторна робота №4 — Гармоніка з шумом та фільтрацією")
print("=" * 60)
print()
print("Інструкція:")
print("  - Слайдери: змінюйте параметри гармоніки, шуму та фільтра")
print("  - Чекбокс 'Show Noise': вмикає/вимикає шум")
print("  - Кнопка 'Reset': відновлює початкові параметри")
print("  - Червона лінія: чиста гармоніка")
print("  - Оранжева лінія: зашумлена гармоніка")
print("  - Синя пунктирна: відфільтрована (Баттерворт lowpass)")
print()

plt.show()
