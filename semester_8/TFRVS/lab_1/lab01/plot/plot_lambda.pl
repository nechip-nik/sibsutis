# Устанавливаем заголовок графика
set title "Mean time between failures (lambda)"

# Устанавливаем подписи осей
set xlabel "Number n of elementary machines in base subsystem"
set ylabel "Mean time between failures (hours)"

# Устанавливаем диапазон осей
set xrange [8090:8200]  # Диапазон от 2^0 до 2^6 (64)
set yrange [0.95:1.20]  # Диапазон от 0.00 до 10^50

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'T(n)_lambda.png'

# Устанавливаем логарифмическую шкалу для оси X
set xtics 10

set ytics 0.05



# Устанавливаем формат меток оси Y с двумя знаками после запятой
set format y "%.2f"

# Устанавливаем легенду справа
set key right

# Строим график с несколькими линиями и разными стилями
plot '../data/T_lambda_0.000000001.txt' with linespoints lw 2 lt rgb "blue" title 'lambda = {10}^{-5} 1/hours', \
     '../data/T_lambda_0.000000010.txt' with linespoints lw 2 lt rgb "red" title 'lambda = {10}^{-6} 1/hours', \
     '../data/T_lambda_0.000000100.txt' with linespoints lw 2 lt rgb "green" title 'lambda = {10}^{-7} 1/hours', \
     '../data/T_lambda_0.000001000.txt' with linespoints lw 2 lt rgb "purple" title 'lambda = {10}^{-8} 1/hours', \
     '../data/T_lambda_0.000010000.txt' with linespoints lw 2 lt rgb "black" title 'lambda = {10}^{-9} 1/hours'