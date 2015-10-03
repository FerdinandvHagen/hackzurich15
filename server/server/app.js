var express = require('express');
var app = express();

var net = require('net');

var MongoClient = require('mongodb').MongoClient;
var assert = require('assert');
var ObjectId = require('mongodb').ObjectID;

var url = 'mongodb://localhost:27017/hackzurich15';
MongoClient.connect(url, function (err, db) {
	assert.equal(null, err);
	console.log("Connected correctly to server.");
	db.close();
});

var insertDocument = function (db, callback) {
	db.collection('restaurants').insertOne({
		"address": {
			"street": "2 Avenue",
			"zipcode": "10075",
			"building": "1480",
			"coord": [-73.9557413, 40.7720266]
		},
		"borough": "Manhattan",
		"cuisine": "Italian",
		"grades": [
			{
				"date": new Date("2014-10-01T00:00:00Z"),
				"grade": "A",
				"score": 11
			},
			{
				"date": new Date("2014-01-16T00:00:00Z"),
				"grade": "B",
				"score": 17
			}
		],
		"name": "Vella",
		"restaurant_id": "41704620"
	}, function (err, result) {
		assert.equal(err, null);
		console.log("Inserted a document into the restaurants collection.");
		callback(result);
	});
};

MongoClient.connect(url, function (err, db) {
	assert.equal(null, err);
	var socketServer = net.createServer(function (socket) {
		socket.on('end', function () {
			console.log('client disconnected');
		});

		socket.on('timeout', function () {
			console.log('Closing socket to ' + socket.remoteAddress + ' due to a timeout');
			socket.destroy();
		})

		console.log("Client connected :)  " + socket.remoteAddress);
		socket.setTimeout(60000);
		
		socket.on('data', function (data) {
			chunks = String(data).split('\0');
			for (i = 0; i < chunks.length; ++i) {
				if (chunks[i].length == 31) {
					//ignore first character
					console.log('ID: ' + chunks[i].charCodeAt(2));
					decode(chunks[i].substring(3, chunks[i].length));
					db.collection('dump').insertOne({
						"data": data,
						"timestamp": process.hrtime()
					}, function (err, result) {
						assert.equal(err.null);
					});
				}
			}
		});
	});

	socketServer.listen(3003, function () {
		console.log('Socket Server is now bound');
	});
});



function decode(chunk) {
	console.log(chunk);
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

	console.log('MAC: ' + chunk.substring(12, 24));

	timestamp = chunk.substring(0, 8);
	timestamp = parseInt(timestamp, 16);
	//console.log('Timestamp: ' + timestamp);
}

