# Устанавливаем заголовок графика
set title "Dependence of execution time on message size"

# Устанавливаем подписи осей
set xlabel "Message size (Mb)"
set ylabel "Elapsed time (S)"

# Устанавливаем диапазон осей
set xrange [1:2**6]  # Диапазон от 2^0 до 2^6 (64)
set yrange [0:0.1]  # Диапазон от 0.00 до 0.030

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plot.png'

# Устанавливаем логарифмическую шкалу для оси X
set logscale x 2

# Устанавливаем шаг для оси Y
set ytics 0.02

# Устанавливаем легенду слева
set key left

# Устанавливаем формат меток оси X в виде степеней двойки
set format x "2^{%L}"

# Строим график с несколькими линиями и разными стилями
plot 'separate.txt' with linespoints lw 2 lt rgb "blue" title '1x8 Separate CPUs', \
     'single.txt' with linespoints lw 2 lt rgb "red" title '1x8 Single CPU', \
     'separate_note.txt' with linespoints lw 2 lt rgb "green" title '2x4 Separate nodes'