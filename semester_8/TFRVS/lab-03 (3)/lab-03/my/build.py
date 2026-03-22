import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из CSV-файла
data = pd.read_csv("times_large.csv")

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(data["n"], data["execution_time"], marker='o', linestyle='-', label="Execution Time", color='#7232f2')

# Настройка графика
plt.title("Execution Time vs Number of Systems (n)", fontsize=14)
plt.xlabel("Number of Systems (n)", fontsize=12)
plt.ylabel("Execution Time (seconds)", fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=12)
plt.xticks(fontsize=10)
plt.yticks(fontsize=10)

# Сохранение графика
plt.savefig("times_large.png")
