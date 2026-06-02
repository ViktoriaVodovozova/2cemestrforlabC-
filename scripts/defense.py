# defense.py
import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import f1_score
import anaconda_nn
import joblib
import sys
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"

sys.path.insert(0, str(BUILD))

def defense(weights_file, scaler_file, test_csv, epochs=5):
    """
    Загружает готовую модель, дообучает на скрытом датасете, выдаёт метрику.
    """
    print(f"=== Defense mode ===")
    print(f"Веса: {weights_file}, Скейлер: {scaler_file}, Тест: {test_csv}")

    # Загрузка модели
    net = anaconda_nn.NeuralNetwork([2, 16, 1], ['relu', 'sigmoid'])
    net.load_weights(weights_file)
    scaler = joblib.load(scaler_file)

    # Загрузка скрытого датасета
    df = pd.read_csv(test_csv)
    X = df[['x', 'y']].values
    y = df['label'].values.reshape(-1, 1)

    # Масштабирование
    X_scaled = scaler.transform(X)

    # Дообучение (3-5 эпох, маленький lr)
    print(f"Дообучение: {epochs} эпох, lr=0.01")
    net.train(X_scaled, y, epochs, lr=0.01)

    # Оценка
    y_pred = net.predict(X_scaled)
    f1 = f1_score(y, y_pred)
    acc = np.mean(y == y_pred)

    print(f"\n Результат на скрытом датасете:")
    print(f"   F1-score: {f1:.3f}")
    print(f"   Accuracy: {acc:.3f}")

    # Условие принятия
    if f1 >= 0.55:
        print(" Модель прошла проверку")
        return True
    else:
        print(" Модель не достигла порога")
        return False

if __name__ == "__main__":
    # Пример вызова:
    # python defense.py final_weights.json final_scaler.pkl d3.csv
    if len(sys.argv) != 4:
        print("Использование: python defense.py <weights.json> <scaler.pkl> <test.csv>")
        sys.exit(1)

    success = defense(sys.argv[1], sys.argv[2], sys.argv[3])
    sys.exit(0 if success else 1)