#!/bin/bash

sudo docker compose up -d
sudo docker compose exec apachebench ab -n 30000 -c 100 nginx/
#sudo docker compose exec cat "cat /var/log/nginx/access.log"
#sudo docker compose down