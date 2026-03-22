set terminal pngcairo enhanced font 'Arial,12'
set output 'U_star_plot.png'
set title 'Зависимость U*(t) для различных n'
set xlabel 'Время t (часы)'
set ylabel 'U*(t)'
set grid
plot for [col=2:8] 'U_star_data.txt' using 1:col with lines title columnheader
