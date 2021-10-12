#!/bin/bash

echo "Дата $(date '+%D');"
echo "Имя учетной записи $(whoami);"
echo "Доменное имя ПК $(hostname -d);"
echo "Процессор:"
data=$(lscpu | grep 'Имя модели')
echo "Модель: ${data:21:38}"
data=$(lscpu | grep 'Архитектура')
echo "Архитектура ${data:20}"
data=$(lscpu | grep 'CPU MHz')
echo "Тактовая частота ${data:21:16}"
data=$(lscpu | grep 'Ядер на сокет')
echo "Количество ядер ${data:21}"
data=$(lscpu | grep 'Thread(s) per core')
echo "Потоков на ядро ${data:21}"
echo "Оперативна память:"
data=$(free | grep 'Mem')
echo "Всего ${data:12:10}"
echo "Доступно ${data:36:10}"
echo "Жесткий диск:"
data=$(lsblk /dev/sda | grep sda)
echo "Всего ${data:18:6}"
data=$(df -h | grep /dev/sda1)
echo "Доступно ${data:43:6}"
data=$(df -h | grep /\$)
echo "Смонтировано в корневую директорию ${data:0:11}"
data=$(free | grep 'Swap')
echo "SWAP всего ${data:12:10}"
echo "SWAP доступно ${data:36:10}"
echo "Сетевые интерфейсы:"
data=$(ifconfig -s | wc -l)
diff=1
echo "Количество интерфейсов $(($data - $diff))"
echo "№ Имя             MAC адрес           IP адрес        Скорость"
for (( counter=1, counter2=2, counter3=1; counter < $((data)); counter++,counter2++,counter3+=2))
do
    data1=$(ifconfig -s | head -n $counter2 | tail -n 1 )
    data2=$(ifconfig -a | grep ether | head -n $counter | tail -n 1)
    data3=$(ifconfig -a | grep inet | head -n $counter3 | tail -n 1)
    echo "$counter ${data1:0:7}      ${data2:14:18}    ${data3:13:13}"
done
