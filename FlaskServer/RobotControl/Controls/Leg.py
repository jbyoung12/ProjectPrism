from Motor import Motor
from RobotUtil import RobotUtils
import time,math,json

class Leg(object):

	def __init__(self, leg_debug, motor_debug, pwm,name, body_pin,	body_min,	body_max,	body_center, mid_horiz_value, 	middle_pin,	middle_min,	middle_max,	middle_offset_to_center, leg_horiz_value, 	leg_pin,	leg_min,	leg_max,	leg_offset_to_center):
		
		if leg_debug:
			print "this should print status of leg, but is not implemented yet."
			
		self.pwm = pwm
	
		self.name = name
		
		self.body_name = "body"+name
		self.middle_name = "middle"+name
		self.leg_name = "leg"+name
		
		self.body 	= Motor( motor_debug,	0,					body_pin,	body_min,	body_max,	body_center,				self.body_name,		pwm)
		self.middle = Motor( motor_debug,	mid_horiz_value, 	middle_pin,	middle_min,	middle_max,	middle_offset_to_center,	self.middle_name,	pwm)
		self.leg 	= Motor( motor_debug,	leg_horiz_value, 	leg_pin,	leg_min,	leg_max,	leg_offset_to_center,		self.leg_name,		pwm)

	# velocity		- speed of movement (second delay between incrememnts in position)
	# Standard step in which the middle and leg lift up, the body rotates, and the middle + leg return to starting positions
	def standardPivotStep(self,body_delta,middle_delta,leg_delta,velocity,time_delay):
		
		middleEndVal = self.middle.value  
		legEndVal = self.leg.value
		
		print "in standardPivorStep"
		print ""
		
		print "moving middle"
		self.middle.moveOffSetInT(middle_delta, velocity)
		print "done moving middle"
		print ""
		time.sleep(time_delay)
		
		print "moving leg"
		self.leg.moveOffSetInT(leg_delta,velocity)
		print "done moving leg"
		print ""
		time.sleep(time_delay)
		
		print "moving body"
		self.body.moveOffSetInT(body_delta,velocity)
		print "done moving body"
		print ""
		time.sleep(time_delay)
		
		print "moving to: ",middleEndVal
		self.middle.moveTo(middleEndVal)
		print "done moving to",middleEndVal
		print ""
		time.sleep(time_delay)
		
		print "moving leg"
		self.leg.moveTo(legEndVal)
		print "done moving leg"
		print ""
		
		
	# velocity		- speed of movement (second delay between incrememnts in position)
	# Specific motion in which the robot lifts the middle and leg, pivots the body and drops the leg in front of the robot
	def legExtend(self, body_delta, middle_delta, leg_delta, velocity, time_delay):
		middle_raise= 30
		
		self.middle.moveOffset(middle_raise)
		print "in Leg Extend"
		print ""
		
		print "moving leg"
		self.leg.moveOffSetInT(leg_delta,velocity)
		print "done moving leg"
		print ""
		time.sleep(time_delay)
		
		print "moving body"
		self.body.moveOffSetInT(body_delta,velocity)
		print "done moving body"
		print ""
		time.sleep(time_delay)
		
		print "moving leg"
		self.middle.moveOffSetInT(middle_delta - middle_raise, velocity)
		print "done moving leg"
		print ""
		time.sleep(time_delay)

	
	def setMidAndLegHoriz(self):
		self.middle.moveToHoriz()
		self.leg.moveToHoriz()
	
	
	def reset(self):
		self.body.reset()
		self.middle.reset()
		self.leg.reset()
	
	# setLegToXY moves the tip of the leg to an xy point
	# x	-  horozontal distance to middle servo nut, must be POSITVE
	# y	-  vertical distance from the ground to the middle servo   **NEED TO CHECK IF CAN BE NEG**	
	def setLegXY(self, x, y):
		
		print "this method has not been completed"
		
	def standardPivotStepWithMidMovement(self,body_delta,middle_delta,leg_delta,velocity,time_delay):
		
		middlePickUp = 40
		
		print "in standardPivorStep"
		print ""
		
		print "moving middle"
		self.middle.moveOffSetInT(middlePickUp, velocity)
		print "done moving middle"
		print ""
		time.sleep(time_delay)
		
		
		
		print "moving body"
		self.body.moveOffSetInT(body_delta,velocity)
		print "done moving body"
		print ""
		time.sleep(time_delay)
		
		print "moving leg"
		self.leg.moveOffSetInT(leg_delta,velocity)
		print "done moving leg"
		print ""
		time.sleep(time_delay)
		
		self.middle.moveOffset(middle_delta - middlePickUp)
		print ""
		
	

