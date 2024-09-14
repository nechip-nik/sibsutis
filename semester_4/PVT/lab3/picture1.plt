set terminal png size 1400,900 font "Arial,20"
set output "histogram.png"

set yrange [0:9]
set xrange [0:10]

set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9

set grid
set key top left
set xlabel "Processes. 'amount of processes'('nodes'/'processes')"
set ylabel "Relative speedup" rotate by 90

plot 'data.dat' using 2:xtic(1) ti "Linear speedup", '' u 3 ti "N = 28000", '' u 4 ti "N = 45000"