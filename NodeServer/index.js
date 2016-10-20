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

		options ={
			args:[msg.ledState,msg.verticalVal,msg.horizantalVal]
		}
		console.log('Options: ',options)

		PythonShell.run('main.py',options,function(err,results){
				socket.broadcast.emit('failure: ',err);
				console.log(results)
		})

	});

});




http.listen(3000, function(){
  console.log('Listening on port 3000');
});
