
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.table import Table

# Шаг 1: Генерация 10 случайных чисел и их нормализация
# np.random.seed(42)  # Для воспроизводимости результата
random_numbers = np.random.rand(10)
normalized_probabilities = random_numbers / np.sum(random_numbers)

# Проверка, что сумма нормализованных вероятностей равна 1
print("Нормализованные вероятности:", normalized_probabilities)

# Шаг 2: Создание отрезков вероятностей
cumulative_probabilities = np.cumsum(normalized_probabilities)
print("Кумулятивные вероятности (границы отрезков):", cumulative_probabilities)

# Шаг 3: Моделирование выборки с возвращением
num_samples = 1000
random_samples = np.random.rand(num_samples)

# Определение, в какой отрезок попадает каждое число
frequencies = np.zeros(10)
for sample in random_samples:
    for i, boundary in enumerate(cumulative_probabilities):
        if sample <= boundary:
            frequencies[i] += 1
            break

# Шаг 4: Подсчет эмпирических вероятностей
empirical_probabilities = frequencies / num_samples
print("Эмпирические вероятности:", empirical_probabilities)  # Добавлен вывод

# Шаг 5: Гистограмма - сравнение начальных и эмпирических вероятностей
plt.figure(figsize=(10, 5))
bar_width = 0.35
indices = np.arange(1, 11)

plt.bar(indices - bar_width/2, normalized_probabilities, bar_width, 
        label="Начальные вероятности", alpha=0.7)
plt.bar(indices + bar_width/2, empirical_probabilities, bar_width, 
        label="Эмпирические вероятности", alpha=0.7)

plt.xlabel("Отрезок")
plt.ylabel("Вероятность")
plt.title("Сравнение начальных и эмпирических вероятностей")
plt.xticks(indices)
plt.legend()
plt.savefig('Гист. сравн..png')
