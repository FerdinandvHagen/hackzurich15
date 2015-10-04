'use strict';

angular.module('wiloc')
    .controller('MonitorCtrl', function ($scope, $rootScope, $routeParams, $location, dataService) {

        dataService.getMACs(function (msg) {
            var timestamp = new Date();
            var hosts = [];

            for (var key in msg) {
                if (msg.hasOwnProperty(key)) {
                    hosts.push({mac: key, timestamp: timestamp - msg[key]});
                }
            }

            console.log(hosts);
            $scope.hosts = _.sortBy(hosts, ['timestamp'], ['desc']);

            $scope.$apply();
        });

        $scope.monitor = function (host) {
            console.log("starting to monitor" + host.mac);

            $location.path("/monitor/" + host.mac);
        }
    });
