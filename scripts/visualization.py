# visualization.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
import anaconda_nn
import joblib

def plot_decision_boundary(name, filepath, weights_file, scaler_file):
    print(f"=== Визуализация: {name} ===")

    # Загрузка данных
    df = pd.read_csv(filepath)

# ✅ Гибкий выбор координат
    if 'feature_0' in df.columns and 'feature_1' in df.columns:
        X = df[['feature_0', 'feature_1']].values
    else:
        X = df.iloc[:, :2].values  # fallback

    # ✅ Гибкий выбор меток
    if 'target' in df.columns:
        y = df['target'].values
    else:
        y = df.iloc[:, -1].values

    # Загрузка scaler и весов
    scaler = joblib.load(scaler_file)
    net = anaconda_nn.NeuralNetwork([2, 16, 1], ['relu', 'sigmoid'])
    net.load_weights(weights_file)

    # Масштабируем исходные данные для отображения
    X_scaled = scaler.transform(X)

    # Создаём сетку для decision boundary
    h = 0.02  # шаг сетки
    x_min, x_max = X_scaled[:, 0].min() - 1, X_scaled[:, 0].max() + 1
    y_min, y_max = X_scaled[:, 1].min() - 1, X_scaled[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))

    # Предсказание для каждой точки сетки
    grid = np.c_[xx.ravel(), yy.ravel()]
    Z = np.array(net.predict(grid)).reshape(xx.shape)

    # Визуализация
    plt.figure(figsize=(8, 6))
    plt.contourf(xx, yy, Z, levels=[-0.5, 0.5, 1.5],
                 colors=['#FF9999', '#99CCFF'], alpha=0.3)
    plt.scatter(X_scaled[:, 0], X_scaled[:, 1], c=y,
                cmap=plt.cm.RdYlBu, edgecolors='k', s=20)
    plt.xlabel('X (scaled)')
    plt.ylabel('Y (scaled)')
    plt.title(f'Decision Boundary — {name}')
    plt.grid(True, alpha=0.3)
    plt.savefig(f'boundary_{name}.png', dpi=300)
    print(f"✓ График сохранён: boundary_{name}.png")
    plt.show()

if __name__ == "__main__":
    plot_decision_boundary("d1", "data/d1.csv", "weights_d1.json", "scaler_d1.pkl")
    plot_decision_boundary("d2", "data/d2.csv", "weights_d2.json", "scaler_d2.pkl")