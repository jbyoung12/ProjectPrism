from pwm import PWM
from I2C import Adafruit_I2C
from Motor import Motor
from RobotUtil import RobotUtils
import time,math,json

class Leg(object):

	def __init__(self,pwm,servo_min,servo_max,name, bodyPin, bodyCenter,bodyMin,bodyMax, middlePin, middleCenter, middleMin,middleMax,    legPin, legCenter, legMin, legMax):
		
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
		self.middle.moveOffSetInT(middle_delta, velocity)
		print "done moving leg"
		print ""
		time.sleep(time_delay)

	def reset(self):
		self.body.reset()
		self.middle.reset()
		self.leg.reset()
		
		

	# setLegToXY moves the tip of the leg to an xy point
	# x	-  horozontal distance to middle servo nut, must be POSITVE
	# y	-  vertical distance from the ground to the middle servo   **NEED TO CHECK IF CAN BE NEG**	
	def setLegXY(self, x, y):
		
		R1 = RobotUtils.R1														
		R2 = RobotUtils.R2
		pi = math.pi
		
		D = math.sqrt( (x**2) + (y**2) )											# Length from body servo to tip of leg
		
		B_rad_input = ((D**2)-(R1**2)-(R2**2)) / (-2*D*R2)
		
		if B_rad_input >= -1 and B_rad_input <= 1:
			B_rad = math.acos(B_rad_input )												# Angle between middle arm and leg in rad. Found with Law of Cosines
			B_deg = math.degrees(B_rad)													# ^ in degrees
			
			A_rad_input = (((R1**2)-(D**2)-(R2**2))/(-2*D*R2))

			if A_rad_input >= -1 and A_rad_input <= 1:

				print "A_rad_input: ",A_rad_input
				A_rad = math.acos(A_rad_input)												# Angle between horizon and middle arm in radians
				A_deg = math.degrees(A_rad)													# ^ in degrees

				midValue = RobotUtils.scale(A_deg,self.middle.min,self.middle.max,0,90)	# scaled value of middle
				#midValue = midValue+(((RobotUtils.MAX_MOTOR_VALUE - RobotUtils.MIN_MOTOR_VALUE)/180)*90)
				print "corrected midValue: ",midValue
			
				legValue = RobotUtils.scale(B_deg,self.leg.min,self.leg.max,0,180)			# scaled value of leg
				legValue = legValue 
				print "corrected legValue: ",legValue
				print ""
		 
				# TODO: Calibrate for each leg
				self.middle.moveTo(midValue)
				self.leg.moveTo(legValue)  
			
			else:
				print "A_rad_input invaled: ",A_rad_input
		
		else:
			print "B_rad_input INVALID: ",B_rad_input

