FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc
COPY . /AVM


WORKDIR /AVM
RUN make
CMD ["./main", "-show","test.txt"]
