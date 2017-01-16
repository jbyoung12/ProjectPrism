#!/usr/bin/env python
from flask import Flask, render_template, session, request, send_from_directory
from flask_socketio import SocketIO, emit, join_room, leave_room, close_room, rooms, disconnect
import time,json
from bColors import bcolors
#from Controls import Robot

async_mode = None

app = Flask(__name__, static_url_path='/static')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None


@app.route('/', methods=['GET', 'POST'])
def index():
  return render_template('index.html', async_mode=socketio.async_mode)


@socketio.on('valueUpdate')
def valueUpdateHandler(message):
  print bcolors.OKGREEN + bcolors.BOLD + "valueUpdate fired. Message: ",bcolors.ENDC,bcolors.OKGREEN, message,  bcolors.ENDC
  data = {}
  data['Recieved'] = True
  return json.dumps(data)

@socketio.on('connect')
def test_connect():
  print bcolors.OKBLUE + "Client connected" + bcolors.ENDC


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
