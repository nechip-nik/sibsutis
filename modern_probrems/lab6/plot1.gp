# Файл: plot_priority.gp
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "priority_impact.png"

set title "Влияние приоритета писателя на производительность" font "Arial,16"
set xlabel "Приоритет писателя" font "Arial,14"
set ylabel "Количество операций" font "Arial,14"
set grid
set key top left

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5
set style line 2 lc rgb '#dd1818' lt 1 lw 2 pt 5 ps 1.5

plot "priority_data.txt" using 1:2 with linespoints ls 1 title "Операции чтения", \
     "priority_data.txt" using 1:3 with linespoints ls 2 title "Операции записи"