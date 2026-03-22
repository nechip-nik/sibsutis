
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import matplotlib.colors as mcolors
from matplotlib.colors import LogNorm

def generate_probabilities(n):
    """Генерирует n случайных чисел и нормализует их в вероятности"""
    probs = np.random.rand(n)
    probs /= probs.sum()
    return probs

def sampling_without_replacement(probs, sample_size):
    """Выборка без возвращения"""
    current_probs = probs.copy()
    n = len(current_probs)
    indices = np.arange(n)
    selection_counts = np.zeros((n, sample_size))
    
    for step in range(sample_size):
        # Вычисляем текущую сумму вероятностей (длину)
        total = np.sum(current_probs[:n - step])
        
        # Генерируем случайное число и находим выбранный индекс
        rand = np.random.rand() * total
        cumsum = 0
        selected_idx = 0
        for i in range(n - step):
            cumsum += current_probs[i]
            if rand <= cumsum:
                selected_idx = i
                break
        
        # Увеличиваем счетчик выбора для этого отрезка на этом шаге
        original_idx = indices[selected_idx]
        selection_counts[original_idx, step] += 1
        
        # Меняем местами выбранный элемент с последним неучтенным
        current_probs[selected_idx], current_probs[n - step - 1] = current_probs[n - step - 1], current_probs[selected_idx]
        indices[selected_idx], indices[n - step - 1] = indices[n - step - 1], indices[selected_idx]
    
    return selection_counts

# Параметры эксперимента
n_segments = 10
sample_size = 6
n_iterations = 1000

# Генерируем начальные вероятности
initial_probs = generate_probabilities(n_segments)

# Выводим начальные отрезки и их вероятности в консоль
print("Начальные отрезки и их вероятности:")
formatted_probs = ", ".join([f"{prob:.4f}" for prob in initial_probs])
print(formatted_probs)

# Матрица для хранения результатов (10 отрезков × 6 шагов)
result_matrix = np.zeros((n_segments, sample_size))

# Проводим 1000 итераций
for _ in range(n_iterations):
    # Используем initial_probs.copy(), чтобы все итерации работали с одинаковыми начальными вероятностями
    probs = initial_probs.copy()
    counts = sampling_without_replacement(probs, sample_size)
    result_matrix += counts

# Нормализуем результаты (чтобы показать вероятности)
normalized_matrix = result_matrix / n_iterations

# Группированная гистограмма

plt.figure(figsize=(14, 6))
bar_width = 0.15
x = np.arange(n_segments)

for step in range(sample_size):
    plt.bar(x + step * bar_width, normalized_matrix[:, step], width=bar_width,
            label=f'Step {step+1}')

plt.xlabel("Segment")
plt.ylabel("Selection Probability")
plt.title("Selection Probability by Segment and Step")
plt.xticks(x + bar_width * (sample_size-1)/2, [f"Seg {i+1}" for i in range(n_segments)])
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('груп. гистограмма.png')
