FROM python:2

LABEL author="Eduard Angold"

# Install platformio. To be able to build tasmota <=v6.6.0 (and later)
# we have to use version 3.6.7 of platformio.
RUN pip install --upgrade pip &&\ 
    pip install -U platformio==3.6.7

# Init project
COPY init_pio_tasmota /init_pio_tasmota

# Install project dependencies using a init project.
RUN cd /init_pio_tasmota &&\ 
    pio run &&\
    cd ../ &&\ 
    rm -fr init_pio_tasmota &&\ 
    cp -r /root/.platformio / &&\ 
    chmod -R 777 /.platformio

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/bin/bash", "/entrypoint.sh"]

