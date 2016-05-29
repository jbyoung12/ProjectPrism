import smbus
import time
import random

# for RPI version 1, use bus = smbus.SMBus(0)
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeData(arrayData):
	for i in range(len(arrayData)):
		bus.write_byte(address,arrayData[i]);

ledVal = sys.argv[0]
verticalVal = sys.argv[1]
horizantalVal = sys.argv[2]

array = [ledVal,verticalVal,horizantalVal];

writeData(array)

print "data sent"
