# Лабораторна робота №3

## Візуалізація даних

### Датасет

**Automobile Dataset** — UCI Machine Learning Repository

- Джерело: https://archive.ics.uci.edu/dataset/10/automobile
- Dataset Characteristics: Multivariate
- Attribute Characteristics: Categorical, Integer, Real
- Number of Instances: 205
- Number of Attributes: 26
- Has Missing Values: Yes

### Опис завдання

1. Завантаження та Data Cleaning датасету Automobile
2. Побудова графіків залежності integer/real атрибутів (scatter plot, line plot)
3. Гістограма з 5 діапазонами (розподіл цін)
4. Multi-dimensional візуалізація (pair plot, violin plot, heatmap)
5. Додаткові корисні графіки (box plot, bubble chart)
6. Всього побудовано **8 графіків**

### Інструкція із запуску

**Вимоги:** Python 3.10+, pip

```bash
# 1. Створити віртуальне середовище
python -m venv venv

# 2. Активувати (Windows)
venv\Scripts\activate

# 3. Встановити залежності
pip install -r requirements.txt

# 4. Запустити Jupyter Notebook
jupyter notebook
```

Відкрити `Lab3_Visualization.ipynb` та виконати комірки послідовно (Shift+Enter).

### Графіки

| # | Тип | Що показує |
|---|-----|-----------|
| 1 | Scatter plot | Залежність ціни від потужності |
| 2 | Line plot | Середня ціна по виробниках |
| 3 | Гістограма | Розподіл цін по 5 діапазонах |
| 4 | Pair plot | Матриця залежностей ключових атрибутів |
| 5 | Violin plot | Розподіл цін за типом кузова |
| 6 | Heatmap | Кореляційна матриця |
| 7 | Box plot | Потужність за типом приводу |
| 8 | Bubble chart | Витрата палива vs Ціна vs Потужність |
