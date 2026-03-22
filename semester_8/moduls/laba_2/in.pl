# Установка выходного файла и формата
set terminal png size 800,600
set output 'points_inside2.png'

# Заголовок и подписи осей
set title "Точки внутри сегмента окружности"
set xlabel "X"
set ylabel "Y"

plot 'inner_points.txt' with points pointtype 0 title "Внутренние точки"