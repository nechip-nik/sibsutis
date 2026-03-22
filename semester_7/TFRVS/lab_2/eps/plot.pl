# Устанавливаем заголовок графика
set title "FFDH and NFDH"

# Устанавливаем подписи осей
set xlabel "Tasks (m)"
set ylabel "Function (Makespan)"

set xrange [1:5000]  
set yrange [1022.0:1026.0] 

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'plot.png'

set xtics 1000

set ytics 0.5

set format y "%.1f" 

set key left



# Строим график с несколькими линиями и разными стилями
plot 'FFDH.txt' with linespoints lw 2 lt rgb "blue" title 'FFDH', \
     'NFDH.txt' with linespoints lw 2 lt rgb "red" title 'NFDH', \