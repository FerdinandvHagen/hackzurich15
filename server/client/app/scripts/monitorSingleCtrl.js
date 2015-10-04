'use strict';

angular.module('wiloc')
    .controller('MonitorSingleCtrl', function ($scope, $rootScope, $routeParams, $location, dataService) {

        console.log($routeParams.mac);

        dataService.subscribe($routeParams.mac, function (data) {

            console.log(data);

        });

    });
