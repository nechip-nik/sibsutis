#!/bin/bash

sudo docker run -it -d --name web-server -p 3000:80 nginx

sudo docker network create my-network
sudo docker network connect my-network web-server

sleep 5
ab -n 1000 -c 10 http://localhost:3000/ > ab_output.txt

sudo docker stop web-server
sudo docker rm web-server
sudo docker network rm my-network