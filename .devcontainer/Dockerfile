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

ARG USER_UID=1000
ARG USER_GID=1000

RUN id -u vscode 2>/dev/null || \
    useradd -m -s /bin/bash vscode && \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER vscode
WORKDIR /home/vscode

ENV PATH="/home/vscode/.local/bin:${PATH}"

RUN pipx install conan

RUN conan profile detect --force
