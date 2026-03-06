import json
import matplotlib.pyplot as plt
import numpy as np

def read_clustered_points(filename):
    with open(filename, 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    points = data['points']
    
    # Если есть информация о кластерах
    if 'true_cluster' in points[0]:
        clusters = {}
        for point in points:
            cluster_id = point.get('true_cluster', 0)
            if cluster_id not in clusters:
                clusters[cluster_id] = {'x': [], 'y': []}
            clusters[cluster_id]['x'].append(point['x'])
            clusters[cluster_id]['y'].append(point['y'])
        return clusters
    else:
        # Если нет кластеров, возвращаем как один кластер
        return {0: {'x': [p['x'] for p in points], 
                   'y': [p['y'] for p in points]}}

def plot_clusters(clusters):
    plt.figure(figsize=(12, 8))
    
    # Цвета для разных кластеров
    colors = ['blue', 'red', 'green', 'purple', 'orange', 'brown']
    
    for i, (cluster_id, coords) in enumerate(clusters.items()):
        color = colors[i % len(colors)]
        plt.scatter(coords['x'], coords['y'], 
                   c=color, alpha=0.6, s=30, 
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