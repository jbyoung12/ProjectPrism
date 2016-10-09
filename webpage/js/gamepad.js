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

window.addEventListener("gamepaddisconnected", function(e) {
  gamepadInfo.innerHTML = "Waiting for gamepad.";

  cancelRequestAnimationFrame(start);
});

/* Chrome - Instead of constantly storing the gamepad's latest state
//in a variable it only stores a snapshot, so to do the same thing in 
//Chrome you have to keep polling it and then only use the Gamepad object in code when it is available


var interval;

if (!('ongamepadconnected' in window)) {
  // No gamepad events available, poll instead.
  interval = setInterval(pollGamepads, 500);
}

function pollGamepads() {
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
*/


function buttonPressed(b) {
  if (typeof(b) == "object") {
    return b.pressed;
  }
  return b == 1.0;
}
//if chrome, the button values are stored as an array of double values,
function gameLoop() {
  var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
  if (!gamepads) {
    return;
  }

  var gp = gamepads[0];
  if (buttonPressed(gp.buttons[0])) {
    b--;
  } else if (buttonPressed(gp.buttons[2])) {
    b++;
  }
  if (buttonPressed(gp.buttons[1])) {
    a++;
  } else if (buttonPressed(gp.buttons[3])) {
    a--;
  }

  ball.style.left = a * 2 + "px";
  ball.style.top = b * 2 + "px";

  start = requestAnimationFrame(gameLoop);
}








