from pwm import PWM
from Motor import Motor

class Leg(object):

	'''* servoMin == 125 ticks, servoMax == 525 ticks *'''
	def __init__(self,pwm,servo_min,servo_max,name, bodyPin, bodyCenter,bodyMin,bodyMax,     middlePin, middleCenter, middleMin,middleMax,    legPin, legCenter, legMin, legMax):
		
		self.pwm = pwm
		self.servo_min = servo_min
		self.servo_max = servo_max
		
		self.name = name
		
		self.body_name = "body"+name
		self.middle_name = "middle"+name
		self.leg_name = "leg"+name
		
		self.body = Motor(bodyPin,bodyMin,bodyMax,bodyCenter,self.body_name,pwm,servo_min, servo_max)
		self.middle = Motor(middlePin,middleMin,middleMax, middleCenter,self.middle_name,pwm,servo_min, servo_max)
		self.leg = Motor(legPin,legMin,legMax, legCenter,self.leg_name,pwm,servo_min, servo_max)

	def reset(self):
		self.body.reset()
		self.middle.reset()
		self.leg.reset()
