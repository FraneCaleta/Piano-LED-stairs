import pygame
import serial
import time

onpi = True

numpins = 8

previnputs = [False for a in range(0, numpins)]

if onpi:
    ser = serial.Serial('/dev/ttyACM0', 9600)

pygame.mixer.pre_init(channels=8, buffer=1024)
pygame.mixer.init()

letters = ["a", "b", "c", "d", "e", "f", "g", "h"]
piano_notes = [pygame.mixer.Sound("notes/"+letter+".wav") for letter in letters]

def piano(i):
    piano_notes[i].play()

while True:
    line = ""
    if onpi:
        line = ser.readline()
    else:
        line = raw_input()
    for i in range(numpins):
        curr = line[i] != '0'
        prev = previnputs[i]
        if curr and not prev:
            piano(i)
        previnputs[i] = curr