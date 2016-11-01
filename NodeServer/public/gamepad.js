//https://developer.mozilla.org/en-US/docs/Web/API/Gamepad_API/Using_the_Gamepad_API

//Gamepad object properties
//id: string - containing the USB vendor and product id of the controller, and the name of the controller as provided by the driver.
//index: integer - that is unique for each gamepad currently connected
//mapping: string - whether browser has remapped controls https://w3c.github.io/gamepad/#remapping
//connected: boolean - whether gamepad still connected
//buttons: array - Gamepadbutton objects
		//Gamepadbutton object properties
			//pressed - boolean
			//value - floating point - 0.0 (not pressed) to 1.0 (fully pressed)
//axes: array - floating point in range of -1.0 to 1.0
//timestamp: DOMHighResTimeStamp - last time data for gamepad was updated

/* FIREFOX
window.addEventListener("gamepaddisconnected", function(e) {
  gamepadInfo.innerHTML = "Waiting for gamepad.";
  cancelRequestAnimationFrame(start);
});

var gamepadInfo = document.getElementById("gamepad-info");
var ball = document.getElementById("ball");
var start;
var a = 0;
var b = 0;

window.addEventListener("gamepadconnected", function(e) {
  var gp = navigator.getGamepads()[e.gamepad.index];
  gamepadInfo.innerHTML = "Gamepad connected at index " + gp.index + ": " + gp.id + ". It has " + gp.buttons.length + " buttons and " + gp.axes.length + " axes.";
  gameLoop();
});

*/


//CHROME
var interval;

if (!('ongamepadconnected' in window)) {
  // No gamepad events available, poll instead.
  interval = setInterval(pollGamepads, 500);
}

function pollGamepads() {
  gamepadInfo = document.getElementById("gamepadInfo");
  var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
  for (var i = 0; i < gamepads.length; i++) {
    var gp = gamepads[i];
    if (gp) {
      gamepadInfo.innerHTML = "Gamepad connected at index " + gp.index + ": " + gp.id +
        ". It has " + gp.buttons.length + " buttons and " + gp.axes.length + " axes.";
      gameLoop();
      clearInterval(interval);
    }
  }
}

function buttonPressed(b) {
  if (typeof(b) == "object") {
    return b.pressed;
  }
  return b == 1.0;
}
var LEDState = 0;
var RotateCamera = 0;

function gameLoop() {
  var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
  if (!gamepads) {
    return;
  }

  var gp = gamepads[0];
  if (buttonPressed(gp.buttons[3])) {
    console.log("led toggled")
    ChangeLEDState();
    updateVal();
  }

  var RotateCamera = 180/Math.pi*Math.atan(gp.axes[1]/gp.axes[0]);
  if (RotateCamera < 0){
    RotateCamera = 180 + RotateCamera;
  }
  var CameraSpeed = Math.sqrt(Math.pow(gp.axes[0],2) + Math.pow(gp.axes[1],2));

  var MoveSide = gp.axes[2];
  var MoveFor = gp.axes[3];

  start = requestAnimationFrame(gameLoop);
}

var socket = io();
function ChangeLEDState(){
  if (LEDState == 0){
    LEDState = 1
  }
  else{
    LEDState = 0;
  }
  return LEDState
}

function updateVal(){
  socket.emit('valueUpdate',{
    ledState : LEDState,
    rotateCamera : RotateCamera,
    cameraSpeed : CameraSpeed,
    moveSide : MoveSide,
    moveFor : MoveFor
  }
);
return false;
}

socket.on('failure',function(msg){
  alert(msg);
});
