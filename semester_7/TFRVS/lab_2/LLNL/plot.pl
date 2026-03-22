# Устанавливаем заголовок графика
 set title "Dependence of T(s) on the number of tasks"

# Устанавливаем подписи осей
 set xlabel "Size tasks (m)\"
 set ylabel "T(S)"

# Устанавливаем диапазон осей
set xrange [500:1500]  
set yrange [20:230] 

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plot.png'

# Устанавливаем шаг для оси X
set xtics 500

# Устанавливаем шаг для оси Y
set ytics 10

# # Устанавливаем формат отображения значений на оси Y
 set format y "%.3f"  # Формат с четырьмя знаками после запятой

# Устанавливаем легенду слева
set key left

# Строим график с несколькими линиями и разными стилями
plot 'LLNL1.txt' with linespoints lw 2 lt rgb "blue" title 'FFDH', \
     'NFHD.txt' with linespoints lw 2 lt rgb "red" title 'NFDH', \