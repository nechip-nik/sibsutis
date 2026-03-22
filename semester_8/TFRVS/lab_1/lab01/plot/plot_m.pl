# Устанавливаем заголовок графика
set title "Mean time between failures (m)"

# Устанавливаем подписи осей
set xlabel "Number n of elementary machines in base subsystem"
set ylabel "Mean time between failures (hours)"

# Устанавливаем диапазон осей
set xrange [8090:8200]  # Диапазон от 2^0 до 2^6 (64)
set yrange [0.2:1.4]  # Диапазон от 0.00 до 10^50

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'T(n)_m.png'

# Устанавливаем логарифмическую шкалу для оси X
set xtics 10

set ytics 0.2

# Устанавливаем формат меток оси Y с двумя знаками после запятой
set format y "%.2f"

# Устанавливаем легенду справа
set key right

# Строим график с несколькими линиями и разными стилями
plot '../data/T_m_1.txt' with linespoints lw 2 lt rgb "blue" title 'm = 1', \
     '../data/T_m_2.txt' with linespoints lw 2 lt rgb "red" title 'm = 2', \
     '../data/T_m_3.txt' with linespoints lw 2 lt rgb "green" title 'm = 3', \
     '../data/T_m_4.txt' with linespoints lw 2 lt rgb "purple" title 'm = 4'