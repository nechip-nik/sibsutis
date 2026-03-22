# Gnuplot скрипт для построения графика времени выполнения
# Сохраните как plot_time.gp и выполните: gnuplot plot_time.gp

# Настройки терминала и выходного файла
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "execution_time.png"

# Заголовок и подписи осей
set title "Экспоненциальный рост времени выполнения полного перебора" font "Arial,16"
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

# Построение графика
plot "time_data.txt" using 1:2 with linespoints ls 1 title "Время выполнения"



# Дополнительный график в линейном масштабе (для сравнения)
set output "execution_time_linear.png"
unset logscale y
set format y "%g"
set title "Время выполнения (линейный масштаб)"
plot "time_data.txt" using 1:2 with linespoints ls 1 title "Время выполнения"