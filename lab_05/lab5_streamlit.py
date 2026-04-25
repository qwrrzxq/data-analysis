"""
Лабораторна робота №5 — Наука про дані: обмін результатами та початковий аналіз
Streamlit веб-додаток для аналізу VHI-даних по областях України.

Запуск: streamlit run lab5_streamlit.py
"""

import streamlit as st
import pandas as pd
import numpy as np
import os
import glob
import re
import plotly.express as px
import plotly.graph_objects as go
from io import StringIO
import urllib.request
from datetime import datetime


# ==================== Конфігурація сторінки ====================
st.set_page_config(
    page_title="VHI Аналіз — Лаб 5",
    page_icon="🌿",
    layout="wide"
)


# ==================== Маппінги областей ====================
NOAA_ID_TO_NAME = {
    1: "Cherkasy", 2: "Chernihiv", 3: "Chernivtsi", 4: "Crimea",
    5: "Dnipropetrovsk", 6: "Donetsk", 7: "Ivano-Frankivsk",
    8: "Kharkiv", 9: "Kherson", 10: "Khmelnytskyi",
    11: "Kyiv", 12: "Kyiv City", 13: "Kirovohrad", 14: "Luhansk",
    15: "Lviv", 16: "Mykolaiv", 17: "Odessa", 18: "Poltava",
    19: "Rivne", 20: "Sevastopol", 21: "Sumy", 22: "Ternopil",
    23: "Transcarpathia", 24: "Vinnytsia", 25: "Volyn",
    26: "Zaporizhzhia", 27: "Zhytomyr"
}

UKR_INDEX = {
    1: ("Вінницька", 24), 2: ("Волинська", 25), 3: ("Дніпропетровська", 5),
    4: ("Донецька", 6), 5: ("Житомирська", 27), 6: ("Закарпатська", 23),
    7: ("Запорізька", 26), 8: ("Івано-Франківська", 7), 9: ("Київська", 11),
    10: ("Кіровоградська", 13), 11: ("Кримська", 4), 12: ("Луганська", 14),
    13: ("Львівська", 15), 14: ("Миколаївська", 16), 15: ("Одеська", 17),
    16: ("Полтавська", 18), 17: ("Рівненська", 19), 18: ("Сумська", 21),
    19: ("Тернопільська", 22), 20: ("Харківська", 8), 21: ("Херсонська", 9),
    22: ("Хмельницька", 10), 23: ("Черкаська", 1), 24: ("Чернівецька", 3),
    25: ("Чернігівська", 2), 26: ("м. Київ", 12), 27: ("м. Севастополь", 20),
}

NOAA_TO_UKR = {noaa_id: (ukr_idx, name) for ukr_idx, (name, noaa_id) in UKR_INDEX.items()}

# Список областей для dropdown
PROVINCE_LIST = {ukr_idx: name for ukr_idx, (name, _) in UKR_INDEX.items()}


# ==================== Завантаження та парсинг даних ====================

@st.cache_data(show_spinner="Завантажуємо VHI-дані...")
def load_all_data():
    """Завантажує та парсить VHI-дані для всіх областей."""
    DATA_DIR = "data"
    os.makedirs(DATA_DIR, exist_ok=True)

    all_data = []

    for noaa_id in range(1, 28):
        # Перевіряємо чи файл вже існує
        existing = glob.glob(os.path.join(DATA_DIR, f"vhi_province_{noaa_id}_*.csv"))

        if existing:
            filepath = existing[0]
        else:
            # Завантажуємо
            url = (f"https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php?"
                   f"country=UKR&provinceID={noaa_id}&year1=1981&year2=2024&type=Mean")
            now = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
            filepath = os.path.join(DATA_DIR, f"vhi_province_{noaa_id}_{now}.csv")
            try:
                urllib.request.urlretrieve(url, filepath)
            except Exception as e:
                st.warning(f"Не вдалось завантажити область {noaa_id}: {e}")
                continue

        # Парсимо файл
        df = parse_vhi_file(filepath, noaa_id)
        if df is not None and not df.empty:
            all_data.append(df)

    if all_data:
        return pd.concat(all_data, ignore_index=True)
    return pd.DataFrame()


