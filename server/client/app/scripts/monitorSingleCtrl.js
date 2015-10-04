'use strict';

angular.module('wiloc')
    .controller('MonitorSingleCtrl', function ($scope, $rootScope, $routeParams, $location, dataService) {

        console.log($routeParams.mac);

        var limit = 60 * 10,
            duration = 900,
            now = new Date(Date.now() - duration)

        var width = 900, height = 300;

        var groups = [];

        var x = d3.time.scale()
            .domain([now - (limit - 2), now - duration])
            .range([0, width])

        var y = d3.scale.linear()
            .domain([-120, 0])
            .range([height, 0])

        var line = d3.svg.line()
            .x(function (d, i) {
                return x(now - (limit - 1 - i) * duration)
            })
            .y(function (d) {
                return y(d)
            })

        var svg = d3.select('.graph').append('svg')
            .attr('class', 'chart')
            .attr('width', width)
            .attr('height', height + 50)

        var axis = svg.append('g')
            .attr('class', 'x axis')
            .attr('transform', 'translate(0,' + height + ')')
            .call(x.axis = d3.svg.axis().scale(x).orient('bottom'))

        var paths = svg.append('g')

        var groups = [];

        dataService.subscribe($routeParams.mac, function (data) {

            for (var key in data.rssi) {
                if (data.rssi.hasOwnProperty(key)) {

                    if (!_.some(groups, {'node_id': key})) {
                        console.log("adding new rssi");

                        var ndata = d3.range(limit).map(function() {
                            return 0
                        });

                        var ngroup = {
                            node_id: key,
                            color: ncolor,
                            data: ndata
                        };

                        var ncolor = d3.scale.category20()(groups.length);

                        ngroup.path = paths.append('path')
                            .data([ngroup.data])
                            .attr('class', ngroup.node_id + ' group')
                            .style('stroke', ngroup.color);

                        groups.push(ngroup);

                    }

                    _.find(groups, 'node_id', key).data.push(data.rssi[key]);
                    _.find(groups, 'node_id', key).path.attr('d', line);
                }
            }

            now = data.timestamp;

            // Shift domain
            x.domain([now - (limit - 2) * duration, now - duration])

            // Slide x-axis left
            axis.transition()
                .duration(duration)
                .ease('linear')
                .call(x.axis)

            // Slide paths left
            paths.attr('transform', null)
                .transition()
                .duration(duration)
                .ease('linear')
                .attr('transform', 'translate(' + x(now - (limit - 1) * duration) + ')')


            // Remove oldest data point from each group

            groups.forEach(function (group) {
                group.data.shift()
            });
            console.log(data);

        });


    });
