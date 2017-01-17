# ProjectPrism

### Description
Autonomous and tele-operated quad-bot, with live streaming video and thermal imaging footage

### Hardware
Robot made of 3d prints designed using <b>AutoCAD for Mac 2016</b>. prints hold 12 Standard Servo motors.<br>
Power supplied externally and regulated with <b>DROK® LM2596 Digital Control Voltage Regulator DC Buck Converter</b>.<br>
Servos controlled by Adafruit's <b>16-Channel PWM / Servo HAT</b>.<br>
Software running on <b>Raspberry Pi 2</b>

### Web Interface
<b>Flask server</b> with <b>socket.io</b> python plugin provide client <-> server communication over <b>websocket protocol</b>.<br>
Front end presented with <b>bootstrap</b>, <b>jQuery</b> and <b>noioslider</b>.<br>
User input gathered with <b>nouislider.js</b>, and <b>GamepadAPI</b> for ps3/xbox controller input

### Robot Controls
Flask server creates Robot object which acts as executive of the system. <br>
<b>Robot.py</b> takes user input and controls Legs  <br>
<b>Leg.py</b> takes commands from Robot class and commands motors <br>
<b>Motor.py</b>commands individual Servos <br>
<b>RobotUtil.py</b> holds universal variables and static methods <br>
<b>pwm.py | I2c.py</b> provide <b>i2c interface</b> <br>
