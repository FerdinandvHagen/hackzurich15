angular.module('wiloc').
    factory('dataService', function () {

        var socket = io('http://192.168.188.20:3000', {path: ''});

        return {
            getMACs: function (callback) {

                socket.emit('getmac', 0);
                socket.on('currentMacs', function (msg) {
                    callback(msg);
                    console.log(msg);
                });
            },

            subscribe: function (mac, callback) {
                socket.emit('subscribe', mac);
                socket.on('data', function (msg) {
                    callback(msg);
                    console.log(msg);
                });
            }
        };
    });