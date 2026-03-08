import json
import matplotlib.pyplot as plt
import numpy as np

def read_clustered_points(filename):
    with open(filename, 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    points = data['points']
    
    # Используем поле 'label' для определения кластера
    if 'label' in points[0]:
        clusters = {}
        for point in points:
            cluster_id = point.get('label', 0)  # Изменено с 'true_cluster' на 'label'
            if cluster_id not in clusters:
                clusters[cluster_id] = {'x': [], 'y': []}
            clusters[cluster_id]['x'].append(point['x'])
            clusters[cluster_id]['y'].append(point['y'])
        return clusters
    else:
        print("Внимание: поле 'label' не найдено, все точки отображаются одним цветом")
        # Если нет кластеров, возвращаем как один кластер
        return {0: {'x': [p['x'] for p in points], 
                   'y': [p['y'] for p in points]}}

def plot_clusters(clusters):
    plt.figure(figsize=(12, 8))
    
    # Цвета для разных кластеров (0 и 1)
    colors = ['blue', 'red']  # Синий для кластера 0, красный для кластера 1
    
    for i, (cluster_id, coords) in enumerate(clusters.items()):
        color = colors[i % len(colors)]
        plt.scatter(coords['x'], coords['y'], 
                   c=color, alpha=0.6, s=50,  # Увеличил размер точек для наглядности
                   label=f'Кластер {cluster_id}')
    
    plt.axhline(y=0, color='gray', linestyle='--', alpha=0.3)
    plt.axvline(x=0, color='gray', linestyle='--', alpha=0.3)
    
    plt.title('Кластеризованные точки на координатной плоскости', fontsize=16)
    plt.xlabel('X', fontsize=12)
    plt.ylabel('Y', fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    plt.tight_layout()
    plt.show()

# Использование
filename = 'build/points.json'
clusters = read_clustered_points(filename)
plot_clusters(clusters)