# train.py
import numpy as np
import pandas as pd
import joblib
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import f1_score, classification_report
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"

sys.path.insert(0, str(BUILD))

import anaconda_nn

def load_and_prepare_data(filepath):

    df = pd.read_csv(filepath)

    target_col = None

    for col in ['target', 'label', 'Label', 'class']:
        if col in df.columns:
            target_col = col
            break

    if target_col is None:
        target_col = df.columns[-1]

    X = df.drop(columns=[target_col]).values
    y = df[target_col].values.reshape(-1, 1)

    return X, y

def train_on_dataset(name, filepath, epochs=500, lr=0.1):
    print(f"\n=== Обучение на {name} ===")

    # 1. Загрузка
    X, y = load_and_prepare_data(filepath)
    columns_n = X.shape[1]

    # 2. масштабирование!
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # 3. Train/test split 80/20 (с перемешиванием!)
    X_train, X_test, y_train, y_test = train_test_split(
        X_scaled, y, test_size=0.2, random_state=42, stratify=y
    )

    # 4. Конвертация в Matrix для C++

    # 5. Создаём сеть: топология [2  16  1], активации [relu, sigmoid]
    net = anaconda_nn.NeuralNetwork([columns_n, 16, 1], ['relu', 'sigmoid'])

    # 6. Обучение (предполагаем, что train() принимает numpy-массивы)
    net.train(X_train, y_train, epochs, lr)

    # 7. Предсказания на тесте
    y_pred = net.predict(X_test)  # возвращает 0 или 1

    # 8. Метрики
    f1 = f1_score(y_test, y_pred)
    acc = np.mean(y_test.flatten() == y_pred)

    print(f"{name} — F1: {f1:.3f}, Accuracy: {acc:.3f}")
    print(classification_report(y_test, y_pred, target_names=['Class 0', 'Class 1']))

    # 9. Сохранение весов
    weights_file = f"weights_{name}.json"
    net.save_weights(weights_file)
    print(f"Веса сохранены в {weights_file}")

    # 10. Сохраняем scaler для defense.py
    import joblib
    joblib.dump(scaler, f"scaler_{name}.pkl")

    return f1, net, scaler

def main():
    # Обучаем на датасетах
    f1_d1, net_d1, scaler_d1 = train_on_dataset("d1", "data/dataset1.csv")
    f1_d2, net_d2, scaler_d2 = train_on_dataset("d2", "data/dataset2.csv")
    f1_d3, net_d3, scaler_d3 = train_on_dataset("d3", "data/dataset3.csv")
    # Итоговая оценка
    final_score = 0.5 * f1_d1 + 0.5 * f1_d2
    print(f"Итоговый скор: {final_score:.3f} (порог: 0.55)")

    if final_score >= 0.55:
        print(" Все хорошо ")
        # Сохраняем лучшую модель (или обе)
        net_d2.save_weights("final_weights.json")
        joblib.dump(scaler_d2, "final_scaler.pkl")
    else:
        print(" Нужно дообучить ")
        sys.exit(1)

if __name__ == "__main__":
    main()