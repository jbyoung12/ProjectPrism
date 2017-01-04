from pwm import PWM
from I2C import Adafruit_I2C
from RobotUtil import RobotUtils
from Leg import Leg
import time,math,json

class Robot():	

	def __init__(self,file_name,leg_debug, motor_debug):
		
		self.leg_debug = leg_debug
		self.motor_debug = motor_debug
		
		self.frequency = RobotUtils.FREQUENCY
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
		
		self.stand()
		self.testWalk()
		
	# loads json data and creates Leg objects with add_leg()
	def setup(self):
		with open(self.data_file_name) as data_file:		
			data = json.load(data_file)
			constants = data["constants"]
			for i in range(len(data["legs"])):
				self.add_leg(data["legs"][i],constants)
		
		
		
			
	# reads dictuanary values from input, creates a Leg object, and adds it to leg variables 
	def add_leg(self,legData,constants):
		
		leg_name = legData["name"]
				
		body_pin 				= legData["motors"]["body"]["pinValue"]
		body_offset 			= legData["motors"]["body"]["offset"]
		body_center 			= constants["bodyCenterValue"] + body_offset
		body_min 				= constants["bodyRange"]["min"]
		body_max 				= constants["bodyRange"]["max"]
		
		
		mid_horiz_value 		=  legData["motors"]["middle"]["horizValue"]
		middle_pin 				= legData["motors"]["middle"]["pinValue"]
		middle_min 				= constants["middleRange"]["min"]
		middle_max 				= constants["middleRange"]["max"]
		middle_offset_to_center = constants["midOffsetFromHoriz"]
		
		leg_horiz_value 		= legData["motors"]["leg"]["horizValue"]
		leg_pin 				= legData["motors"]["leg"]["pinValue"]
		leg_min 				= constants["legRange"]["min"]
		leg_max 				= constants["legRange"]["max"]
		leg_offset_to_center 	= constants["legOffsetFromHoriz"]
		
		leg = Leg(	self.leg_debug, self.motor_debug, self.pwm, leg_name, body_pin,	body_min,	body_max,	body_center, mid_horiz_value, 	middle_pin,	middle_min,	middle_max,	middle_offset_to_center, leg_horiz_value, 	leg_pin,	leg_min,	leg_max,	leg_offset_to_center)
			
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
		
		testLeg = self.front_right
		
		mid = testLeg.middle
		leg = testLeg.leg
		
		#leg.moveToInT(50,2)
		midRange = (mid.max - mid.min)
		print "midRange: ",midRange
		
		print "beggining sweep"
		for i in range(midRange):
			x = i + mid.min
			mid.moveTo(x)
			print "setting :",mid.name," to : ",x
			time.sleep(.1)

	def setAllHoriz(self):
		self.front_right.setMidAndLegHoriz()
		self.front_left.setMidAndLegHoriz()
		self.back_right.setMidAndLegHoriz()
		self.back_left.setMidAndLegHoriz()
		time.sleep(5)

	def setMidsToMin(self):
		self.front_right.middle.moveTo(self.front_right.middle.min)
		self.front_left.middle.moveTo(self.front_left.middle.min)
		self.back_left.middle.moveTo(self.back_left.middle.min)
		self.back_right.middle.moveTo(self.back_right.middle.min)
		time.sleep(10)
		
	def setMidsToMax(self):
		self.front_right.middle.moveTo(self.front_right.middle.max)
		self.front_left.middle.moveTo(self.front_left.middle.max)
		self.back_left.middle.moveTo(self.back_left.middle.max)
		self.back_right.middle.moveTo(self.back_right.middle.max)
		

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



