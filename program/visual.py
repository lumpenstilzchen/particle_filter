import pandas as pd
import matplotlib.pyplot as plt
import os

# список всех тестов
simulations = [
    ("output_default.csv",        "Базовые параметры (N=100, Q=1.0, R=4.0)"),
    ("output_low_Q.csv",          "Малый шум движения (Q=0.1)"),
    ("output_high_Q.csv",         "Большой шум движения (Q=5.0)"),
    ("output_low_R.csv",          "Точный датчик (R=0.5)"),
    ("output_high_R.csv",         "Шумный датчик (R=10.0)"),
    ("output_few_particles.csv",  "Мало частиц (N=10)"),
    ("output_many_particles.csv", "Много частиц (N=500)"),
]

# визуализация
for filename, title in simulations:
    if not os.path.exists(filename):
        continue

    df = pd.read_csv(filename)

    plt.figure(figsize=(10, 5))
    plt.plot(df["step"], df["real_pos"],    "k-",  label="Истинное положение")
    plt.plot(df["step"], df["measurement"], "rx",  label="Измерения")
    plt.plot(df["step"], df["estimate"],    "b--", label="Оценка фильтра")
    plt.title(title)
    plt.xlabel("Шаг")
    plt.ylabel("Позиция")
    plt.legend()
    plt.tight_layout()

    # сохраняем график как PNG
    out_name = filename.replace(".csv", ".png")
    plt.savefig(out_name)
    plt.show()
    plt.close()
