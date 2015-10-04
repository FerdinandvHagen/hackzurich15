var express = require('express');
var app = express();

var net = require('net');

var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var serveStatic = require('serve-static')

var currentMacs = {};

var subscription = [];

var packets = {};

var nodeCount = 0;

//console.log("timestamp;rssi;mac;receiver");

app.use('/app', serveStatic('./client/app'));
app.use('/bower_components', express.static('./client/bower_components'));

app.get('/', function (req, res) {
	res.redirect('/app/index.html');
});

process.on('uncaughtException', function (err) {
    console.error(err.stack);
    console.log("Node NOT Exiting...");
});

io.on('connection', function (socket) {
	console.log('Browser connected');
	socket.on('disconnect', function () {
		console.log('Browser disconnected');
	});

	socket.on('getmac', function (msg) {
		socket.emit('currentMacs', currentMacs);
	});

	socket.on('subscribe', function (msg) {
		console.log('Subscribing to ' + msg);
		if (undefined == subscription[msg]) {
			subscription[msg] = [];
		}
		subscription[msg].push(socket);
	});
});

http.listen(3000, function () {
	console.log('listening on *:3000');
});

var matlab = undefined;

var matlabserver = net.createServer(function (socket) {
	socket.on('end', function () {
		console.log('MatLab disconnected');
		matlab = undefined;
	});

	socket.on('timeout', function () {
		socket.destroy();
		console.log('MatLab disconnected');
		matlab = undefined;
	});

	console.log("Matlab connected :)  " + socket.remoteAddress);
	matlab = socket;
});

matlabserver.listen(3002, function () {
	console.log('MatLab Server is now online');
});

var socketServer = net.createServer(function (socket) {
	socket.on('end', function () {
		console.log('client disconnected');
		nodeCount--;
		console.log('Now there are ' + nodeCount + ' nodes connected!');
	});

	socket.on('timeout', function () {
		console.log('Closing socket to ' + socket.remoteAddress + ' due to a timeout');
		socket.destroy();
		nodeCount--;
		console.log('Now there are ' + nodeCount + ' nodes connected!');
	});

	console.log("Client connected :)  " + socket.remoteAddress);
	nodeCount++;
	console.log('Now there are ' + nodeCount + ' nodes connected!');
	socket.setTimeout(1000);

	socket.on('data', function (data) {
		chunks = String(data).split('\0');
		for (i = 0; i < chunks.length; ++i) {
			if (chunks[i].length == 31) {
				//ignore first three characters
				id = parseInt(socket.remoteAddress.split('.')[3], 10);
				buf = new Buffer(chunks[i]);
				decode(chunks[i].substring(3, chunks[i].length), id);
			}
		}
	});
});

socketServer.listen(3003, function () {
	console.log('Socket Server is now online');
});



function decode(chunk, id) {
	//console.log(chunk);
	
	//Get RSSI:
	rssi = chunk.substring(10, 12);
	rssi = parseInt(rssi, 16);
	if ((rssi & 0x80) > 0) {
		rssi = rssi - 0x100;
	}
	//console.log('RSSI: ' + rssi);

	channel = chunk.substring(8, 10);
	channel = parseInt(channel, 16);
	//console.log('Channel: ' + channel);

	//console.log('MAC: ' + chunk.substring(12, 24));
	mac = chunk.substring(12, 24);
	currentMacs[mac] = Date.now();

	timestamp = chunk.substring(0, 8);
	timestamp = parseInt(timestamp, 16);
	//console.log('Timestamp: ' + timestamp);

	if(matlab != undefined) {	
		console.log(Date.now() + ';' + rssi + ';' + mac + ';192.168.188.' + id);
		matlab.write(Date.now() + ';' + rssi + ';' + mac + ';192.168.188.' + id + '\r\n');
	}

	if (mac in packets && packets[mac].timestamp > (Date.now() - 1000)) {
		if (!(id in packets[mac].rssi)) {
			packets[mac].rssi[id] = rssi;
		}

		if (Object.keys(packets[mac].rssi).length == nodeCount) {
			if (mac in subscription) {
				subscription[mac].forEach(function (element) {
					if (element.connected) {
						element.emit('data', packets[mac]);
					}
				}, this);
			}
		}
	} else {
		time = Date.now();
		packets[mac] = {
			timestamp: time,
			rssi: {},
			addr: mac
		};
		packets[mac].rssi[id] = rssi;
	}
}

