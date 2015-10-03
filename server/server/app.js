var express = require('express');
var app = express();

var net = require('net');

var socketServer = net.createServer(function (socket) {
	socket.on('end', function () {
		console.log('client disconnected');
	});

	socket.on('data', function (data) {
		chunks = String(data).split("\0");
		console.log('Result: ' + String(data));
		console.log('Chunk size: ' + chunks.length + ' hh ' + chunks[0]);

		for (i = 0; i < chunks.length; ++i) {
			if (chunks[i].length == 31) {
				console.log('Found: ' + chunks[i]);
			}
		}
	});
});

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