#!/usr/bin/python
from Services import *
import time
import math
import json
import sys
import threading
import os
from Queue import Queue,Empty

class Robot():

	def __init__(self):

		if RobotUtils.LIVE_TESTING:
			self.pwm = PWM()
			self.pwm.setPWMFreq(RobotUtils.FREQUENCY)
		else:
			self.pwm = None

		self.inputQueue = Queue()
		self.agendaThread = threading.Thread(group=None,target=self.updateAgendaLoop,name="agendaThread")
		#self.agendaThread.start()

		self.data_file_name = RobotUtils.DATA_FILE

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


		# Horizantal Video Servo Data
		self.horizVidValue 	= 50
		self.horizVidPin = 4
		self.horizVidMinVal = 0
		self.horizVidMaxVal = 100

		# Vertical Video Servo Data
		self.vertVidValue 	= 50
		self.vertVidPin = 8
		self.vertVidMinVal = 0
		self.vertVidMaxVal = 100

		self.horizVidMotor = Motor(self.horizVidValue, self.horizVidPin, self.horizVidMinVal, self.horizVidMaxVal, 0,"horizantal video motor", self.pwm)
		self.vertVidMotor = Motor( self.vertVidValue, self.vertVidPin, self.vertVidMinVal, self.vertVidMaxVal, 0,"vertical video motor", self.pwm)

		self.setup()
		self.stand()
		print '\033[93m' + "Robot Created. this was printed from robot class of number: " +str(id(self))+ '\033[0m'
		self.updateAgenda()


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

		leg = Leg( self.pwm, leg_name, body_pin,	body_min,	body_max,	body_center, mid_horiz_value, 	middle_pin,	middle_min,	middle_max,	middle_offset_to_center, leg_horiz_value, 	leg_pin,	leg_min,	leg_max,	leg_offset_to_center)

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

	# Called by server when a change in user data is detected
	def inputData(self,data):
		self.inputQueue.put(data)

	def processData(self,data):
		self.xMovement 		= float(data["data"]["xMovement"])
		self.yMovement	 	= float(data["data"]["yMovement"])
		self.horizVidValue 	= float(data["data"]["horizontalVideo"])
		self.vertVidValue 	= float(data["data"]["verticalVideo"])
		self.stop 	  		= data["data"]["stop"]
		self.autonomous 	= data["data"]["autonomous"]

	def updateAgendaLoop(self):
		n = 100
		while True:
			try:
				data = self.inputQueue.get_nowait()
				self.processData(data)
				self.updateAgenda()
			except Empty:
				print "pass"
				pass

			time.sleep(RobotUtils.AGENDA_UPDATE_SPEED)
			n -= 1
		print '\033[94m' + "Robot: QUEUE READING FINISHED" + '\033[0m'
		sys.exit()

	# acts as central coordinator for the robot - raeads incoming data + state of the bot and calls methods accordingly
	def updateAgenda(self):

		print "in updateAgeda()"
		# if stop is called, the bot freezes in standing pose
		if not self.stop:

			# bot is autonomous, which has not been built yet, so we stand. Or do the mamba.
			if self.autonomous:
				self.stand()
				print '\033[94m' + "Robot: AUTONOMOUS" + '\033[0m'

			# bot is teleop mode
			else:
				# update camera motors
				self.horizVidMotor.moveTo(self.horizVidValue)
				self.vertVidMotor.moveTo(self.vertVidValue)

				xMagnitude = abs(self.xMovement - 50)
				yMagnitude = abs(self.yMovement - 50)

				# filter out value fluctuation by ensuring movment commands are past a certain threshold. Movement commands must be greater than 50 +- threshold to perform a command
				if ( xMagnitude  > self.movement_threshold) or ( yMagnitude  > self.movement_threshold):

					# command to move in the x axis rank higher in importance than command to move in y axis
					if xMagnitude > yMagnitude:

						# if xMovement is greater than 50 than we move left
						if self.xMovement < 50:
							print '\033[95m' + "Robot: LEFT" + '\033[0m \n'
							self.turn(1)
							self.forwardInc = self.forwardIncMin

						# turn left
						elif self.xMovement >= 50:
							self.turn(-1)
							print '\033[95m' + "Robot: RIGHT" + '\033[0m \n'
							self.forwardInc = self.forwardIncMin

						else:
							print "logic error. This should not ever be printed"

					# command to move in the y axis rank higher in importance than command to move in x axis
					elif yMagnitude > xMagnitude:

						# move forward
						if self.yMovement > 50:

							print '\033[95m' + "Robot: FORWARD" + '\033[0m \n'
							# perform next segment of forward walk
							self.walkInit()


							# increment forward walk increment variable
							self.forwardInc += 1

							# test to see if forward walk incrment variable has reached maximum value and reset it to min value if it has
							if self.forwardInc == self.forwardIncMax:
								self.forwardInc = self.forwardIncMin

							# reset the backward incrementer, because the backward motion has been stopped, and needs to reset
							self.backwardInc = self.backwardIncMin

						# move backward
						elif self.yMovement <= 50:
							print '\033[95m' + "Robot: BACKWARD" + '\033[0m \n'

						else:
							print "logic error. this should not be printed"

				else:
					print "\033[94m","Robot: STAND","\033[0m"
					self.stand()

		else:
			print "\033[94m","Robot: STOP \033[0m"
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
			self.time.sleep(.1)

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

	def turn(self,direction):

		if(direction > 0):
			turnDegree = 20
		else:
			turnDegree = -20

		stepHeightMid = 60
		stepHeightLeg = 5
		velocity = 0.002
		time_delay = 0
		self.front_right.standardPivotStep(turnDegree, stepHeightMid, stepHeightLeg, velocity, time_delay)
		time.sleep(time_delay)

		self.back_left.standardPivotStep(turnDegree, stepHeightMid, stepHeightLeg, velocity, time_delay)
		time.sleep(time_delay)

		self.front_left.standardPivotStep(turnDegree, stepHeightMid, stepHeightLeg,velocity,time_delay)
		time.sleep(time_delay)

		self.back_right.standardPivotStep(turnDegree, stepHeightMid, stepHeightLeg, velocity, time_delay)
		time.sleep(time_delay)
		self.reset()



	def lunge(self, FRB, FRM, FRL, FLB, FLM, FLL, BLB, BLM, BLL, BRB, BRM, BRL):
		splitNum = 10
		for x in range(splitNum):
			self.front_right.body.moveOffset(FRB/splitNum)
			self.front_right.middle.moveOffset(FRM/splitNum)
			self.front_right.leg.moveOffset(FRL/splitNum)

			self.front_left.body.moveOffset(FLB/splitNum)
			self.front_left.middle.moveOffset(FLM/splitNum)
			self.front_left.leg.moveOffset(FLL/splitNum)

			self.back_left.body.moveOffset(BLB/splitNum)
			self.back_left.middle.moveOffset(BLM/splitNum)
			self.back_left.leg.moveOffset(BLL/splitNum)

			self.back_right.body.moveOffset(BRB/splitNum)
			self.back_right.middle.moveOffset(BRM/splitNum)
			self.back_right.leg.moveOffset(BRL/splitNum)


	def forward(self):
		velocity = .01
		time_delay = .025
		std_piv_step_body_delta = -20
		std_piv_step_middle_delta = 50
		std_piv_step_leg_delta = 5

		self.front_left.standardPivotStep(std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay*.01)
		time.sleep(time_delay)

		self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
		time.sleep(time_delay)

		self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
		time.sleep(time_delay)

		leg_extend_body_delta 	= 35
		leg_extend_middle_delta = -5
		leg_extend_leg_delta 	= 28

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


		# condense forward right
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

		# back left standard pivot step with mid offset"
		self.back_left.standardPivotStepWithMidMovement(leg_step_BLbody_delta, leg_step_BLmiddle_delta, leg_step_BLleg_delta,velocity,time_delay)

		leg_step_FRbody_delta = -40
		leg_step_FRmiddle_delta = 5
		leg_step_FRleg_delta = 28

		# front left standard pivot step with mid movement"
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

		# forward condence
		for x in range(0, splitNum):
			self.front_right.body.moveOffset(frontRightBodySplitDiff/splitNum)
			self.front_right.middle.moveOffset(frontRightMiddleSplitDiff/splitNum)
			self.front_right.leg.moveOffset(frontRightLegSplitDiff/splitNum)

			#self.front_left.body.moveOffset(frontLeftBodySplitDiff/splitNum)
			self.front_left.middle.moveOffset(frontLeftMiddleSplitDiff/splitNum)
			self.front_left.leg.moveOffset(frontLeftLegSplitDiff/splitNum)

			self.back_right.body.moveOffset(backRightBodySwing)
			self.back_right.middle.moveOffset(backRightMiddleSwing)
			self.back_right.leg.moveOffset(backRightLegSwing)
			self.back_left.body.moveOffset(backLeftBodySwing)

		time.sleep(time_delay)

		leg_step_BRbody_delta = 30
		leg_step_BRmiddle_delta = 30
		leg_step_BRleg_delta = -28
		time.sleep(time_delay)

		self.back_right.standardPivotStepWithMidMovement(leg_step_BRbody_delta, leg_step_BRmiddle_delta, leg_step_BRleg_delta,velocity,time_delay)

		leg_extend_body_delta = 35
		leg_extend_middle_delta =-5
		leg_extend_leg_delta = 28

		self.front_right.legExtend( leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
		time.sleep(time_delay)

		RlungeFLbody= 40
		RlungeBRbody= -20
		RlungeFRmiddle = 30
		RlungeFRleg = -28
		RlungeBLmiddle = -10
		RlungeBLleg = 28

		self.lunge(0,RlungeFRmiddle,RlungeFRleg,RlungeFLbody,0,0, 0,RlungeBLmiddle,RlungeBLleg,RlungeBRbody,0,0)

		leg_step_BLbody_delta = -30
		leg_step_BLmiddle_delta = 30
		leg_step_BLleg_delta = -28
		time.sleep(time_delay)

		self.back_left.standardPivotStepWithMidMovement(leg_step_BLbody_delta, leg_step_BLmiddle_delta, leg_step_BLleg_delta,velocity,time_delay)

		self.front_left.legExtend( -leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
		time.sleep(time_delay)

		LlungeFRbody= -40
		LlungeBLbody= 20
		LlungeFLmiddle = 30
		LlungeFLleg = -28
		LlungeBRmiddle = -10
		LlungeBRleg = 28

		self.lunge(LlungeFRbody, 0,0,0,LlungeFLmiddle,LlungeFLleg, LlungeBLbody,0,0 ,0,LlungeBRmiddle, LlungeBRleg)
		self.reset()
		time.sleep(10)
		self.forward()


	# method to develop walking motion
	def walkInit(self):

		velocity = .01
		time_delay = .025

		if self.forwardInc < 8:


			velocity = .01
			time_delay = .025

			std_piv_step_body_delta = -20
			std_piv_step_middle_delta = 50
			std_piv_step_leg_delta = 5

			if self.forwardInc == 1:
				self.front_left.standardPivotStep(std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay*.01)
				time.sleep(time_delay)
				#self.updateAgenda()

			elif self.forwardInc == 2:
				self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
				time.sleep(time_delay)
				#self.updateAgenda()

			else:
				print "self.forwardInc == else"


			'''
			if self.forwardInc == 1:
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "front left pivot step"


			elif self.forwardInc == 2:
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "back right pivot step"
				self.back_right.standardPivotStep(-std_piv_step_body_delta, std_piv_step_middle_delta, std_piv_step_leg_delta,velocity,time_delay)
				time.sleep(time_delay)
				self.updateAgenda()

			leg_extend_body_delta 	= 35
			leg_extend_middle_delta = -5
			leg_extend_leg_delta 	= 28


			elif self.forwardInc == 3:
				print " walkInit with self.forwardInc:	",self.forwardInc
				print "front right leg extend"
				#self.front_right.legExtend( leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
				time.sleep(time_delay)
				self.updateAgenda()


			splitNum = 10
			leg_condense_FLbody_delta = 40/splitNum
			leg_condense_BRbody_delta = -20/splitNum
			leg_condense_FRmiddle_delta = 20/splitNum
			leg_condense_FRleg_delta = -28/splitNum
			leg_condense_BLbody_delta = 20/splitNum
			leg_condense_BLmiddle_delta = -10/splitNum
			leg_condense_BLleg_delta = 28/splitNum


			elif (self.forwardInc == 4):
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "condense forward right"
				for x in range(0, splitNum):
					self.front_left.body.moveOffset(leg_condense_FLbody_delta)
					self.back_right.body.moveOffset(leg_condense_BRbody_delta)
					self.front_right.middle.moveOffset(leg_condense_FRmiddle_delta)
					self.front_right.leg.moveOffset(leg_condense_FRleg_delta)
					self.back_left.body.moveOffset(leg_condense_BLbody_delta)
					self.back_left.middle.moveOffset(leg_condense_BLmiddle_delta)
					self.back_left.leg.moveOffset(leg_condense_BLleg_delta)
				self.updateAgenda()


			leg_step_BLbody_delta = -30
			leg_step_BLmiddle_delta = 30
			leg_step_BLleg_delta = -28
			time.sleep(time_delay)


			elif self.forwardInc == 5:
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "back left standard pivot step with mid offset"
				self.back_left.standardPivotStepWithMidMovement(leg_step_BLbody_delta, leg_step_BLmiddle_delta, leg_step_BLleg_delta,velocity,time_delay)
				self.updateAgenda()

			leg_step_FRbody_delta = -40
			leg_step_FRmiddle_delta = 5
			leg_step_FRleg_delta = 28

			elif self.forwardInc == 6:
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "front left standard pivot step with mid movement"
				self.front_left.standardPivotStepWithMidMovement(leg_step_FRbody_delta, leg_step_FRmiddle_delta, leg_step_FRleg_delta, velocity,time_delay)
				time.sleep(time_delay)
				self.updateAgenda()

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


			elif self.forwardInc == 7:
				print " walkInit with self.forwardInc:	",self.forwardInc
				#print "forward condence"
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
				self.updateAgenda()

			else:
				print "This message should not appear"
			'''

		else:
			self.walkCont(time_delay,velocity,1)



	def walkCont(self,time_delay,velocity,timesThrough):

		# THIS WILL NOT WORK. KNOWN BUG
		if(timesThrough ==2):
			print "times through == 2, reseting to avoid offset buildup"
			self.reset()
			self.forwardInc == self.forwardIncMin
			self.walkInit()

		leg_step_BRbody_delta = 30
		leg_step_BRmiddle_delta = 30
		leg_step_BRleg_delta = -28
		time.sleep(time_delay)
		'''
		if self.forwardInc  == 8:
			print " walkCont with self.forwardInc:	",self.forwardInc
			#print "back right standard pivot step with mid offset"
			self.back_right.standardPivotStepWithMidMovement(leg_step_BRbody_delta, leg_step_BRmiddle_delta, leg_step_BRleg_delta,velocity,time_delay)
			self.updateAgenda()

		elif self.forwardInc == 9:

			leg_extend_body_delta = 35
			leg_extend_middle_delta =-5
			leg_extend_leg_delta = 28

			print " walkCont with self.forwardInc:	",self.forwardInc
			#print "front right leg extend"
			self.front_right.legExtend( leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
			time.sleep(time_delay)
			self.updateAgenda()

		RlungeFLbody= 40
		RlungeBRbody= -20
		RlungeFRmiddle = 30
		RlungeFRleg = -28
		RlungeBLmiddle = -10
		RlungeBLleg = 28

		elif self.forwardInc == 10:
			self.lunge(0,RlungeFRmiddle,RlungeFRleg,RlungeFLbody,0,0, 0,RlungeBLmiddle,RlungeBLleg,RlungeBRbody,0,0)
			self.updateAgenda()

		leg_step_BLbody_delta = -30
		leg_step_BLmiddle_delta = 30
		leg_step_BLleg_delta = -28
		time.sleep(time_delay)

		elif self.forwardInc == 11:
			print " walkCont with self.forwardInc:	",self.forwardInc
			#print "back left standard pivot step with mid offset"
			self.back_left.standardPivotStepWithMidMovement(leg_step_BLbody_delta, leg_step_BLmiddle_delta, leg_step_BLleg_delta,velocity,time_delay)
			self.updateAgenda()

		elif self.forwardInc == 12:
			print " walkCont with self.forwardInc:	",self.forwardInc
			#print "front left leg extend"
			self.front_left.legExtend( -leg_extend_body_delta, leg_extend_middle_delta, leg_extend_leg_delta, velocity, time_delay)
			time.sleep(time_delay)
			self.updateAgenda()

		LlungeFRbody= -40
		LlungeBLbody= 20
		LlungeFLmiddle = 30
		LlungeFLleg = -28
		LlungeBRmiddle = -10
		LlungeBRleg = 28

		elif self.forwardInc == 13:
			print " walkCont with self.forwardInc:	",self.forwardInc
			self.lunge(LlungeFRbody, 0,0,0,LlungeFLmiddle,LlungeFLleg, LlungeBLbody,0,0 ,0,LlungeBRmiddle, LlungeBRleg)
			self.forwardInc == 7
			self.updateAgenda()

		else:
			print "this print statement should never be visible. error."
		'''
