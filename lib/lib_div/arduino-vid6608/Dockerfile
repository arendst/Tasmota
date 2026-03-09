# Dockerfile to build platformio
FROM petrows/arduino-vid6608:latest

RUN <<PKG
  set -e
  apt-get -qq update
  apt-get install -y --no-install-recommends \
      python3 \
      python3-pip \
      python3-venv \

  apt-get -qq clean
  rm -rf /var/lib/apt/lists/*
PKG

# Python virtual env
ENV VIRTUAL_ENV=/var/venv
RUN mkdir -p -m 777 $VIRTUAL_ENV
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

# Default env
ENV HOME=/opt/home
RUN mkdir -p -m 777 $HOME

RUN pip install platformio==6.1.18

# Copy config file and enforce deps installation
COPY platformio.ini /tmp/
RUN <<PIO
  cd /tmp/
  # Install common packages
  platformio pkg install
  # Install required tools
  platformio pkg install -t platformio/tool-cppcheck
  platformio pkg install -t platformio/tool-clangtidy
  # Cleanup
  rm -rf /tmp/*
  # Allow this image to run under non-root
  chmod -R 777 $HOME
PIO

WORKDIR /app
