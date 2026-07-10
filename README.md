# Copacity

## Prerequisites

- Docker
- WSL
- Visual Studio Code
  - WSL (Extension)
  - Dev Containers (Extension)

## Build

First clone and build a local docker image from:
```sh
$ git clone https://github.com/ixuz/dev-container
$ cd dev-container
$ docker build -t dev-container:latest .
``` 

Clone and open this repository with **Visual Studio Code** and when prompted, click the _Reopen in Dev Container_ button.
```
$ git clone git@github.com:ixuz/copacity.git
$ cd copacity
$ code .
// When prompted, click the "Reopen in Dev Container" button.
```
