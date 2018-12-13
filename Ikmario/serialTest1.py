import serial
s = serial.Serial("/dev/ttyACM0", 9600)
s.close()
s.open()
s.write(str.encode('1'))
