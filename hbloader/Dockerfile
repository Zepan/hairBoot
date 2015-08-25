FROM ubuntu:14.04

RUN sudo apt-get update && \
    sudo apt-get install -y linux-libc-dev gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf make cmake && \
    sudo apt-get purge

VOLUME /workspace

WORKDIR /workspace
ENTRYPOINT ["/bin/sh", "-c"]

