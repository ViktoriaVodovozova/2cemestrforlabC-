# train.py
import numpy as np
import pandas as pd
import joblib
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import f1_score, classification_report
import anaconda_nn  # модуль через pybind11
import json
import sys

def load_and_prepare_data(filepath):
    """Загружает CSV, возвращает X, y как numpy-массивы"""
    print(f"Загрузка данных из {filepath}...")
    df = pd.read_csv(filepath)

    # Проверяем, какие колонки есть
    if 'feature_0' in df.columns and 'feature_1' in df.columns:
        X = df[['feature_0', 'feature_1']].values
    elif 'x' in df.columns and 'y' in df.columns:
        X = df[['x', 'y']].values
    elif 'X' in df.columns and 'Y' in df.columns:
        X = df[['X', 'Y']].values
    else:
        X = df.iloc[:, :2].values  # fallback

    # Ищем метки
    if 'target' in df.columns:
        y = df['target'].values.reshape(-1, 1)
    elif 'label' in df.columns:
        y = df['label'].values.reshape(-1, 1)
    elif 'Label' in df.columns:
        y = df['Label'].values.reshape(-1, 1)
    elif 'class' in df.columns:
        y = df['class'].values.reshape(-1, 1)
    else:
        y = df.iloc[:, -1].values.reshape(-1, 1)  # fallback

    print(f" Загружено {len(X)} точек")
    print(f"  Класс 0: {np.sum(y == 0)}, Класс 1: {np.sum(y == 1)}")

    return X, y

def train_on_dataset(name, filepath, epochs=500, lr=0.1):
    print(f"\n=== Обучение на {name} ===")

    # 1. Загрузка
    X, y = load_and_prepare_data(filepath)

    # 2. масштабирование!
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # 3. Train/test split 80/20 (с перемешиванием!)
    X_train, X_test, y_train, y_test = train_test_split(
        X_scaled, y, test_size=0.2, random_state=42, stratify=y
    )

    # 4. Конвертация в Matrix для C++

    # 5. Создаём сеть: топология [2  16  1], активации [relu, sigmoid]
    net = anaconda_nn.NeuralNetwork([2, 16, 1], ['relu', 'sigmoid'])

    # 6. Обучение (предполагаем, что train() принимает numpy-массивы)
    net.train(X_train, y_train, epochs, lr)

    # 7. Предсказания на тесте
    y_pred = net.predict(X_test)  # возвращает 0 или 1

    # 8. Метрики
    f1 = f1_score(y_test, y_pred)
    acc = np.mean(y_test == y_pred)

    print(f"{name} — F1: {f1:.3f}, Accuracy: {acc:.3f}")
    print(classification_report(y_test, y_pred, target_names=['Class 0', 'Class 1']))

    # 9. Сохранение весов
    weights_file = f"weights_{name}.json"
    net.save_weights(weights_file)
    print(f"✓ Веса сохранены в {weights_file}")

    # 10. Сохраняем scaler для defense.py
    import joblib
    joblib.dump(scaler, f"scaler_{name}.pkl")

    return f1, net, scaler

def main():
    # Обучаем на двух датасетах
    f1_d1, net_d1, scaler_d1 = train_on_dataset("d1", "data/d1.csv")
    f1_d2, net_d2, scaler_d2 = train_on_dataset("d2", "data/d2.csv")
    # Итоговая оценка
    final_score = 0.5 * f1_d1 + 0.5 * f1_d2
    print(f"\ Итоговый скор: {final_score:.3f} (порог: 0.55)")

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