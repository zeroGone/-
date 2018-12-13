import pygame

print("Audio Test")

pygame.mixer.init()

pygame.mixer.music.load("/home/pi/Music/Marionette.mp3")
pygame.mixer.music.play(0,0)

