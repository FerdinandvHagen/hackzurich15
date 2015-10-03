var express = require('express')
var serveStatic = require('serve-static')

var app = express()

app.use('/app', express.static('app'));
app.use('/bower_components', express.static('bower_components'));
app.listen(3000);