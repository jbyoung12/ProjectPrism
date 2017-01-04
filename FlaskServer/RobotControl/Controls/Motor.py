from RobotUtil import RobotUtils
import time,math,json

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
		self.value = centerVal
		
		self.name = name
		
		self.pwm = pwm
		
		self.moveTo(self.center_value)

	def moveTo(self,val):
		if val > self.min and val < self.max:
			self.value = val

		elif val >= self.max:
			self.value = self.max

		elif val <= self.min:
			self.value = self.min
		
		else:
			print "something strange is happeneing..."
		
		scaled_value = int(RobotUtils.scale(self.value, RobotUtils.MIN_MOTOR_VALUE, RobotUtils.MAX_MOTOR_VALUE,  self.servo_min,self.servo_max))
		self.pwm.setPWM(self.pin_number, 0, scaled_value)

	def moveToInT(self,val,t):
		
		delta = self.value - val
		
		if val > self.min and val < self.max:
			self.value = val

		elif val >= self.max:
			self.value = self.max

		elif val <= self.min:
			self.value = self.min
		
		else:
			print "something strange is happeneing..."

		self.moveOffSetInT(delta,t)
		
		
	def moveOffSetInT(self,deltaVal,t):
		for i in range(int(math.fabs(deltaVal))):
			self.moveOffset( RobotUtils.PositiveOrNegative(deltaVal) ) 
			time.sleep(t)
	
	

	def moveOffset(self,deltaVal):
		val = self.value + deltaVal

		if val > self.min and val < self.max:
			self.value = val

		elif val >= self.max:
			self.value = self.max

		elif val <= self.min:
			self.value = self.min
		
		else:
			print "ERROR, very odd indeed"
		
		scaled_value = int(RobotUtils.scale(self.value, RobotUtils.MIN_MOTOR_VALUE, RobotUtils.MAX_MOTOR_VALUE, self.servo_min,self.servo_max))
		self.pwm.setPWM(self.pin_number, 0, scaled_value)
		


	def reset(self):
		self.moveTo(self.center_value)


