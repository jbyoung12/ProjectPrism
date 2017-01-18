#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory, send_file
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json,datetime,logging
from bColors import bcolors
from Controls import Robot

async_mode = None
app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

thread = None
quadbotThread = None

image_delay_time = 1

data_file = "Controls/data.json" # path to data file
leg_debug = False
motor_debug = False
quadbot = Robot(data_file, leg_debug, motor_debug,socketio)


@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

def get_image():
     # read is the easiest way to get a full image out of a VideoCapture object.
     retval, im = camera.read()
     return im
     
quadbot_loop_time = 10


while True:
	for i in range(15):
		quadbot.turn(1)
		time.sleep(0)
	
	socketio.sleep(quadbot_loop_time/10)
		
	for i in range(16):
		quadbot.turn(-1)
		time.sleep(0)
	
	socketio.sleep(quadbot_loop_time)
	

def quadbot_thread():
	while True:
		socketio.sleep(quadbot_loop_time)
		
		for i in range(5):
			quadbot.turn(1)
		
		quadbot.displayWalkInit()
		socketio.sleep(quadbot_loop_time/10)
		
		
		for i in range(5):
			quadbot.turn(-1)

def background_thread():
    global image_delay_time
    while True:
        socketio.sleep(image_delay_time)
        
        with open("image.png", "rb") as f:
            data = f.read()
            socketio.emit('image', { 'image': True, 'buffer': data.encode("base64") });

@socketio.on('valueUpdate')
def valueUpdateHandler(message):
    print bcolors.OKGREEN + bcolors.BOLD +  str(datetime.datetime.now().time())+ ": valueUpdate fired" +bcolors.ENDC#". Message: ",bcolors.ENDC,bcolors.OKGREEN, message,  bcolors.ENDC
    #quadbot.inputData(message)
    data = {}
    data['Recieved'] = True
    return json.dumps(data)

@socketio.on('connect')
def test_connect():
    print bcolors.OKBLUE + "Client connected" + bcolors.ENDC
    global thread,quadbotThread
    if thread is None:
        print "init"
        #thread = socketio.start_background_task(target=background_thread)
	if quadbotThread is None:
		quadbotThread = socketio.start_background_task(target=quadbot_thread)

@socketio.on('disconnect')
def test_disconnect():
    print bcolors.WARNING + 'Client disconnected' + bcolors.ENDC

if __name__ == '__main__':
    print "sadness"
    #socketio.run(app, debug=True)
