#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory, send_file
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json,datetime,logging
from bColors import bcolors
from Controls import Robot
from Controls import RobotUtils

async_mode = None
app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

thread = None
image_delay_time = 1

if not RobotUtils.LIVE_TESTING:
    import cv2
    camera_port = 0
    camera = cv2.VideoCapture(camera_port)
    camera.set(3,300)
    camera.set(4,300)
    image_delay_time = 1.0/30.0

# Create robot object.
quadbot = Robot(socketio)

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

def get_image():
     retval, im = camera.read()
     return im

def background_thread():
    if not RobotUtils.LIVE_TESTING:
        global camera
    global image_delay_time
    while True:
        socketio.sleep(image_delay_time)

        # If live streaming and not live testing (== running on mac)
        if RobotUtils.VIDEO_STEAMING and not RobotUtils.LIVE_TESTING:
            camera_capture = get_image()
            cv2.imwrite("image.png", camera_capture)
            with open("image.png", "rb") as f:
                data = f.read()
                socketio.emit('image', { 'image': True, 'buffer': data.encode("base64") });

@socketio.on('valueUpdate')
def valueUpdateHandler(message):
    print bcolors.OKGREEN + bcolors.BOLD +  str(datetime.datetime.now().time())+ ": valueUpdate fired" +bcolors.ENDC#". Message: ",bcolors.ENDC,bcolors.OKGREEN, message,  bcolors.ENDC
    quadbot.inputData(message)
    data = {}
    data['Recieved'] = True
    return json.dumps(data)

@socketio.on('connect')
def test_connect():
    print bcolors.OKBLUE + "Client connected" + bcolors.ENDC
    global thread,quadbotThread
    if thread is None:
        print "init"
        thread = socketio.start_background_task(target=background_thread)

@socketio.on('disconnect')
def test_disconnect():
    print bcolors.WARNING + 'Client disconnected' + bcolors.ENDC

if __name__ == '__main__':
    socketio.run(app, debug=True)
