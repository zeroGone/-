import pygame
import serial

print("Mario SeungIk Start")

s = serial.Serial("/dev/ttyACM0", 9600)

pygame.mixer.init()
s.write(str.encode("Hello"))
try:
    while True:
        data = s.readline()
        print(data.decode('UTF-8'))
        if data == 2018:
            pygame.mixer.music.load("/home/pi/Music/Marionette.mp3")
            pygame.mixer.music.play(0,0)
        elif data == 666:
           break
except SerialException:
    s.close()
