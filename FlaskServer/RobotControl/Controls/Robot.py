from pwm import PWM
from I2C import Adafruit_I2C
from RobotUtil import RobotUtils
from Leg import Leg
import time,math,json

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
		self.testSetXY()
		
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
	
	# Refer to stand()
	def reset(self):
		self.stand()	
	
	# resets legs to default position
	def stand(self):
		self.front_left.reset()
		self.front_right.reset()
		self.back_left.reset()
		self.back_right.reset()

	# tester method for Leg.setLegXY
	def testSetXY(self):
		

		print "Enter x value"
		x = int(raw_input(" >>> "))
		print "Enter y value"
		y = int(raw_input(" >>> "))
		print ""
		self.front_left.setLegXY(x,y)
			
		'''
		for x in range(20):		
			x = x + 1
			#for y in range(20):
			y = -5
			print "Testing : (",x,",",y,")"
			self.front_left.setLegXY(x,y)
			print ""
			time.sleep(.5)
			print ""
			print " ------------------------- "
			print ""
		
		time.sleep(1)
		'''
		self.testSetXY()
		


	# method to develop walking motion
	def testWalk(self):
		
		std_piv_step_body_delta = -25
		std_piv_step_middle_delta = 50
		std_piv_step_leg_delta = 5
		velocity = .001
		time_delay = .5
		
		self.front_left.standardPivotStep(std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
		time.sleep(time_delay)
		self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
		time.sleep(time_delay)
		
		leg_extend_body_delta = 25
		leg_extend_middle_delta =-15
		leg_extend_leg_delta = 15
		
		self.front_right.legExtend( leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
		time.sleep(time_delay*4)
		self.reset()
		self.testWalk()



