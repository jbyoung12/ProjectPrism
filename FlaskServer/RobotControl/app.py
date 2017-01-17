#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory, send_file
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json,datetime
from bColors import bcolors
#from Controls import Robot

async_mode = None
app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None
image_delay_time = 1

data_file = "Controls/data.json" # path to data file
leg_debug = False
motor_debug = False
print "Creating Robot object"
#quadbot = Robot(frequency,leg_debug, motor_debug)
print "Robot created"
global quadbot

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

def get_image():
     # read is the easiest way to get a full image out of a VideoCapture object.
     retval, im = camera.read()
     return im

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
    #data = {}
    #data['Recieved'] = True
    #return json.dumps(data)

@socketio.on('connect')
def test_connect():
    print bcolors.OKBLUE + "Client connected" + bcolors.ENDC
    global thread
    if thread is None:
        thread = socketio.start_background_task(target=background_thread)

@socketio.on('disconnect')
def test_disconnect():
    print bcolors.WARNING + 'Client disconnected' + bcolors.ENDC

if __name__ == '__main__':
    socketio.run(app, debug=True)
