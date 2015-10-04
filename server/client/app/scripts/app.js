'use strict';

/**
 * @ngdoc overview
 * @name WILOC
 * @description
 * #
 *
 * Main module of the application.
 */
angular.module('wiloc', [
        'ngRoute',
        'btford.socket-io'
        //'ui.bootstrap'
    ])
    .config(function ($routeProvider) {
        $routeProvider
            .when('/map', {
                templateUrl: 'template/map.html',
                controller: 'MapCtrl'
            })
            .when('/monitor', {
                templateUrl: 'template/monitor.html',
                controller: 'MonitorCtrl'
            })
            .when('/monitor/:mac', {
                templateUrl: 'template/monitorSingle.html',
                controller: 'MonitorSingleCtrl'
            })
    }).run(function ($rootScope, $location) {
        $rootScope.l = $location;
    });