function printType(chunk) {
	//First decode the Type
	subtype = parseInt(chunk.charAt(24), 16);
	maintype = parseInt(chunk.charAt(25), 16);

	switch (maintype) {
		case 0:
			console.log('Type: Management');
			switch (subtype) {
				case 0:
					console.log('Subtype: Association Request');
					break;

				case 1:
					console.log('Subtype: Association Response');
					break;

				case 2:
					console.log('Subtype: Reassociation Request');
					break;

				case 3:
					console.log('Subtype: Reassociation Response');
					break;

				case 4:
					console.log('Subtype: Probe Request');
					break;

				case 5:
					console.log('Subtype: Probe Response');
					break;

				case 6:
					console.log('Subtype: Timing Advertisement');
					break;

				case 8:
					console.log('Subtype: Beacon');
					break;

				case 9:
					console.log('Subtype: ATIM');
					break;

				case 10:
					console.log('Subtype: Disassociation');
					break;

				case 11:
					console.log('Subtype: Authentication');
					break;

				case 12:
					console.log('Subtype: Action');
					break;

				case 13:
					console.log('Subtype: Action No Ack');
					break;

				default:
					console.log('Subtype: Reserved');
			}
			break;

		case 8:
			console.log('Type: Data');
			switch (subtype) {
				case 0:
					console.log('Subtype: Data');
					break;

				case 2:
					console.log('Subtype: Data + CF-Ack');
					break;

				case 2:
					console.log('Subtype: Data + CF-Poll');
					break;

				case 3:
					console.log('Subtype: Data + CF-Ack + CF-Poll');
					break;

				case 4:
					console.log('Subtype: Null (no data)');
					break;

				case 5:
					console.log('Subtype: CF-Ack(no data)');
					break;

				case 6:
					console.log('Subtype: CF-Poll(no data)');
					break;

				case 7:
					console.log('Subtype: CF-Ack + CF-Poll(no data)');
					break;

				case 8:
					console.log('Subtype: QoS Data');
					break;

				case 9:
					console.log('Subtype: QoS Data + CF-Ack');
					break;

				case 10:
					console.log('Subtype: QoS Data + CF-Ack');
					break;

				case 12:
					console.log('Subtype: QoS Data + CF-Ack + CF-Poll');
					break;

				case 13:
					console.log('Subtype: Reserved');
					break;

				case 14:
					console.log('Subtype: QoS CF-Poll (no data)');
					break;

				case 15:
					console.log('Subtype: QoS CF-Ack + CF-Poll (no data)');
					break;

				default:
					console.log('Never considered this...');
					break;
			}
			break;

		case 4:
			console.log('Type: Control');
			switch (subtype) {
				case 7:
					console.log('Subtype: Control Wrapper');
					break;

				case 8:
					console.log('Subtype: Block Ack Request (BlockAckReq)');
					break;

				case 9:
					console.log('Subtype: Block Ack (Block Ack)');
					break;

				case 10:
					console.log('Subtype: PS-Poll');
					break;

				case 11:
					console.log('Subtype: RTS');
					break;

				case 12:
					console.log('Subtype: CTS');
					break;

				case 13:
					console.log('Subtype: ACK');
					break;

				case 14:
					console.log('Subtype: CF-End');
					break;

				case 15:
					console.log('Subtype: CF-End + CF-Ack');
					break;

				default:
					console.log("Go home you are drunk...");
					break;
			}
			break;

		default:
			console.log('Go home you are drunk!');
	}
}