def parse_vhi_file(filepath, noaa_province_id):
    """Парсить один VHI-файл з NOAA."""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()

        content = content.replace('<pre>', '').replace('</pre>', '')
        content = re.sub(r'<[^>]+>', '', content)

        lines = content.strip().split('\n')
        header = "Year,Week,SMN,SMT,VCI,TCI,VHI"
        data_lines = [header]

        for line in lines:
            line = line.strip()
            if not line or 'Year' in line or 'Week' in line:
                continue
            if any(c.isalpha() for c in line.replace(',', '').strip()):
                continue
            line = line.rstrip(',').strip()
            parts = [p.strip() for p in line.split(',')]
            if len(parts) >= 7:
                data_lines.append(','.join(parts[:7]))

        csv_text = '\n'.join(data_lines)
        df = pd.read_csv(StringIO(csv_text))

        for col in df.columns:
            df[col] = pd.to_numeric(df[col], errors='coerce')

        df = df.replace(-1, pd.NA).replace(-1.0, pd.NA)
        df = df.dropna(subset=['Year', 'Week'])
        df['Year'] = df['Year'].astype(int)
        df['Week'] = df['Week'].astype(int)

        ukr_idx, ukr_name = NOAA_TO_UKR.get(noaa_province_id, (noaa_province_id, "Unknown"))
        df['NOAA_ID'] = noaa_province_id
        df['Province_ID'] = ukr_idx
        df['Province_Name'] = ukr_name

        return df
    except Exception:
        return None


# ==================== Головний додаток ====================

st.title("🌿 Аналіз VHI-індексу по областях України")
st.markdown("**Лабораторна робота №5** — Наука про дані: обмін результатами")

# Завантажуємо дані
vhi_df = load_all_data()

if vhi_df.empty:
    st.error("Не вдалось завантажити дані. Перевірте з'єднання з інтернетом.")
    st.stop()

# ==================== Початкові значення ====================
min_year = int(vhi_df['Year'].min())
max_year = int(vhi_df['Year'].max())

DEFAULTS = {
    "index_select": "VHI",
    "province_select": "Вінницька",
    "week_slider": (1, 52),
    "year_slider": (2000, max_year),
    "sort_asc": False,
    "sort_desc": False,
}

# Ініціалізуємо session_state початковими значеннями
for key, val in DEFAULTS.items():
    if key not in st.session_state:
        st.session_state[key] = val


def reset_filters():
    """Скидає всі фільтри до початкових значень."""
    for key, val in DEFAULTS.items():
        st.session_state[key] = val


# ==================== Layout: sidebar (фільтри) + main (контент) ====================

# --- ЛІВА КОЛОНКА: Інтерактивні елементи ---
left_col, right_col = st.columns([1, 3])

with left_col:
    st.header("⚙️ Фільтри")

    # Dropdown: вибір індексу (VCI, TCI, VHI)
    index_options = ["VHI", "VCI", "TCI"]
    selected_index = st.selectbox("Оберіть індекс:", index_options,
                                  key="index_select")

    # Dropdown: вибір області
    province_options = {v: k for k, v in PROVINCE_LIST.items()}
    province_names_sorted = sorted(province_options.keys())
    selected_province_name = st.selectbox(
        "Оберіть область:",
        province_names_sorted,
        key="province_select"
    )
    selected_province_id = province_options[selected_province_name]

    # Slider: інтервал тижнів
    week_range = st.slider(
        "Інтервал тижнів:",
        min_value=1, max_value=52,
        key="week_slider"
    )

    # Slider: інтервал років
    year_range = st.slider(
        "Інтервал років:",
        min_value=min_year, max_value=max_year,
        key="year_slider"
    )

    st.markdown("---")

    # Checkbox: сортування
    st.subheader("📊 Сортування")
    sort_asc = st.checkbox("За зростанням ↑", key="sort_asc")
    sort_desc = st.checkbox("За спаданням ↓", key="sort_desc")

    # Обробка конфлікту чекбоксів
    if sort_asc and sort_desc:
        st.warning("⚠️ Обрано обидва варіанти — використовується сортування за зростанням")
        sort_order = True  # ascending
    elif sort_asc:
        sort_order = True
    elif sort_desc:
        sort_order = False
    else:
        sort_order = None

    st.markdown("---")

    # Кнопка Reset
    st.button("🔄 Скинути фільтри", use_container_width=True, on_click=reset_filters)


# ==================== Фільтрація даних ====================

