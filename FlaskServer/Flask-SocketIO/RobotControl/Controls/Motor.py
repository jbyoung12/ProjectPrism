from pwm import PWM

class Motor(object):

    servoMin = 125
    servoMax = 575
    pwm = PWM()

    '''* minVal>=0, maxVal<=100 *'''
    def __init__(self, pinNumber, minVal=0, maxVal=100, currentVal=50,name="no name", pwmFreq=50):
        self.pinNumber = pinNumber
        self.min = minVal
        self.max = maxVal
        self.value = currentVal
        self.name = name
        self.pwmFreq = pwmFreq
        pwm.setPWMFreq(self.pwmFreq)

    def move(self,val):
        if val > self.min and val < self.max:
            self.value = val
            scaledValule = self.scale(self.value, self.minValue, self.maxValue, servoMin,servoMax)
            pwm.setPWM(self.pinNumber, 0, scaledValue)

    def scale(self,value, leftMin, leftMax, rightMin, rightMax):
        return rightMin + ((float(value - leftMin) / float((leftMax - leftMin))) * (rightMax - rightMin))
