var gamepadSupport = {

  pollGamepads: function() {

    // Get the array of gamepads ï¿½ the first method (function call)
    // is the most modern one, the second is there for compatibility with
    // slightly older versions of Chrome, but it shouldnï¿½t be necessary
    // for long.
    var rawGamepads =
      (navigator.getGamepads && navigator.getGamepads()) ||
      (navigator.webkitGetGamepads && navigator.webkitGetGamepads()) ||
      navigator.webkitGamepads;

    if (rawGamepads) {
      // We donï¿½t want to use rawGamepads coming straight from the browser,
      // since it can have ï¿½holesï¿½ (e.g. if you plug two gamepads, and then
      // unplug the first one, the remaining one will be at index [1]).
      gamepadSupport.gamepads = [];
      gamepadSupport.gamepadsRaw = [];

      //var rawFixedGamepads = {};
      //for(var i = 0, ii = 0; i <= rawGamepads.length; i++){
      //    if (typeof rawGamepads[i] == "object" && rawGamepads[i].id.indexOf("(Vendor: b58e Product: 9e84)") !== -1) continue;
      //    rawFixedGamepads[ii] = rawGamepads[i];
      //    rawFixedGamepads.length = i;
      //    ii++;
      //}
      //console.log(rawFixedGamepads);
      //rawGamepads = rawFixedGamepads;


      // We only refresh the display when we detect some gamepads are new
      // or removed; we do it by comparing raw gamepad table entries to
      // ï¿½undefined.ï¿½
      var gamepadsChanged = false;

      for (var i = 0; i < rawGamepads.length; i++) {
        if (typeof rawGamepads[i] != gamepadSupport.prevRawGamepadTypes[i]) {
          gamepadsChanged = true;
          gamepadSupport.prevRawGamepadTypes[i] = typeof rawGamepads[i];
        }
        //console.log(rawGamepads[i].id);
        //if (rawGamepads[i] == undefined || rawGamepads[i].id.indexOf("(Vendor: b58e Product: 9e84)") !== -1) continue;
        if (rawGamepads[i] && controllerRebinds != "" && typeof controllerRebinds.mapping != 'undefined' && controllerRebinds.mapping.length > 0) {
          var remapObj = $.extend(true, {}, rawGamepads[i]);
          for (var b = 0; b < remapObj.buttons.length; b++) {
            remapObj.buttons[b] = $.extend({}, rawGamepads[i].buttons[b]);
          }
          for (var m = 0; m < controllerRebinds.mapping.length; m++) {
            var bindmap = controllerRebinds.mapping[m];
            var noMap = bindmap.disabled;
            var tType = bindmap.targetType;
            var tMap = bindmap.target;
            if (tType == "buttons" && typeof remapObj[tType][tMap] == "undefined") {
              remapObj[tType][tMap] = {};
            }

            function stickToButton(stickObj) {
              //contrary to the function's name, it turns buttons into buttons too
              if (stickObj.choiceType == "buttons") {
                return rawGamepads[i].buttons[stickObj.choice].value;
              } else {
                var axisVal = rawGamepads[i].axes[stickObj.choice];
                switch (stickObj.choiceOperand) {
                  case "+":
                    return (axisVal > 0) ? axisVal : 0;
                    break;
                  case "-":
                    return (axisVal < 0) ? Math.abs(axisVal) : 0;
                    break;
                  default:
                    return 0;
                }
              }
            }

            function makeAxis(positiveAxis, negativeAxis) {
              return positiveAxis - negativeAxis;
            }

            /***
             * This should function as a somewhat wrapper function to allow an easier way of producing
             * values on remapping. Should the bindmap have no data for axis configuration, the mapping is
             * then sent off to the stickToButton function which then produces the appropriate values.
             *
             * This tends to get slightly trickier when I have to actually account for when an axis is
             * broken as I first have to manipulate the raw data, then return proper values, either as a
             * button or an axis.
             * */
            function bindWrapper(stickObj) {
              if (tType == "dpad") {
                dpadPOV(stickObj);
                return;
              }
              if (typeof stickObj.axesConfig == "undefined") {
                setMapping(stickObj, {});
                return;
              }
              fixAxes(stickObj);
            }

            function choiceValue(mapObj) {
              var value;
              switch (mapObj.choiceType) {
                case "":
                  return choiceValue(mapObj.positive);
                  break;
                case "axes":
                  value = rawGamepads[i].axes[mapObj.choice];
                  break;
                case "buttons":
                  value = rawGamepads[i].buttons[mapObj.choice].value;
                  break;
                default:
                  value = 0;
              }
              return value;
            }

            function fixAxes(stickObj) {
              var startValue = +stickObj.axesConfig.lowValue;
              var endValue = +stickObj.axesConfig.highValue;
              var absStart = Math.abs(startValue);
              var absEnd = Math.abs(endValue);
              var maxAbs = Math.max(absStart, absEnd);
              var zeroOffset = 0 - startValue;
              var axisVal = choiceValue(stickObj);
              var newValue = (zeroOffset + axisVal) / (zeroOffset + endValue);
              switch (stickObj.axesConfig.type) {
                case "trigger":
                  newValue = (newValue > 1) ? 1 : ((newValue < 0) ? 0 : newValue);
                  break;
                case "stick":
                  newValue = (axisVal * (1 / maxAbs));
                  newValue = (newValue > 1) ? 1 : ((newValue < -1) ? -1 : newValue);
                  break;
                default:
                  setMapping(stickObj, {});
                  return;
              }
              setMapping(stickObj, newValue);
            }

            function dpadPOV(stickObj) {
              var up, down, left, right, upright, downright, downleft, upleft, value;
              setMapping({
                targetType: "buttons",
                target: 12
              }, 0);
              setMapping({
                targetType: "buttons",
                target: 13
              }, 0);
              setMapping({
                targetType: "buttons",
                target: 14
              }, 0);
              setMapping({
                targetType: "buttons",
                target: 15
              }, 0);
              if (stickObj.disabled) return;
              up = stickObj.positions.up;
              down = stickObj.positions.down;
              left = stickObj.positions.left;
              right = stickObj.positions.right;
              upright = stickObj.positions.upright;
              downright = stickObj.positions.downright;
              downleft = stickObj.positions.downleft;
              upleft = stickObj.positions.upleft;
              value = choiceValue(stickObj);
              if (value == up) {
                setMapping({
                  targetType: "buttons",
                  target: 12
                }, 1);
              } else if (value == down) {
                setMapping({
                  targetType: "buttons",
                  target: 13
                }, 1);
              } else if (value == left) {
                setMapping({
                  targetType: "buttons",
                  target: 14
                }, 1);
              } else if (value == right) {
                setMapping({
                  targetType: "buttons",
                  target: 15
                }, 1);
              } else if (value == upright) {
                setMapping({
                  targetType: "buttons",
                  target: 12
                }, 1);
                setMapping({
                  targetType: "buttons",
                  target: 15
                }, 1);
              } else if (value == downright) {
                setMapping({
                  targetType: "buttons",
                  target: 15
                }, 1);
                setMapping({
                  targetType: "buttons",
                  target: 13
                }, 1);
              } else if (value == downleft) {
                setMapping({
                  targetType: "buttons",
                  target: 13
                }, 1);
                setMapping({
                  targetType: "buttons",
                  target: 14
                }, 1);
              } else if (value == upleft) {
                setMapping({
                  targetType: "buttons",
                  target: 14
                }, 1);
                setMapping({
                  targetType: "buttons",
                  target: 12
                }, 1);
              }
            }

            /***
             * This is to easily set the remapped item properly, whether it's a stick or axis as per defined
             * by the bindmap.
             * */
            function setMapping(stickObj, setValue) {
              switch (typeof setValue) {
                case "number":
                case "Number":
                  if (stickObj.targetType == "axes") {
                    remapObj[stickObj.targetType][stickObj.target] = setValue;
                  } else {
                    remapObj[stickObj.targetType][stickObj.target].value = setValue;
                  }
                  break;
                case "object":
                case "Object":
                  if (stickObj.targetType == "axes") {
                    remapObj[stickObj.targetType][stickObj.target] = makeAxis(stickToButton(stickObj.positive), stickToButton(stickObj.negative));
                  } else {
                    remapObj[stickObj.targetType][stickObj.target].value = stickToButton(stickObj);
                  }
                  break;
                default:
              }
            }

            try {
              if (noMap && tType != "dpad") {
                setMapping(bindmap, 0);
              } else {
                bindWrapper(bindmap);
              }
            } catch (e) {
              //console.log("ERROR IN MAPPING: ", e);
            }
          }
          gamepadSupport.gamepads.push(remapObj);
          gamepadSupport.gamepadsRaw.push(rawGamepads[i]);
        } else if (rawGamepads[i]) {
          gamepadSupport.gamepads.push(rawGamepads[i]);
          gamepadSupport.gamepadsRaw.push(rawGamepads[i]);
        }
      }

      // Ask the tester to refresh the visual representations of gamepads
      // on the screen.
      if (gamepadsChanged) {
        tester.updateGamepads(gamepadSupport.gamepads);
      }
    }
  }
}
