#!/usr/bin/env python

import smbus
import time
import random
import sys


# [ ledState,autonomousState, horizontalVideoValue, verticalVideoValue, xMovement, yMovement]

# for RPI version 1, use bus = smbus.SMBus(0)
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeData(arrayData):
	for i in range(len(arrayData)):
		bus.write_byte(address,arrayData[i]);

array = []
for i in range(len(sys.argv)-1):
	print sys.argv[i+1]

<<<<<<< HEAD
ledVal = int(sys.argv[1])
rotateCameraVal = float(sys.argv[2])
cameraSpeedVal = float(sys.argv[3])
moveSideVal : float(sys.argv[4])
moveForVal : float(sys.argv[5])

array = [ledVal,rotateCameraVal,cameraSpeedVal,moveSideVal,moveForVal];
print array
=======
>>>>>>> d3dbe35858e7c362521e9fc3e1d8000ca9956038
writeData(array)

print "data sent"
