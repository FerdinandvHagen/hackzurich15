angular.module('wiloc').
    factory('dataService', function (socketFactory) {


    return socketFactory();
});