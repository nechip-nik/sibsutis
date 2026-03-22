# Устанавливаем выходной формат (например, PNG)
set terminal png size 800,600 enhanced font "Helvetica,12"
set output 'histogram.png'

# Заголовок графика
set title "Гистограмма значений из data.txt"

# Подписи осей
set xlabel "Значения"
set ylabel "Частота"

# Устанавливаем стиль гистограммы
set style data histograms
set style fill solid border -1

# Устанавливаем количество бинов (столбцов) для гистограммы
bin_width = 0.05  # Ширина бина
bin_number(x) = floor(x/bin_width)  # Функция для определения номера бина
bin_center(x) = bin_width*(bin_number(x) + 0.5)  # Центр бина

# Строим гистограмму
plot 'data.txt' using (bin_center($1)):(1) smooth frequency with boxes title "Частота"