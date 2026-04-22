import json
import numpy as np
import matplotlib.pyplot as plt

# =====  загрузка аточек из json =====
with open('build/points.json', 'r') as f:
    json_data = json.load(f)

# извлекаем точки
points = json_data['points']
X = [p['x'] / 100.0 for p in points]   # нормализация как в C++
Y = [p['y'] / 100.0 for p in points]
labels = [p['label'] for p in points]

print(f"Загружено {len(X)} точек")

# ===== загрузка границы =====
try:
    boundary_data = np.loadtxt('build/line.txt')
    X_grid = boundary_data[:, 0]
    Y_grid = boundary_data[:, 1]
    probs = boundary_data[:, 2]
    has_boundary = True
    print("Загружена разделяющая граница")
except:
    has_boundary = False
    print("Файл boundary.txt не найден, рисую только точки")

# ===== построение графика =====
plt.figure(figsize=(10, 8))

# Рисуем точки
colors = ['purple' if l == 1 else 'blue' for l in labels]
plt.scatter(X, Y, c=colors, edgecolors='black', s=50, alpha=0.7)

# Рисуем границу (если есть)
if has_boundary:
    # Создаём сетку для contour
    import pandas as pd
    df = pd.DataFrame({'x': X_grid, 'y': Y_grid, 'prob': probs})
    pivot = df.pivot(index='y', columns='x', values='prob')
    plt.contour(pivot.columns.values, pivot.index.values, pivot.values, 
                levels=[0.5], colors='black', linewidths=2, linestyles='dashed')

plt.xlabel('X (нормализованная)')
plt.ylabel('Y (нормализованная)')
plt.title('Кластеры точек и разделяющая граница')
plt.colorbar(plt.scatter(X, Y, c=labels, cmap='bwr', alpha=0), label='Класс (0=синий, 1=фиолетовый)')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.savefig('visualization.png', dpi=150)
plt.show()

print("График сохранён в visualization.png")