import matplotlib.pyplot as plt
import json
import numpy as np

with open('build/points.json', 'r') as f:
    data = json.load(f)

points = data['points']
x = [p['x'] for p in points]
y = [p['y'] for p in points]
labels = [p['label'] for p in points]

# Преобразуем в numpy массивы для удобства
x = np.array(x)
y = np.array(y)
labels = np.array(labels)

plt.figure(figsize=(10, 8))

# Задаем цвета для каждого кластера
colors = ['blue', 'purple']  # кластер 0 - синий, кластер 1 - фиолетовый

# Рисуем точки каждого кластера
for cluster_id in [0, 1]:
    mask = (labels == cluster_id)
    plt.scatter(x[mask], y[mask], 
               c=colors[cluster_id], 
               label=f'Кластер {cluster_id}', 
               s=50, alpha=0.8)

plt.grid(True, linestyle='--', alpha=0.7)
plt.xlabel('X координата', fontsize=12)
plt.ylabel('Y координата', fontsize=12)
plt.title('Визуализация кластеров', fontsize=14)
plt.legend()  # Добавляем легенду

plt.tight_layout()
plt.savefig('build/clusters.png', dpi=150, bbox_inches='tight')
plt.show()