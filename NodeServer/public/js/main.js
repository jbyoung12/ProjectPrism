// Beggining of JoyStick

/*
$(document).ready(function(){
    var joystickView = new JoystickView(150, function(callbackView){
        $("#joystickContent").append(callbackView.render().el);
        setTimeout(function(){
            callbackView.renderSprite();
        }, 0);
    });

    joystickView.bind("verticalMove", function(y){
        $("#yVal").html(y);
    });
    joystickView.bind("horizontalMove", function(x){
        $("#xVal").html(x);
    });
});*/
//End of JoyStick

$(document).ready(function(){

  var socket = io();
  var LEDState = 0;
  var autonomousState = 0;

  function ChangeLEDState(){
    if (LEDState == 0){
      LEDState = 1
    }
    else{
      LEDState = 0;
    }
    return LEDState
  }

  function ChangeAutonomousState(){
    if (autonomousState == 0){
      autonomousState = 1
    }
    else{
      autonomousState = 0;
    }
    return autonomousState
  }

  $("#led_toggle").click(function(){
    ChangeLEDState();
    updateVal();
  })

  $("#autonomous_toggle").click(function(){
    ChangeAutonomousState();
    updateVal();
  });



  var sliders = {
    sliderDivIds : ["horizontalVideo","verticalVideo","xMovement","yMovement" ],
    options : [ [start=90,min=0,max=180], [start=90,min=0,max=180],[start=90,min=0,max=180],[start=90,min=0,max=180] ]
  }

  var noUiSliders = []

  for ( var i = 0; i <sliders.sliderDivIds.length; i ++){

    tempSliderElement = document.getElementById(sliders.sliderDivIds[i]);

    noUiSlider.create(tempSliderElement,{
      start:[ sliders.options[0][0]  ] ,
      range:{
        'min':[sliders.options[0][1] ],
        'max':[sliders.options[0][2] ]
      }
    });

    tempSliderElement.noUiSlider.on('change',function(){
      updateVal();
    });

    noUiSliders.push(tempSliderElement)

  }

  function updateVal(){

    socket.emit('valueUpdate',{
        horizontalVideoValue : Math.floor(horizontalVideo.noUiSlider.get()),
        verticalVideoValue : Math.floor(verticalVideo.noUiSlider.get()),
        xMovement: Math.floor(noUiSliders[2].noUiSlider.get()),
        yMovement: Math.floor(noUiSliders[3].noUiSlider.get()),
        autonomousState: autonomousState,
        ledState : LEDState
      }
    );


    return false;
  }

  socket.on('failure',function(msg){
      alert(msg);
  });


});
