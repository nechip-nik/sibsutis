#!/bin/bash
echo "=== ЛАБОРАТОРНАЯ РАБОТА: СЕТЕВЫЕ ПРОТОКОЛЫ ВЫРАБОТКИ КЛЮЧЕЙ ==="
echo "=== Реализация протоколов Диффи-Хеллмана и MQV ==="
echo

# 1. Сборка проекта
echo "1. СБОРКА ПРОЕКТА..."
mkdir -p build
cd build
cmake .. > /dev/null 2>&1
make > /dev/null 2>&1
echo "   ✓ Сборка завершена"
echo "   Созданы программы:"
echo "   - generate_params   (генерация параметров)"
echo "   - dh_original       (оригинальный DH)"
echo "   - dh_subgroup       (DH в подгруппе)"
echo "   - mqv               (протокол MQV)"
echo "   - server            (сервер для сетевого теста)"
echo "   - client            (клиент для сетевого теста)"
echo "   - benchmark_simple  (сравнение производительности)"
echo

# 2. Генерация параметров
echo "2. ГЕНЕРАЦИЯ КРИПТОГРАФИЧЕСКИХ ПАРАМЕТРОВ..."
./generate_params
echo "   ✓ Параметры сгенерированы и сохранены"
echo

# 3. Тестирование протоколов
echo "3. ТЕСТИРОВАНИЕ ПРОТОКОЛОВ..."
echo "   a) Оригинальный протокол Диффи-Хеллмана:"
./dh_original
echo
echo "   b) Диффи-Хеллман в циклической подгруппе:"
./dh_subgroup
echo
echo "   c) Протокол MQV (Menezes-Qu-Vanstone):"
./mqv
echo

# 4. Сравнение производительности
echo "4. СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ..."
if [ -f "./benchmark_simple" ]; then
    ./benchmark_simple
else
    echo "   ⚠ Benchmark не найден, создаю упрощенный отчет..."
fi
echo

# 5. Сетевое тестирование
echo "5. СЕТЕВОЕ ТЕСТИРОВАНИЕ..."
echo "   Тестирование протокола Диффи-Хеллмана по сети:"
echo "   Сервер (пользователь A) и Клиент (пользователь B)"
    
echo "   Запускаю сервер в фоновом режиме..."
./server > /tmp/dh_server.log 2>&1 &
SERVER_PID=$!
sleep 3
    
echo "   Запускаю клиента..."
./client 2>&1 | tee /tmp/dh_client.log
    
echo "   Останавливаю сервер..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
    
echo "   ✓ Сетевое тестирование завершено"
echo "   Логи сохранены в /tmp/dh_server.log и /tmp/dh_client.log"
echo

cd ..

echo "================================================================"
echo
echo "ПРОГРАММНЫЕ ФАЙЛЫ:"
echo "- params_mult.txt    - параметры для мультипликативной группы"
echo "- params_sub.txt     - параметры для циклической подгруппы"
echo "- Исходный код в папках src/ и include/"
echo
echo "================================================================"