var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var pythonScript = 'main.py';
var PythonShell = require('python-shell');

app.use(express.static(__dirname+'/public'));

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});


io.on('connection', function(socket){

  console.log("Client connected.")

	socket.on('valueUpdate' , function(msg){

		options ={
			args:[msg.ledState, msg.autonomousState, msg.horizontalVideoValue,msg.verticalVideoValue,msg.xMovement, msg.yMovement]
		}
		console.log('Args: ',options.args)


    var pyshell = new PythonShell(pythonScript,options)

    pyshell.send("test \n testline2");

    pyshell.on('message', function (message) {
      console.log(message);
    });

    pyshell.end(function (err) {
      if (err) throw err;
      console.log('finished');
    });


	});

});



http.listen(3000, function(){
  console.log('Listening on port 3000');
});
