var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
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

		PythonShell.run('main.py',options,function(err,results){
				socket.broadcast.emit('failure: ',err);
        if (err) throw err;
				console.log("Arduino Response: "+results)
		})

	});

});



http.listen(3000, function(){
  console.log('Listening on port 3000');
});
