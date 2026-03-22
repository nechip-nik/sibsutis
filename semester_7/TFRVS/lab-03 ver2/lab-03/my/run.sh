#!/bin/bash

# Имя выходного файла
output_file="times_large.csv"

# Записываем заголовки в CSV
echo "n,execution_time" > $output_file

# Цикл по n от 50 до 1000 с шагом 50
for n in $(seq 50 50 2000); do
    echo "Запуск main с n = $n ..."
    
    # Запускаем программу и измеряем время выполнения
    start_time=$(date +%s.%N)
    ./main $n 1.5 2.5 3.5 > /dev/null
    end_time=$(date +%s.%N)

    # Вычисляем время выполнения
    execution_time=$(echo "$end_time - $start_time" | bc)

    # Записываем результат в CSV
    echo "$n,$execution_time" >> $output_file

    echo "n = $n, время выполнения: $execution_time секунд"
done

echo "Все запуски завершены. Результаты сохранены в $output_file."
