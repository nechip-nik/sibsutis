# Устанавливаем заголовок графика
set title "Mean time between failures (µ)"

# Устанавливаем подписи осей
set xlabel "Number n of elementary machines in base subsystem"
set ylabel "Mean time between failures (hours)"

# Устанавливаем диапазон осей
set xrange [900:1000]  # Диапазон от 2^0 до 2^6 (64)
set yrange [0.13:64]  # Диапазон от 0.00 до 0.030

# Устанавливаем сетку
set grid

# Устанавливаем формат вывода (например, PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'T(n)_µ.png'

# Устанавливаем логарифмическую шкалу для оси X
set xtics 10

# Устанавливаем логарифмическую шкалу для оси Y
set logscale y 2  # Логарифмическая шкала с основанием 2

# Устанавливаем шаг для оси Y в виде конкретных значений
set ytics (0.13, 0.25, 0.50, 1.00, 2.00, 4.00, 8.00, 16.00, 32.00, 64.00)

# Устанавливаем формат меток оси Y с двумя знаками после запятой
set format y "%.2f"


# Устанавливаем легенду слева
set key left


# Строим график с несколькими линиями и разными стилями
# Строим график с несколькими линиями и разными стилями
plot '../data/T_mu_1.txt' with linespoints lw 2 lt rgb "blue" title 'µ = 1 1/hours', \
     '../data/T_mu_2.txt' with linespoints lw 2 lt rgb "red" title 'µ = 2 1/hours', \
     '../data/T_mu_4.txt' with linespoints lw 2 lt rgb "green" title 'µ = 4 1/hours', \
     '../data/T_mu_6.txt' with linespoints lw 2 lt rgb "purple" title 'µ = 6 1/hours'