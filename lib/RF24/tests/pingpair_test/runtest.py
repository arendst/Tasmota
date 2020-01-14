#!/opt/local/bin/python

import sys,serial

def read_until(token):
	while 1: 
		line = ser.readline(None,"\r")
		sys.stdout.write(line)

		if (line.startswith(token)):
			break
	return line


ser = serial.Serial(sys.argv[1], 57600, timeout=5, dsrdtr=False, rtscts=False)

read_until("+READY")
ser.write(sys.argv[2])

line = read_until("+OK")
ser.close()
if (line.find("PASS") != -1):
	sys.exit(0)
else:
	sys.exit(1)
