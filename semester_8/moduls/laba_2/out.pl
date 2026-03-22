# Установка выходного файла и формата
set terminal png size 800,600
set output 'points_outside2.png'

# Заголовок и подписи осей
set title "Точки на границах сегмента окружности"
set xlabel "X"
set ylabel "Y"

# Построение графика
plot 'boundary_points.txt' using 1:2  with points pointtype 0 title "", \
     'boundary_points.txt' using 1:3  with points pointtype 0 title ""