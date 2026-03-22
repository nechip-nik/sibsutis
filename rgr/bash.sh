#!/bin/bash

# Проверка Minikube
if ! minikube status | grep -q "Running"; then
    echo "Minikube не запущен. Запускаю..."
    minikube start --memory=4096 --cpus=2 --driver=docker
fi

# Отключение Ingress если включен
minikube addons disable ingress 2>/dev/null

# Переключение Docker контекста
eval $(minikube docker-env)

# Сборка образа Flask
cd flask
docker build -t flask-app:latest .
cd ..

# Развертывание MariaDB
kubectl apply -f mariadb.yaml

# Ожидание запуска MariaDB
echo "Ожидание запуска MariaDB (60 секунд)..."
sleep 60

# Проверка статуса MariaDB
if kubectl get pod mariadb-0 2>/dev/null | grep -q "Running"; then
    # Инициализация базы данных
    kubectl exec -i mariadb-0 -- mysql -uroot -proot << 'EOF'
CREATE DATABASE IF NOT EXISTS books;
CREATE USER IF NOT EXISTS 'appuser'@'%' IDENTIFIED BY 'apppass';
GRANT ALL PRIVILEGES ON books.* TO 'appuser'@'%';
FLUSH PRIVILEGES;
EOF
    
    # Импорт дампа
    if [ -f mydb_dump.sql ]; then
        kubectl exec -i mariadb-0 -- mysql -uroot -proot books < mydb_dump.sql
    fi
else
    echo "MariaDB не запустилась"
    kubectl describe pod mariadb-0
    exit 1
fi

# Развертывание Flask
kubectl apply -f flask/flask-config.yaml
kubectl apply -f flask/flask.yaml
kubectl apply -f flask-service.yaml

# Развертывание Nginx
kubectl apply -f nginx-config.yaml
kubectl apply -f nginx.yaml
kubectl apply -f nginx-service.yaml

# Ожидание запуска всех компонентов
sleep 30

# Проверка состояния
kubectl get all

# Настройка доступа
MINIKUBE_IP=$(minikube ip)
echo "Доступ к приложению:"
echo "http://$MINIKUBE_IP:31115"
echo "Или через команду:"
minikube service nginx-service --url