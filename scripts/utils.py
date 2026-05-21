#!/usr/bin/env python3
"""
utils.py - Вспомогательные функции для Python-части
"""
import sys
import os
import numpy as np
from pathlib import Path

# ================== ПУТИ ==================
PROJECT_ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = PROJECT_ROOT / "data"
MODELS_DIR = PROJECT_ROOT / "models"
VIS_DIR = PROJECT_ROOT / "visualization"

def ensure_dirs():
    """Создаёт необходимые папки, если их нет"""
    DATA_DIR.mkdir(exist_ok=True)
    MODELS_DIR.mkdir(exist_ok=True)
    VIS_DIR.mkdir(exist_ok=True)

# ================== ИМПОРТ C++ МОДУЛЯ ==================
def import_cpp_module():
    """
    Импортирует anaconda_nn, автоматически находя .so/.pyd файл в папке build/
    """
    # Ищем скомпилированный модуль
    build_dir = PROJECT_ROOT / "cmake-build-debug"  # или просто "build"
    if not build_dir.exists():
        build_dir = PROJECT_ROOT / "build"

    if build_dir.exists():
        sys.path.insert(0, str(build_dir))

    try:
        import anaconda_nn
        return anaconda_nn
    except ImportError:
        print(" Не найден модуль anaconda_nn")
        print(" Убедись, что Участники 1-2 собрали проект:")
        print(" cd build && cmake .. && make")
        print(" Файл anaconda_nn.cpython-*.so должен лежать в build/")
        sys.exit(1)

# ================== КОНВЕРТАЦИЯ ДАННЫХ ==================
def numpy_to_list(arr):
    """Конвертирует numpy array в список списков (для передачи в C++ через pybind11)"""
    return arr.tolist()

def list_to_numpy(lst):
    """Конвертирует список из C++ обратно в numpy array"""
    return np.array(lst)

# ================== ЛОГИРОВАНИЕ ==================
def log(msg, level="INFO"):
    prefix = {"INFO": "ℹ️", "WARN": "предупреждение️", "ERROR": " ошибка ", "SUCCESS": " успех "}
    icon = prefix.get(level, "📝")
    print(f"{icon} [{level}] {msg}")