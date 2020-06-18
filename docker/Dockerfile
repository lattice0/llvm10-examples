FROM ubuntu:bionic

WORKDIR /home/project

RUN export DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y cmake build-essential autoconf make zlib1g-dev python python3 git

RUN git clone https://github.com/llvm/llvm-project.git && cd llvm-project && mkdir build && cd build && cmake -G "Unix Makefiles" ../llvm

RUN cd llvm-project && cd build && make

RUN cd llvm-project && cd build && make install