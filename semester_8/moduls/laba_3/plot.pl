# Установка терминала и вывода
set terminal pngcairo size 1200,800
set output 'tree.png'

# Настройка графика
set title "Случайное дерево "
set xlabel "X"
set ylabel "Y"
set grid

# Установка диапазона осей для лучшей видимости
set xrange [-5:5]
set yrange [-9:1]
# Отрисовка рёбер
plot 'edges.dat' using 1:2 with lines lc rgb "black" notitle

# Отрисовка вершин
set pointsize 2
replot 'vertices.dat' using 2:3 with points pt 7 lc rgb "blue" notitle

# Отрисовка меток вершин
set label front
replot 'vertices.dat' using 2:3:1 with labels offset 0.5,0.5 notitle

# Завершение
set output