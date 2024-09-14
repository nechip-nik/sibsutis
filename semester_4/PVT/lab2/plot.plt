#!/usr/bin/gnuplot

set termoption enhanced
set terminal png size 1400,900 font "Arial, 24"
set output "grath.png"
set style line 1 lc rgb "0x1500ff" lt 3 lw 3 pt 4 ps 2
set style line 2 lc rgb "0x00ff1a" lt 3 lw 2 pt 5 ps 1
set border lw 2
set grid
set key top left
set xlabel "Threads (after 8 threads the second node is connected)"
set ylabel "Relative speedup" rotate by 90
set xtics 2
set ytics 1
set format x "%6.0f"
set format y "%1.0f"
set yrange [0:8]
set xrange [2:16]
plot "rezult.txt" using 1:2 title "N0=2000000" with linespoints ls 2, \
"rezult.txt" using 1:3 title "Linear speedup" with linespoints ls 1, \
