# Устанавливаем заголовок графика
set title "Dependence of execution time on the number of elementary machines"
# Устанавливаем подписи осей
set xlabel "Number of elementary machines"
set ylabel "Elapsed time (s)"

# Устанавливаем диапазон осей
set xrange [0:2100]  
set yrange [0:100] 

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plot_large.png'

# Устанавливаем шаг для оси X
set xtics 250

# Устанавливаем шаг для оси Y
set ytics 10



# Устанавливаем легенду слева
set key left

# Строим график с несколькими линиями и разными стилями
plot 'large.txt' with linespoints lw 2 lt rgb "blue" title 'Elapsed time', \