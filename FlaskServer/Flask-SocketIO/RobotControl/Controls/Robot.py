import time
import json
from Motor import Motor
from pwm import PWM
from Leg import Leg

class Robot():	

	
	def __init__(self,frequency=50,file_name='data.json',servo_min = 125, servo_max = 575):
		
		self.frequency = frequency
		self.pwm = PWM()
		self.pwm.setPWMFreq(self.frequency)
		
		self.data_file_name = file_name
		
		self.servo_min = 125
		self.servo_max = 575
		
		self.front_left = None
		self.front_right = None
		self.back_left = None
		self.back_right = None
		
		self.setup()
		
		
	# loads json data and creates Motor objects with add_motor()
	def setup(self):
		with open(self.data_file_name) as data_file:		
			data = json.load(data_file)
			constants = data["constants"]
			for i in range(len(data["legs"])):
				self.add_leg(data["legs"][i],constants)
			
			
	# reads dictuanary values from input, creates a Leg object, and adds it to leg variables 
	def add_leg(self,legData,constants):
		leg_name = legData["name"]
		
		body_pin = legData["motors"]["body"]["pinValue"]
		mid_pin = legData["motors"]["middle"]["pinValue"]
		leg_pin = legData["motors"]["leg"]["pinValue"]
		
		body_offset = legData["motors"]["body"]["offset"]
		mid_offset = legData["motors"]["middle"]["offset"]
		leg_offset = legData["motors"]["leg"]["offset"]
		
		body_center = constants["bodyCenterValue"]
		middle_center = constants["middleCenterValue"]
		leg_center = constants["legCenterValue"]
		
		body_min = constants["bodyRange"]["min"]
		body_max = constants["bodyRange"]["max"]
		
		middle_min = constants["middleRange"]["min"]
		middle_max = constants["middleRange"]["max"]
		
		leg_min = constants["legRange"]["min"]
		leg_max = constants["legRange"]["max"]
		
		bodyCenter = body_center + body_offset
		middleCenter = middle_center + mid_offset
		legCenter = leg_center + leg_offset
		
		leg = Leg(self.pwm, self.servo_min, self.servo_max,leg_name, body_pin,bodyCenter, body_min, body_max, mid_pin, middleCenter, middle_min, middle_max, leg_pin, legCenter,leg_min, leg_max)
		
		if leg_name == "FR":
			self.front_right = leg
			
		elif leg_name == "FL":
			self.front_left = leg
			
		elif leg_name == "BL":
			self.back_left = leg
		
		elif leg_name == "BR":
			self.back_right = leg
			
		else:
			print "ERROR: LEG CANNOT BE IDENTIFIED"
	
		
	def stand(self):
		self.front_left.reset()
		self.front_right.reset()
		self.back_left.reset()
		self.back_right.reset()
