from pwm import PWM

class Motor(object):

	'''* servoMin == 125, servoMax == 525ms *'''
	'''* minVal>=0, maxVal<=100 *'''
	def __init__(self, pinNumber, minVal=0, maxVal=100, centerVal=50,name="no name", pwm=None,servo_min=125,servo_max = 575):
		
		self.pin_number = pinNumber
		
		self.servo_min = servo_min
		self.servo_max = servo_max
		self.min = minVal
		self.max = maxVal
		self.center_value = centerVal
		
		self.name = name
		
		self.pwm = pwm
		
		self.moveTo(self.center_value)

	def moveTo(self,val):
		if val > self.min and val < self.max:
			self.value = val

		elif val > self.max:
			self.value = self.max
			print "val > self.max - Servo name: ",self.name

		else:
			self.value = self.min
			print "val < self.min - Servo name: ",self.name

		scaled_value = int(self.scale(self.value, self.min, self.max, self.servo_min,self.servo_max))
		self.pwm.setPWM(self.pin_number, 0, scaled_value)


	def moveOffset(self,deltaVal):
		val = self.value + deltaVal
		
		if val > self.min and val < self.max:
			self.value = val

		elif val > self.max:
			self.value = self.max
			print "deltaVal + self.value > self.max - Servo name: ",self.name

		else:
			self.value = self.min
			print "deltaVal + self.value < self.min - Servo name: ",self.name
		
		scaled_value = int(self.scale(self.value, self.min, self.max, self.servo_min,self.servo_max))
		self.pwm.setPWM(self.pin_number, 0, scaled_value)
		

	def scale(self,value, leftMin, leftMax, rightMin, rightMax):
		return rightMin + ((float(value - leftMin) / float((leftMax - leftMin))) * (rightMax - rightMin))

	def reset(self):
		self.moveTo(self.center_value)
