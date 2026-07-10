FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    sudo \
    git \
    build-essential \
    cmake \
    pkg-config \
    python3 \
    python3-pip \
    pipx \
    clang-format \
    mingw-w64 \
    g++-14 \
    && rm -rf /var/lib/apt/lists/*

USER ubuntu
WORKDIR /home/ubuntu

ENV PATH="/home/ubuntu/.local/bin:${PATH}"

RUN pipx install conan && \
    conan profile detect --force && \
    touch ~/.sudo_as_admin_successful
