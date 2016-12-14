var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var pythonScript = 'main.py';
var PythonShell = require('python-shell');

app.use(express.static(__dirname + '/public'));

app.get('/', function(req, res) {
  res.sendFile(__dirname + '/index.html');
});


io.on('connection', function(socket) {

  console.log("Client connected.")

  socket.on('valueUpdate', function(msg, callback) {

    console.log("input: " + JSON.stringify(msg));
    options = {
      args: [msg.autonomousState, msg.horizontalVideoValue, msg.verticalVideoValue, msg.xMovement, msg.yMovement]
    }
    console.log('Args: ', options.args)

    var pyshell = new PythonShell(pythonScript, options)

    var pyshellErr = "";
    var pyshellMsg = "Success";

    pyshell.send("test \n testline2");

    pyshell.on('message', function(message) {
      console.log("python shell: " + message);
      pyshellMsg = message;
    });

    pyshell.end(function(err) {
      //if (err) throw err;
      if (err) {
        console.log("Error: " + err);
        pyshellErr = err;
      }
    });

    callback(pyshellErr, pyshellMsg);
  });
});



http.listen(3000, function() {
  console.log('Listening on port 3000');
});
