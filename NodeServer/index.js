var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var PythonShell = require('python-shell');

app.use(express.static(__dirname+'/public'));

app.get('/', function(req, res){
	console.log(__dirname);
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  console.log('Client connected');
});


io.on('connection', function(socket){

	socket.on('valueUpdate' , function(msg){
		console.log('ValueUpdate! ',msg);
		console.log(msg.horizantalVal);
		console.log(msg.verticalVal);
		console.log(msg.ledState);
		console.log('\n');
		//PythonShell.run('main.py',function(err){
		//		socket.broadcast.emit('failure: ',err);
		//})
			
	});

});




http.listen(3000, function(){
  console.log('Listening on port 3000');
});
