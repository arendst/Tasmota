from RF24 import *
from RF24Network import *
from RF24Mesh import *


# radio setup for RPi B Rev2: CS0=Pin 24
radio = RF24(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)

mesh.setNodeID(0)
mesh.begin(108, RF24_250KBPS)
radio.setPALevel(RF24_PA_MAX) # Power Amplifier
radio.printDetails()

while 1:
    mesh.update()
    mesh.DHCP()

    while network.available():
        print("Received message")
        header, payload = network.read(10)
