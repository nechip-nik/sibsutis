# Файл: plot_dfs.gp
# Gnuplot скрипт для построения графика времени выполнения DFS
# Выполните: gnuplot plot_dfs.gp

# Настройки терминала и выходного файла
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "dfs_execution_time.png"

# Заголовок и подписи осей
set title "Экспоненциальный рост времени выполнения DFS (поиск в глубину)" font "Arial,16"
set xlabel "Количество вершин" font "Arial,14"
set ylabel "Время выполнения (секунды)" font "Arial,14"

# Включаем сетку
set grid

# Устанавливаем логарифмическую шкалу для оси Y
set logscale y

# Формат меток на оси Y (научный формат)
set format y "10^{%L}"

# Добавляем легенду
set key left top

# Стили линий
set style line 1 lc rgb '#0060ad' lt 1 lw 3 pt 7 ps 2
set style line 2 lc rgb '#dd1818' lt 1 lw 2 pt 5 ps 1.5

# Построение графика
plot "dfs_time_data.txt" using 1:2 with linespoints ls 1 title "DFS время выполнения"

# Дополнительный график в линейном масштабе
set output "dfs_execution_time_linear.png"
unset logscale y
set format y "%g"
set title "Время выполнения DFS (линейный масштаб)"
plot "dfs_time_data.txt" using 1:2 with linespoints ls 1 title "DFS время выполнения"

# График с точками данных
set output "dfs_execution_time_points.png"
set logscale y
set title "Время выполнения DFS с отмеченными точками"
plot "dfs_time_data.txt" using 1:2 with linespoints ls 1 title "DFS", \
     "dfs_time_data.txt" using 1:2:(sprintf("%.2e", $2)) with labels offset 1,1 font "Arial,8" notitle

# График сравнения с O(n!) (теоретической сложностью)
set output "dfs_complexity.png"
set title "Сравнение DFS с теоретической сложностью O(n!)"
set xlabel "Количество вершин"
set ylabel "Время (секунды)"
set logscale y

# Вычисление факториала и нормализация
factorial(n) = (n<=1) ? 1 : n * factorial(n-1)
norm_factor = 1e-9  # Коэффициент нормализации (подберите под ваши данные)

plot "dfs_time_data.txt" using 1:2 with linespoints ls 1 title "DFS (реальные данные)", \
     "dfs_time_data.txt" using 1:(norm_factor * factorial($1-2)) with lines ls 2 title "O(n!) (теоретическая)"