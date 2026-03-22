# Устанавливаем заголовок графика
set title "Elapsed time FFDH and NFDH"

# Устанавливаем подписи осей
set xlabel "Number of tasks (m)"
set ylabel "Elapsed time (s)"

# Устанавливаем диапазон осей
set xrange [500:1500]  
set yrange [0.0000:0.0200] 

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plot_LLNL.png'

# Устанавливаем шаг для оси X
set xtics 500

# Устанавливаем шаг для оси Y
set ytics 0.0025

# Устанавливаем формат отображения значений на оси Y
set format y "%.4f"  # Формат с четырьмя знаками после запятой

# Устанавливаем легенду слева
set key left

# Строим график с несколькими линиями и разными стилями
plot 'FFDH-1024.txt' with linespoints lw 2 lt rgb "blue" title 'FFDH', \
     'NFDH-1024.txt' with linespoints lw 2 lt rgb "red" title 'NFDH', \