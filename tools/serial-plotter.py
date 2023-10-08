#!/usr/bin/env python3

"""
  serial-plotter.py - for Tasmota

  Copyright (C) 2021  Christian Baars

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

Requirements:
   - Python
   - pip3 install matplotlib pyserial
   - for Windows: Full python install including tkinter
   - a Tasmotadriver that plots

Instructions:
  expects serial data in the format:
    'PLOT: graphnumber value'
      graph (1-4)
      integer value
  Code snippet example: (last value will be ignored)
  AddLog_P2(LOG_LEVEL_INFO, PSTR("PLOT: %u, %u, %u,"),button_index+1, _value, Button.touch_hits[button_index]);

Usage:
    ./serial-plotter.py --port /dev/PORT --baud BAUD (or change defaults in the script)
    set output in tasmota, e.g.; TouchCal 1..4 (via Textbox)

"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import TextBox
import time
import serial
import argparse
import sys

print("Python version")
print (sys.version)

#default values
port = '/dev/cu.SLAB_USBtoUART'
baud = 115200

#command line input
parser = argparse.ArgumentParser()
parser.add_argument("--port", "-p", help="change serial port, default: " + port)
parser.add_argument("--baud", "-b", help="change baud rate, default: " + str(baud))
args = parser.parse_args()
if args.port:
    print("change serial port to %s" % args.port)
    port = args.port
if args.baud:
    print("change baud rate to %s" % args.baud)
    baud = args.baud


#time range
dt = 0.01
t = np.arange(0.0, 100, dt)

#lists for the data
xs = [0] #counting up x
ys = [[0],[0],[0],[0]] #4 fixed graphs for now
max_y = 1
# min_y = 0

fig = plt.figure('Tasmota Serial Plotter')
ax = fig.add_subplot(111, autoscale_on=True, xlim=(0, 200), ylim=(0, 20)) #fixed x scale for now, y will adapt
ax.grid()

line1, = ax.plot([], [], color = "r", label='G 1')
line2, = ax.plot([], [], color = "g", label='G 2')
line3, = ax.plot([], [], color = "b", label='G 3')
line4, = ax.plot([], [], color = "y", label='G 4')

time_template = 'time = %.1fs'
time_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)

ser = serial.Serial()
ser.port = port
ser.baudrate = baud
ser.timeout = 0 #return immediately
try:
  ser.open()
except:
  print("Could not connect to serial with settings: " + str(ser.port) + ' at ' + str(ser.baudrate) + 'baud')
  print("port available?")
  exit()

if ser.is_open==True:
  print("Serial Plotter started ...:")
  plt.title('connected to ' + str(ser.port) + ' at ' + str(ser.baudrate) + 'baud')
else:
  print("Could not connect to serial: " + str(ser.port) + ' at ' + str(ser.baudrate) + 'baud')
  plt.title('NOT connected to ' + str(ser.port) + ' at ' + str(ser.baudrate) + 'baud')

def init():
    line1.set_data([], [])
    line2.set_data([], [])
    line3.set_data([], [])
    line4.set_data([], [])
    time_text.set_text('')
    return [line1,line2,line3,line4,time_text ]  #was line


def parse_line(data_line):
  pos = data_line.find("PLOT:", 10)
  if pos<0:
    # print("wrong format")
    return 0,0

  raw_data = data_line[pos+6:]
  val_list = raw_data.split(',')
  try:
    g = int(val_list[0])
    v = int(val_list[1])
    return g, v
  except:
    return 0,0

def update(num, line1, line2):
    global xs, ys, max_y

    time_text.set_text(time_template % (num*dt) )

    receive_data = str(ser.readline())      #string

    g, v = parse_line(receive_data)
    if (g in range(1,5)):
      # print(v,g)
      if v>max_y:
        max_y = v
        print(max_y)
        ax.set_ylim([0, max_y * 1.2])

      idx = 0
      for y in ys:
        y.append(y[-1])
        if idx == g-1:
          y[-1] = v
        idx = idx +1
      xs.append(xs[-1]+1)

    if len(ys[0])>200:
        xs.pop()
        for y in ys:
          y.pop(0)
    line1.set_data(xs, ys[0])
    line2.set_data(xs, ys[1])
    line3.set_data(xs, ys[2])
    line4.set_data(xs, ys[3])
    return [line1,line2,line3,line4, time_text]

def handle_close(evt):
    print('Closing serial connection')
    ser.close()
    print('Closed serial plotter')

def submit(text):
    print (text)
    ser.write(text.encode() + "\n".encode())


ani = animation.FuncAnimation(fig, update, None, fargs=[line1, line2],
                  interval=10, blit=True, init_func=init)

ax.set_xlabel('Last 200 Samples')
ax.set_ylabel('Values')
plt.subplots_adjust(bottom=0.25)
ax.legend(loc='lower right', ncol=2)

fig.canvas.mpl_connect('close_event', handle_close)

axbox = plt.axes([0.15, 0.05, 0.7, 0.075])
text_box = TextBox(axbox, 'Send:', initial='')
text_box.on_submit(submit)

if ser.is_open==True:
  plt.show()
