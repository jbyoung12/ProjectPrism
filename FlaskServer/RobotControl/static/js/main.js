var data = {
  "buttons": [{
    "name": "stop",
    "elementId": "stopButton",
    "value": 0,
    "type": "button",
    "element_class": "col-md-4",
    "labelSize": 4
  }, {
    "name": "autonomous",
    "elementId": "autonomousButton",
    "value": 0,
    "type": "button",
    "element_class": "col-md-4",
    "labelSize": 4
  }],

  // display types: progress_bar,
  "status": [{
    "name": "battery",
    "labelSize": 5,
    "displayType": "progress_bar",
    "class": "progress-bar progress-bar-success",
    "id": "batteryValue",
    "data": {
      "current": 90,
      "min": 0,
      "max": 180
    }
  }],

  "controls": {
    "sliders": [
      xMovementSlider = {
        "id": "xMovement",
        "data": {
          "current": 50,
          "min": 0,
          "max": 100
        }
      },
      yMovementSlider = {
        "id": "yMovement",
        "data": {
          "current": 50,
          "min": 0,
          "max": 100
        }
      },

      horizontalVideoSlider = {
        "id": "horizontalVideo",
        "data": {
          "current": 50,
          "min": 0,
          "max": 100
        }
      },
      verticalVideoSlider = {
        "id": "verticalVideo",
        "data": {
          "current": 50,
          "min": 0,
          "max": 100
        }
      }
    ],
    "gamepad": {
      "gamepadConnected": false,
      "poll_time": 15,
      "gamepadVisible": false,
      "leastSignificantValidChange": 5,
      "inputs": [{
        "name": "xMovement",
        "elementId": "gamepad_x_movement",
        "axis": 0,
        "value": 50
      }, {
        "name": "yMovement",
        "elementId": "gamepad_y_movement",
        "value": 50,
        "axis": 1
      }, {
        "name": "horizontalVideo",
        "elementId": "gamepad_horizontal_video",
        "axis": 2,
        "value": 50
      }, {
        "name": "verticalVideo",
        "elementId": "gamepad_vertical_video",
        "axis": 3,
        "value": 50
      }]
    }
  }

}

function initializeGamepad() {
  if ("getGamepads" in navigator) {
    var gamepadInterval = setInterval(gamepadLoop, data.controls.gamepad.poll_time);
  } else {
    gamepadControls.innerHTML = "<h3>gamepad not available on your browser. Please switch to chrome </h3>";
    alert("Error! cannot gamepad");
  }
}

function gamepadLoop() {

  // Gamepad is connectd
  if (gamepadConnected()) {

    var rawGamepads = (navigator.getGamepads && navigator.getGamepads()) || (navigator.webkitGetGamepads && navigator.webkitGetGamepads()) || navigator.webkitGamepads;
    var gamepad = rawGamepads[0];

    // clear off the sliders if they havent been already
    if (!data.controls.gamepad.gamepadVisible) {
      hideElement("sliderControls");
      showElement("gamepadControls");
      data.controls.gamepad.gamepadVisible = true;
    }

    // The smallest percent difference before two values will considered to be changed
    var leastSignificantValidChange = data.controls.gamepad.leastSignificantValidChange;

    for (var i = 0; i < data.controls.gamepad.inputs.length; i++) {

      var oldValue = data.controls.gamepad.inputs[i].value
      var currentAxisNum = data.controls.gamepad.inputs[i].axis;

      newValue = map_range(gamepad.axes[currentAxisNum], -1, 1, 0, 100);

      if (valuesChanged(oldValue, newValue, leastSignificantValidChange)) {
        document.getElementById(data.controls.gamepad.inputs[i].elementId).setAttribute("style", "width:" + newValue + "%");
        data.controls.gamepad.inputs[i].value = newValue;
        send_data();
      }
    }

  } else {

    // check to see if sliders have been recreated
    if (data.controls.gamepad.gamepadVisible) {
      showElement("sliderControls");
      hideElement("gamepadControls");
      data.controls.gamepad.gamepadVisible = false;
    }

  }

}

function valuesChanged(oldValue, newValue, leastSignificantValidChange) {
  var error = Math.abs(((oldValue - newValue) / oldValue) * 100);
  return (error >= leastSignificantValidChange);
}


// this should take 5 parameters: input, minIn, maxIn, minOut, maxOut
function map_range(input, minIn, maxIn, minOut, maxOut) {
  return minOut + (maxOut - minOut) * (input - minIn) / (maxIn - minIn);
}


function gamepadConnected() {
  var rawGamepads = (navigator.getGamepads && navigator.getGamepads()) || (navigator.webkitGetGamepads && navigator.webkitGetGamepads()) || navigator.webkitGamepads;
  if (rawGamepads[0]) {
    data.controls.gamepad.gamepadConnected = true;
    return true;
  }
  data.controls.gamepad.gamepadConnected = false;
  return false;
}

