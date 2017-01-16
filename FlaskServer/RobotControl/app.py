#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory, send_file
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json,subprocess,picamera
from bColors import bcolors
#from Controls import Robot

async_mode = None

app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None
image_delay_time = 1

camera = picamera.PiCamera()
n = 5

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', async_mode=socketio.async_mode)

def background_thread():
    global image_delay_time
    while True:
        os.system("raspistill -o image.png")
        time.sleep(image_delay_time)
        with open("image.png", "rb") as f:
            data = f.read()
            print "sending image"
            socketio.emit('image', { 'image': True, 'buffer': data.encode("base64") });
            n += 1

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
