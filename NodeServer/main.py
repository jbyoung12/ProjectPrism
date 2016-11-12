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

writeData(array)

print "data sent"
