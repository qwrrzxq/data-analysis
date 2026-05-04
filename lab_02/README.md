# Лабораторна робота №2

## Наука про дані: підготовчий етап

### Опис завдання

**Частина 1 (Part1_VHI.ipynb):**
- Завантаження VHI-індексу для всіх адміністративних одиниць України з NOAA через `urllib`
- Data cleaning: очищення від HTML-тегів, обробка пропусків, видалення зайвих стовпців
- Зміна індексації областей з англійської абетки (NOAA) на українську
- Формування вибірок: VHI по області/року, діапазон років, екстремуми (min/max/середнє/медіана), аналіз посух, порівняння областей, тренди

**Частина 2 (Part2_Electric_Power.ipynb):**
- Завантаження та очищення датасету Individual Household Electric Power Consumption
- Формування вибірок із фільтрацією за потужністю, силою струму, часом доби
- Нормалізація (Min-Max) та стандартизація (Z-score)
- Кореляція Пірсона та Спірмена
- One Hot Encoding категоріального атрибута
- Профілювання часу виконання через `timeit`

### Інструкція із запуску

**Вимоги:** Python 3.10+, pip

```bash
# 1. Створити віртуальне середовище
python -m venv venv

# 2. Активувати
source venv/bin/activate   # Linux/Mac
# venv\Scripts\activate    # Windows

# 3. Встановити залежності
pip install -r requirements.txt

# 4. Запустити Jupyter Notebook
jupyter notebook
```

Відкрити `Part1_VHI.ipynb` або `Part2_Electric_Power.ipynb` та виконати комірки послідовно (Shift+Enter).

### Файли

| Файл | Опис |
|------|------|
| `Part1_VHI.ipynb` | Частина 1 — VHI-індекс |
| `Part2_Electric_Power.ipynb` | Частина 2 — електроспоживання |
| `requirements.txt` | Залежності Python |
