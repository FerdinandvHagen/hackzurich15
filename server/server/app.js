var express = require('express');
var app = express();

var net = require('net');

var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/hackzurich15');

var prefix = '';

var socketServer = net.createServer(function (socket) {
	socket.on('end', function () {
		console.log('client disconnected');
	});
	
	console.log("Client connected :)  " + socket.remoteAddress);
	socket.on('data', function (data) {
		console.log(String(data));
	});
});

function decode(chunk) {
	//First get the client ID
	character = chunk.split('');

	for (i = 0; i < 31; ++i) {
		console.log(i + ' ' + character[i]);
	}
}


socketServer.listen(3003, function () {
	console.log('Socket Server is now bound');
});

app.get('/', function (req, res) {
	res.send('Hello World!');
});

var server = app.listen(3000, function () {
	var host = server.address().address;
	var port = server.address().port;

	console.log('Example app listening at http://%s:%s', host, port);
});