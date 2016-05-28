var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var PythonShell = require('python-shell');

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  console.log('Client connected');
});


io.on('connection', function(socket){

	socket.on('button click' , function(msg){
		console.log('Button Clicked');
		PythonShell.run('main.py',function(err){
				socket.broadcast.emit('failure: ',err);
		})	
	});

});




http.listen(3000, function(){
  console.log('listening on *:3000');
});