# Фільтруємо за обраною областю, роками та тижнями
filtered_df = vhi_df[
    (vhi_df['Province_ID'] == selected_province_id) &
    (vhi_df['Year'] >= year_range[0]) &
    (vhi_df['Year'] <= year_range[1]) &
    (vhi_df['Week'] >= week_range[0]) &
    (vhi_df['Week'] <= week_range[1])
].copy()

# Сортування
if sort_order is not None:
    filtered_df = filtered_df.sort_values(selected_index, ascending=sort_order)

# Дані для порівняння по областях
comparison_df = vhi_df[
    (vhi_df['Year'] >= year_range[0]) &
    (vhi_df['Year'] <= year_range[1]) &
    (vhi_df['Week'] >= week_range[0]) &
    (vhi_df['Week'] <= week_range[1])
].copy()


# --- ПРАВА КОЛОНКА: Таблиця та графіки ---
with right_col:
    st.header(f"📈 {selected_index} — {selected_province_name}")
    st.markdown(f"Роки: **{year_range[0]}–{year_range[1]}** | "
                f"Тижні: **{week_range[0]}–{week_range[1]}** | "
                f"Записів: **{len(filtered_df)}**")

    # Три вкладки
    tab_table, tab_chart, tab_compare = st.tabs([
        "📋 Таблиця", "📈 Графік", "🔄 Порівняння областей"
    ])

    # --- Вкладка 1: Таблиця ---
    with tab_table:
        if filtered_df.empty:
            st.warning("Немає даних для обраних фільтрів")
        else:
            display_cols = ['Year', 'Week', 'VCI', 'TCI', 'VHI', 'Province_Name']
            st.dataframe(
                filtered_df[display_cols].reset_index(drop=True),
                use_container_width=True,
                height=500
            )
            st.markdown(f"**Статистика {selected_index}:** "
                        f"мін = {filtered_df[selected_index].min():.2f}, "
                        f"макс = {filtered_df[selected_index].max():.2f}, "
                        f"середнє = {filtered_df[selected_index].mean():.2f}, "
                        f"медіана = {filtered_df[selected_index].median():.2f}")

    # --- Вкладка 2: Графік часового ряду ---
    with tab_chart:
        if filtered_df.empty:
            st.warning("Немає даних для обраних фільтрів")
        else:
            # Створюємо часову мітку для осі X
            plot_df = filtered_df.copy()
            plot_df = plot_df.sort_values(['Year', 'Week'])
            plot_df['Date'] = plot_df['Year'].astype(str) + '-W' + plot_df['Week'].astype(str).str.zfill(2)

            fig = px.line(
                plot_df, x='Date', y=selected_index,
                title=f'{selected_index} для {selected_province_name} ({year_range[0]}–{year_range[1]})',
                labels={'Date': 'Рік-Тиждень', selected_index: selected_index},
            )
            fig.update_traces(line=dict(width=1))
            fig.update_layout(
                xaxis=dict(
                    tickmode='auto',
                    nticks=20,
                    tickangle=45
                ),
                height=500
            )
            st.plotly_chart(fig, use_container_width=True)

    # --- Вкладка 3: Порівняння областей ---
    with tab_compare:
        if comparison_df.empty:
            st.warning("Немає даних для порівняння")
        else:
            # Середнє значення по кожній області
            avg_by_province = comparison_df.groupby('Province_Name')[selected_index].mean().round(2)
            avg_by_province = avg_by_province.sort_values(ascending=False).reset_index()
            avg_by_province.columns = ['Область', f'Середнє {selected_index}']

            # Виділяємо обрану область
            colors = ['#e74c3c' if name == selected_province_name else '#3498db'
                      for name in avg_by_province['Область']]

            fig2 = go.Figure(data=[
                go.Bar(
                    x=avg_by_province['Область'],
                    y=avg_by_province[f'Середнє {selected_index}'],
                    marker_color=colors,
                    text=avg_by_province[f'Середнє {selected_index}'],
                    textposition='auto'
                )
            ])
            fig2.update_layout(
                title=f'Порівняння середнього {selected_index} по областях ({year_range[0]}–{year_range[1]})',
                xaxis_title='Область',
                yaxis_title=f'Середнє {selected_index}',
                xaxis_tickangle=45,
                height=550
            )
            st.plotly_chart(fig2, use_container_width=True)
            st.caption(f"🔴 Червоним виділено обрану область: **{selected_province_name}**")
