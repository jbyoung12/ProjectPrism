#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory, send_file
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json
from bColors import bcolors
#from Controls import Robot
import cv2
camera = cv2.VideoCapture(0)
ret3 = camera.set(3,50)
ret4 = camera.set(4,50)
print "ret3: ",ret3
print "ret4: ",ret4

async_mode = None
app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None
image_delay_time = .01

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
        camera_capture = get_image()
        cv2.imwrite("image.png", camera_capture)
        socketio.sleep(image_delay_time)

        with open("image.png", "rb") as f:
            data = f.read()
            socketio.emit('image', { 'image': True, 'buffer': data.encode("base64") });


@socketio.on('valueUpdate')
def valueUpdateHandler(message):
    print bcolors.OKGREEN + bcolors.BOLD + "valueUpdate fired. Message: ",bcolors.ENDC,bcolors.OKGREEN, message,  bcolors.ENDC
    data = {}
    data['Recieved'] = True
    return json.dumps(data)

@socketio.on('connect')
def test_connect():
    print bcolors.OKBLUE + "Client connected" + bcolors.ENDC
    global thread
    if thread is None:
        thread = socketio.start_background_task(target=background_thread)


@socketio.on('disconnect')
def test_disconnect():
    print bcolors.WARNING + 'Client disconnected' + bcolors.ENDC


def main():
    print "in main"
    data_file = "Controls/data.json" # path to data file
    leg_debug = False
    motor_debug = False
    quadbot = Robot(frequency,leg_debug, motor_debug)


if __name__ == '__main__':
    socketio.run(app, debug=True)
    #main()