var socket = io();
socket.on('failure', function(msg) {
  alert(msg);
});


$(document).ready(function() {
  hideElement("gamepadControls");
  initializeButtons();
  initializeSliders();
  initializeStatus();
  initializeGamepad();
});



function reportOnGamepad() {
  var gp = navigator.getGamepads()[0];
  var html = "";
  html += "id: " + gp.id + "<br/>";
  for (var i = 0; i < gp.buttons.length; i++) {
    html += "Button " + (i + 1) + ": ";
    if (gp.buttons[i].pressed) html += " pressed";
    html += "<br/>";
  }
  for (var i = 0; i < gp.axes.length; i += 2) {
    html += "Stick " + (Math.ceil(i / 2) + 1) + ": " + gp.axes[i] + "," + gp.axes[i + 1] + "<br/>";
  }
  $("#gamepadDisplay").html(html);
}


function initializeSliders() {
  for (var i = 0; i < data.controls.sliders.length; i++) {
    slider = document.getElementById(data.controls.sliders[i].id);
    noUiSlider.create(slider, {
      start: data.controls.sliders[i].data.current,
      range: {
        'min': [data.controls.sliders[i].data.min],
        'max': [data.controls.sliders[i].data.max]
      }
    });
    slider.noUiSlider.on('change', function() {
      send_data();
    });
  }
}


function send_data() {

  sendData = {};

  if (data.controls.gamepad.gamepadConnected) {
    for (var i = 0; i < data.controls.gamepad.inputs.length; i++) {
      sendData[data.controls.gamepad.inputs[i].name] = data.controls.gamepad.inputs[i].value;
    }
  } else {
    for (var i = 0; i < data.controls.sliders.length; i++) {
      sendData[data.controls.sliders[i].id] = document.getElementById(data.controls.sliders[i].id).noUiSlider.get();
    }
  }

  for (var i = 0; i < data.buttons.length; i++) {
    sendData[data.buttons[i].name] = document.getElementById(data.buttons[i].elementId).checked;
  }
  socket.emit('valueUpdate', {
    data: sendData
  }, function(callbackData) {
    console.log("Server Response | callbackData:" + callbackData);
  });
  return false;
}

function hideElement(id) {
  $("#" + id).hide();
  //document.getElementById(id).style.display = "none";
}

function showElement(id) {
  $("#" + id).show();
  //document.getElementById(id).style.display = "visible";
}

function initializeStatus() {
  var parentDiv = document.getElementById("statusParent");
  for (var i = 0; i < data.status.length; i++) {
    var tr = document.createElement("tr");
    var td = document.createElement("td");
    var divRow = document.createElement("div");
    divRow.setAttribute("class", "row");
    var titleDiv = document.createElement("div");
    titleDiv.setAttribute("class", "col-md-4");
    var h = document.createElement("h" + data.status[i].labelSize);
    h.innerHTML = data.status[i].name;
    var progressOutsideDiv = document.createElement("div");
    progressOutsideDiv.setAttribute("class", "col-md-8");
    var progressInsideDiv = document.createElement("div");
    progressInsideDiv.setAttribute("class", "progress");
    var progressDiv = document.createElement("div");
    progressDiv.setAttribute("id", data.status[i].id);
    progressDiv.setAttribute("class", data.status[i].class);
    progressDiv.setAttribute("style", "width: " + data.status[i].data.current + "%");

    tr.appendChild(td);
    td.appendChild(divRow);
    divRow.appendChild(titleDiv);
    titleDiv.appendChild(h);
    divRow.appendChild(progressOutsideDiv);
    progressOutsideDiv.appendChild(progressInsideDiv);
    progressInsideDiv.appendChild(progressDiv);
    parentDiv.appendChild(tr);
  }
}

function initializeButtons() {

  var parentDiv = document.getElementById("permenant_controls");
  for (var i = 0; i < data.buttons.length; i++) {
    var button = document.createElement("div");
    button.innerHTML = "<h" + data.buttons[i].labelSize + "> " + data.buttons[i].name + "</" + data.buttons[i].labelSize + ">";
    button.setAttribute("class", data.buttons[i].element_class);
    var label = document.createElement("label");
    label.className = "switch";
    var input = document.createElement("input");
    input.setAttribute("type", "checkbox");
    input.setAttribute("id", data.buttons[i].elementId);
    var div = document.createElement("div");
    div.setAttribute("class", "slider round");
    parentDiv.appendChild(button);
    button.appendChild(label);
    label.appendChild(input);
    label.appendChild(div);

    input.addEventListener("click", function() {
      send_data();
    })
  }

}
