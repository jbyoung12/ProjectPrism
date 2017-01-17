# ProjectPrism

## Description
Autonomous and tele-operated quad-bot, with live streaming video and thermal imaging footage

## Hardware
The robot is made of 3d prints designed using AutoCAD for Mac 2016. The frame hold 12 Standard Servo motors which are powered through an external power supply which is regulated by the DROK® LM2596 Digital Control Voltage Regulator DC Buck Converter. The servos are driven by Adafruit's  16-Channel PWM / Servo HAT .<br>
Software runs off a Raspberry Pi 2

## Web Interface
 Flask server  with  socket.io  python plugin provide client <-> server communication over  websocket protocol .<br>
Front end presented with  bootstrap ,  jQuery  and  noioslider .<br>
User input gathered with  nouislider.js , and  GamepadAPI  for ps3/xbox controller input

## Robot Controls
Flask server creates Robot object which acts as executive of the system. <br>
Robot.py  takes user input and controls Legs  <br>
Leg.py  takes commands from Robot class and commands motors <br>
Motor.py commands individual Servos <br>
RobotUtil.py  holds universal variables and static methods <br>
pwm.py | I2c.py  provide  i2c interface  <br>
