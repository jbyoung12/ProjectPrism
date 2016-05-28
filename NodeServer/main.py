import smbus
import time
import random

# for RPI version 1, use bus = smbus.SMBus(0)
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

#bus.write_byte(address,260)


def writeData(arrayData):
	for i in range(len(arrayData)):
		bus.write_byte(address,arrayData[i]);

array = [1,random.randint(10,170),random.randint(10,170)];

writeData(array)
