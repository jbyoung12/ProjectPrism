
class RobotUtils(object):

	R1 				= 11.5			# length of curved leg portion of bot
	R2 				= 6.5			# length of junction between middle and leg servos
	MAX_MOTOR_VALUE = 100			# Maximum possible motor value
	MIN_MOTOR_VALUE = 0				# Minumum possible motor value
	SERVO_MIN		= 130			# Minumum tick count for duty cycle
	SERVO_MAX		= 570 			# Maximum tick count for duty cycle
	FREQUENCY		= 50 			# 50 Hz creates a 20 ms period, which servos operate with
	DATA_FILE	 	= "Controls/data.json" 	# path to data file
	LEG_DEBUG 		= False			# Debug Legs
	MOTOR_DEBUG 	= False			# Debug Motors
	LIVE_TESTING	= False			# Dictates whether program is executing on robot or on a development computer
	VIDEO_STEAMING	= False			# Determines whether the computer/Pi streams live video footage

	# takes any numer and returns 1 if positive, -1 if negative, and 0 if input is 0
	@staticmethod
	def PositiveOrNegative(n):
		if (n>0):
			return 1
		elif (n<0):
			return -1
		else:
			return 0

	@staticmethod
	def scale(value, leftMin, leftMax, rightMin, rightMax):
		return rightMin + ((float(value - leftMin) / float((leftMax - leftMin))) * (rightMax - rightMin))
