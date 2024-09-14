#!/bin/bash
current_date=$(date "+%Y-%m-%d %H:%M:%S")
echo "Дата: $current_date" 
echo "Имя учетной записи : $USER"
echo "Доменное имя Пк: $(hostname)" 

echo "Процессор:"
echo -en "\t Модель - " & lscpu | grep 'Имя модели' | awk -F ':' '{print $2}' | sed 's/^[ \t]*//'
echo -en "\t Архитектура - " & lscpu | grep 'Архитектура' | awk '{print $2}'
echo -en "\t Тактовая частота максимальная - " & sudo dmidecode -t processor | grep 'Max Speed' | awk '{print $3, $4}'
echo -en "\t Тактовая частота (используемая) - " & sudo dmidecode -t processor | grep 'Current Speed' | awk '{print $3, $4}'
echo -en "\t Количество ядер - " &lscpu  | grep 'Ядер на сокет' | awk '{print $4}'
echo -en "\t Количество потоков на одно ядро - " &lscpu  | grep 'Потоков на ядро' | awk '{print $4}'
echo -en "\t Загрузка процессора- " &top -bn1 | grep "Cpu(s)" | awk '{print $2}' | cut -d'%' -f1

echo "Оперативная память:"
echo -en "\t Cache L1d - " & lscpu | grep 'L1d' | awk '{print $3, $4}'
echo -en "\t Cache L1i - " & lscpu | grep 'L1i' | awk '{print $3, $4}'
echo -en "\t Cache L2 - " & lscpu | grep 'L2' | awk '{print $3, $4}'
echo -en "\t Cache L3 - " & lscpu | grep 'L3' | awk '{print $3, $4}'
echo -en "\t Всего - " & free -h | grep 'Память' | awk '{print $2}'
echo -en "\t Доступно - " & free -h | grep 'Память' | awk '{print $4}'

echo "Жесткий диск:"
echo -en "\t Всего - " & df -h | grep '/dev/nvme0n1p2' | awk '{print $2}'
echo -en "\t Доступно - " & df -h | grep '/dev/nvme0n1p2' | awk '{print $4}'
echo -en "\t Количество разделов- " & lsblk -l | grep -c 'part'
echo -en "\t Объём неразмеченного пространства - " & sudo parted -l | grep '1049kB'| awk '{print $4}'
echo -en "\t Всего(раздел2) - " & df -h | grep '/dev/nvme0n1p1' | awk '{print $2}'
echo -en "\t Доступно(раздел2) - " & df -h | grep '/dev/nvme0n1p1' | awk '{print $4}'
echo -en "\t SWAP всего - " & free -h | grep 'Подкачка' | awk '{print $2}'
echo -en "\t SWAP доступно - " & free -h | grep 'Подкачка' | awk '{print $4}'

echo "Сетевые интерфейсы:"
networkNames=$(ip address show | awk '/^[0-9]+:/ { print $2 }' | sed 's|:||')

echo "Количество интерфейсов: $(echo $networkNames | wc -w)"

temp=$(mktemp)
num=0
for name in $networkNames; do
    num=$(($num + 1))
    mac=$(ip address show "$name" | grep 'link' | awk 'NR==1{print $2}')
    ip=$(ip address show "$name" | grep 'inet' | awk 'NR==1{print $2}')    
    echo "$num|$name|$mac|$ip" >> $temp
done
column -t -s '|' -N '#','Name','МАС','IP' $temp
rm $temp
