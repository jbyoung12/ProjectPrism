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

		self.front_left = None
		self.front_right = None
		self.back_left = None
		self.back_right = None

		self.xMovement 	= 50
		self.yMovement 	= 50

		self.forwardIncMax 	=	15
		self.forwardInc 	=	1
		self.forwardIncMin 	=	1

		self.backwardIncMax 	=	15
		self.backwardInc 		=	1
		self.backwardIncMin 	=	1

		self.movement_threshold  = 3

		self.stop 	  	= False
		self.autonomous = False

		self.horizVidValue 	= 50
		self.horizVidPin = 4
		self.horizVidMinVal = 0
		self.horizVidMaxVal = 100

		self.vertVidValue 	= 50
		self.vertVidPin = 8

		self.horizVidMotor = Motor(False, self.horizVidValue, self.horizVidPin, self.horizVidMinVal, self.horizVidMaxVal, 0,"horizantal video motor", self.pwm)
		self.vertVidMotor = Motor(False, self.vertVidValue, self.vertVidPin, self.vertVidMinVal, self.vertVidMaxVal, 0,"vertical video motor", self.pwm)

		self.setup()

		self.stand()
		time.sleep(3)
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

	# {u'data': {u'xMovement': u'50.00', u'stop': False, u'horizontalVideo': u'24.02', u'yMovement': u'50.00', u'verticalVideo': u'50.00', u'autonomous': False}}
	def inputData(self,data):
		json_data 	= json.load(data)
		self.xMovement 	= json_data["data"]["xMovement"]
		self.yMovement 	= json_data["data"]["yMovement"]
		self.horizVidValue 	= json_data["data"]["horizontalVideo"]
		self.vertVidValue 	= json_data["data"]["verticalVideo"]
		self.stop 	  	= json_data["data"]["stop"]
		self.autonomous 	= json_data["data"]["autonomous"]
		self.updateAgenda()

	# acts as central coordinator for the robot - raeads incoming data + state of the bot and calls methods accordingly
	def updateAgenda(self):

		# if stop is called, the bot freezes in standing pose
		if not self.stop:

			# bot is autonomous, which has not been built yet, so we stand. Or do the mamba.
			if self.autonomous:
				self.stand()

			# bot is teleop mode
			else:

				# update camera motors
				self.horizVidMotor.moveTo(self.horizVidValue)
				self.vertVidMotor.moveTo(self.vertVidValue)

				xMagnitude = abs(self.xMovement - 50)
				yMagnitude = abs(self.yMovement - 50)

				# filter out value fluctuation by ensuring movment commands are past a certain threshold
				if abs(xMagnitude - 50 ) < self.movement_threshold and abs(yMagnitude - 50) < self.movement_threshold:

					# command to move in the x axis rank higher in importance than command to move in y axis
					if xMagnitude > yMagnitude:
						# if xMovement is greater than 50 than we move left
						if self.xMovement > 50:
							self.turn(1)

						# turn left
						elif self.xMovement <= 50:
							self.turn(-1)

						else:
							print "logic error. This should not ever be printed"

					# command to move in the y axis rank higher in importance than command to move in x axis
					elif yMagnitude > xMagnitude:

						# move forward
						if self.yMovement > 50:

							# perform next segment of forward walk
							self.forward(self.forwardInc)

							# increment forward walk increment variable
							self.forwardInc += 1

							# test to see if forward walk incrment variable has reached maximum value and reset it to min value if it has
							if self.forwardInc == self.forwardIncMax:
								self.forwardInc = self.forwardIncMin

							# reset the backward incrementer, because the backward motion has been stopped, and needs to reset
							self.backwardInc = self.backwardIncMin

						# move backward
						elif self.yMovement < 50:

							# perform next segment of backward walk
							self.backward(self.backwardInc)

							# increment backward walk increment variable
							self.backwardInc += 1

							# test to see if backward walk incrment variable has reached maximum value and reset it to min value if it has
							if self.backwardInc == self.backwardIncMax:
								self.backwardInc = self.backwardIncMin

							# reset the forward incrementer, because the forward motion has been stopped, and needs to reset
							self.forwardInc = self.backwardIncMin

				else:
					print "movment command to small in intensity - classified as noise and filtered"


		else:
			self.stand()


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

		std_piv_step_body_delta = -20
		std_piv_step_middle_delta = 50
		std_piv_step_leg_delta = 5
		velocity = .01
		time_delay = .025

		print "front left pivot step"
		self.front_left.standardPivotStep(std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay*.01)
		time.sleep(time_delay)

		print "back right pivot step"
		self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
		time.sleep(time_delay)

		leg_extend_body_delta = 35
		leg_extend_middle_delta =-5
		leg_extend_leg_delta = 28

		print "front right leg extend"
		self.front_right.legExtend( leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
		time.sleep(time_delay)

		splitNum = 10
		leg_condense_FLbody_delta = 40/splitNum
		leg_condense_BRbody_delta = -20/splitNum
		leg_condense_FRmiddle_delta = 20/splitNum
		leg_condense_FRleg_delta = -28/splitNum
		leg_condense_BLbody_delta = 20/splitNum
		leg_condense_BLmiddle_delta = -10/splitNum
		leg_condense_BLleg_delta = 28/splitNum

		print "condense forward right"
		for x in range(0, splitNum):
			self.front_left.body.moveOffset(leg_condense_FLbody_delta)
			self.back_right.body.moveOffset(leg_condense_BRbody_delta)
			self.front_right.middle.moveOffset(leg_condense_FRmiddle_delta)
			self.front_right.leg.moveOffset(leg_condense_FRleg_delta)
			self.back_left.body.moveOffset(leg_condense_BLbody_delta)
			self.back_left.middle.moveOffset(leg_condense_BLmiddle_delta)
			self.back_left.leg.moveOffset(leg_condense_BLleg_delta)

		leg_step_BLbody_delta = -30
		leg_step_BLmiddle_delta = 30
		leg_step_BLleg_delta = -28
		time.sleep(time_delay)

		print "back left standard pivot step with mid offset"
		self.back_left.standardPivotStepWithMidMovement(leg_step_BLbody_delta, leg_step_BLmiddle_delta, leg_step_BLleg_delta,velocity,time_delay)

		leg_step_FRbody_delta = -40
		leg_step_FRmiddle_delta = 5
		leg_step_FRleg_delta = 28

		print "front left standard pivot step with mid movement"
		self.front_left.standardPivotStepWithMidMovement(leg_step_FRbody_delta, leg_step_FRmiddle_delta, leg_step_FRleg_delta, velocity,time_delay)
		time.sleep(time_delay)

		frontRightBodySplitDiff = self.front_right.body.center_value - self.front_right.body.value
		frontRightMiddleSplitDiff =self.front_right.middle.value - self.front_right.middle.center_value
		frontRightLegSplitDiff = self.front_right.leg.value - self.front_right.leg.center_value

		frontLeftBodySplitDiff = self.front_left.body.center_value - self.front_left.body.value
		frontLeftMiddleSplitDiff =self.front_left.middle.center_value  - self.front_left.middle.value
		frontLeftLegSplitDiff = self.front_left.leg.center_value - self.front_left.leg.value

		backRightBodySwing = -20/splitNum
		backRightMiddleSwing = -10/splitNum
		backRightLegSwing = 28/splitNum
		backLeftBodySwing = 40/splitNum

		print "forward condence"
		for x in range(0, splitNum):
			self.front_right.body.moveOffset(frontRightBodySplitDiff/splitNum)
			self.front_right.middle.moveOffset(frontRightMiddleSplitDiff/splitNum)
			self.front_right.leg.moveOffset(frontRightLegSplitDiff/splitNum)
			#spacer
			#self.front_left.body.moveOffset(frontLeftBodySplitDiff/splitNum)
			self.front_left.middle.moveOffset(frontLeftMiddleSplitDiff/splitNum)
			self.front_left.leg.moveOffset(frontLeftLegSplitDiff/splitNum)

			self.back_right.body.moveOffset(backRightBodySwing)
			self.back_right.middle.moveOffset(backRightMiddleSwing)
			self.back_right.leg.moveOffset(backRightLegSwing)
			self.back_left.body.moveOffset(backLeftBodySwing)

		time.sleep(time_delay)

		print "front right reset"

		self.front_right.middle.moveOffSetInT(40,velocity)
		self.front_right.reset()

		time.sleep(4)
		self.back_right.standardPivotStep(30,50,10, velocity,0)
		time.sleep(4)

		'''
		backLeftResetMoveBody = self.back_left.body.center_value - self.back_left.body.value
		backLeftResetMoveMiddle = self.back_left.middle.value - self.back_left.middle.center_value
		backLeftResetMoveLeg = self.back_left.leg.center_value - self.back_left.leg.value



		print "back left standard pivot step with mid movement"
		self.back_left.standardPivotStep(backLeftResetMoveBody, backLeftResetMoveMiddle, backLeftResetMoveLeg, velocity,time_delay)
		time.sleep(time_delay)
		backRightResetMoveBody = self.back_right.body.center_value - self.back_right.body.value
		backRightResetMoveMiddle = self.back_right.middle.center_value - self.back_right.middle.value
		backRightResetMoveLeg = self.back_right.leg.center_value - self.back_right.leg.value

		print "back right standard pivot step with mid movement"
		self.back_right.standardPivotStepWithMidMovement(backRightResetMoveBody, backRightResetMoveMiddle, backRightResetMoveLeg, velocity,time_delay)

		time.sleep(time_delay)

		frontLeftBodySplitDiff = self.front_left.body.center_value - self.front_left.body.value
		frontLeftMiddleSplitDiff = self.front_left.middle.center_value  - self.front_left.middle.value
		frontLeftLegSplitDiff = self.front_left.leg.center_value - self.front_left.leg.value

		print "front left standard pivot step with mid movement"
		self.front_left.standardPivotStepWithMidMovement(frontLeftBodySplitDiff, frontLeftMiddleSplitDiff, frontLeftLegSplitDiff, velocity,time_delay)
		time.sleep(time_delay)
		'''
		self.reset()
