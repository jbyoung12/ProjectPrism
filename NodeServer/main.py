import smbus
import time
import random
import sys

# for RPI version 1, use bus = smbus.SMBus(0)
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeData(arrayData):
	for i in range(len(arrayData)):
		bus.write_byte(address,arrayData[i]);


ledVal = int(sys.argv[1])
verticalVal = int(sys.argv[2])
horizantalVal = int(sys.argv[3])

array = [ledVal,verticalVal,horizantalVal];
print array
writeData(array)

print "data sent"
