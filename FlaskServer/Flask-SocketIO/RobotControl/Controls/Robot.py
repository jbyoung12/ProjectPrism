import time
import json

class Robot():

  pwmFrequency = 50
  dataFileName = "data.json"
  with open(dataFileName) as data_file:
    data = json.load(data_file)
    for i in range(len(data["servos"])):
        print data["servos"][i]

  def __init__(self, arg):
    self.arg = arg
