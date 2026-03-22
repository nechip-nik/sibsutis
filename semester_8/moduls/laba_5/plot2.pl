# Настройки терминала
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output 'markov_chains.png'

# Настройки границ для центрирования
set lmargin at screen 0.15
set rmargin at screen 0.85
set tmargin at screen 0.9
set bmargin at screen 0.1

# Основной график
set title "Анализ цепей Маркова" font ",14" offset 0,1
set xlabel "Шаг" offset 0,0.5
set ylabel "Нормированное значение" offset 2,0
set grid
set key center top

plot "chain1.txt" using 1:3 with lines lw 2 title "Цепь 1 (P1)", \
    #  "chain2.txt" using 1:3 with lines lw 2 title "Цепь 2 (P2)"
