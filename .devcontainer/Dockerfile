FROM debian:bullseye-slim
#FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID


ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get -qqy install \
        # Common stuff
        apt-utils \
        build-essential \
        ca-certificates \
        curl \
        dialog \
        git \
        sudo \
        unzip \
        wget \
        # for platformio
        bc \
        build-essential \
        clang \
        curl \
        gcc \
        python3 \
        python3-dev \
        python3-distutils \
        python3-pip \
        srecord \
        xz-utils \
        # debugging
        libpython2.7 \
        libusb-1.0 \
        libelf1 \
        libncurses5 \
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
ENV DEBIAN_FRONTEND=dialog

RUN if [ ${USER_UID:-0} -ne 0 ] && [ ${USER_GID:-0} -ne 0 ]; then \
        groupadd --gid $USER_GID $USERNAME \
        && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
        && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME\
        && chmod 0440 /etc/sudoers.d/$USERNAME \
    ; fi

# https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules
RUN curl -fLo /etc/udev/rules.d/99-platformio-udev.rules --create-dirs https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules \
    && usermod -a -G dialout $USERNAME \
    && usermod -a -G plugdev $USERNAME

# Restart udev service to use the newly installed udev rules
#RUN service udev restart
# some OpenOCD req outdated libmpfr.so.4
RUN ln -s /usr/lib/x86_64-linux-gnu/libmpfr.so.6 /usr/lib/x86_64-linux-gnu/libmpfr.so.4

USER $USERNAME
RUN python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/develop/scripts/get-platformio.py)"

