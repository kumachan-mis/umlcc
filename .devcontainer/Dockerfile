# Image contents:
# https://github.com/microsoft/vscode-dev-containers/tree/v0.231.6/containers/cpp/.devcontainer/base.Dockerfile

# [Choice]
# Debian / Ubuntu version (use Debian 11, Ubuntu 18.04/21.04 on local arm64/Apple Silicon):
# debian-11, debian-10, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04

ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional]
# Install CMake version different from what base image has already installed. 
# CMake reinstall choices: none, 3.21.5, 3.22.2, or versions from https://cmake.org/download/

ARG CMAKE_VERSION="default"
COPY ./reinstall-cmake.sh /tmp/
RUN if [ ${CMAKE_VERSION} != "default" ]; \
    then \
        chmod +x /tmp/reinstall-cmake.sh && \
        /tmp/reinstall-cmake.sh ${CMAKE_VERSION}; \
    fi && \
    rm -f /tmp/reinstall-cmake.sh

# [Optional]
# Install additional vcpkg ports.

# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional]
# Install additional packages.

ARG PACKAGES=""
RUN if [ "${PACKAGES}" != "" ]; \
    then \
        apt-get update && \
        export DEBIAN_FRONTEND=noninteractive && \
        apt-get -y install --no-install-recommends ${PACKAGES}; \
    fi
