var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var request = require('request');
var SunCalc = require('suncalc');
var timerDict = require('./timerDictionary.js');

var app = express();

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.set('view engine', 'jade');

// This interface will trigger the given event if the given time/location is at "dusk"
// Here "dusk" is defined as half an hour before the sunset
// Example query: http://lab.grapeot.me/ifttt/dusk?key=MY_KEY&lat=51.5&lon=-0.1&event=lightson
app.get('/dusk', function(req, res) {
    var key = req.query.key;
    var lat = parseFloat(req.query.lat);
    var lon = parseFloat(req.query.lon);
    var event = req.query.event;

    // get sunset time
    var times = SunCalc.getTimes(new Date(), lat, lon);
    var sunsetTime = times.sunset;
    var triggered = false;
    if (sunsetTime - new Date() < 0.5 * 3600 * 1000) {
        var url = 'https://maker.ifttt.com/trigger/' + event + '/with/key/' + key;
        console.log('URL = ' + url);
        request.post(url);
        triggered = true;
    }
    res.send('Request recorded. Sunset time = ' + sunsetTime + '. ' + (triggered ? 'Triggered. ' : 'Not triggered.'));
});

app.use('/cancel', function(req, res) {
    var key = req.query.key;
    var event = req.query.event;
    if (key == undefined || event == undefined) {
        res.send("Error: must specify key and event in the URL.");
        return;
    }
    timerDict.removeAllTimers(key, event);
    res.send("All future times for { key = " + key + ", event = " + event + " } are removed. ");
});

app.use('/delay', function(req, res) {
    // Parse the inputs
    var delay = parseFloat(req.query.t); // in minutes, now allowing float
    var key = req.query.key;
    var event = req.query.event;
    var reset = req.query.reset == '1';
    if (reset == undefined) {
        reset = false;
    }
    if (delay == undefined || key == undefined || event == undefined) {
        res.send("Error: must specify t (for delay time) and key and event in the URL.");
        return;
    }
    var maxSupportedDelay = Math.pow(2,31) - 1;
    if ((delay * 60 * 1000) > maxSupportedDelay) {
        // setTimeout() uses a 32-bit signed integer for its delay parameter
        // A larger number causes setTimeout() to execute immediately, which
        // is likely not what the user would want.
        // Throw an explicit error if the passed delay time will exceed it.
        res.send("Error: Delay time cannot exceed " + (maxSupportedDelay / 60 / 1000) + " minutes.");
        return;
    }

    // Fetch the values from the POST body
    var value1 = req.body.Value1;
    var value2 = req.body.Value2;
    var value3 = req.body.Value3;
    var bodyToSend = {
        value1: value1,
        value2: value2,
        value3: value3
    };
    console.log('body = ' + JSON.stringify(bodyToSend));

    var timerId = setTimeout(function() {
        var url = 'https://maker.ifttt.com/trigger/' + event + '/with/key/' + key;
        console.log('URL = ' + url);
        request.post(url, { form: bodyToSend }, function(error, response, body) { });
        timerDict.removeTimer(key, event, timerId);
    }, delay * 60 * 1000);
    // If reset is specified, cancel all existing timers
    if (reset) {
        timerDict.removeAllTimers(key, event);
    }
    // Add the current timer to the timer dictionary
    timerDict.addTimer(key, event, timerId);
    res.send('Request recorded. Delay = ' + delay + ' minutes, event = ' + event + ', key = ' + key + ', reset = ' + reset);
});

app.use('/', function(req, res) {
    res.status(200).json({"status": "ok", "msg": "You've reached the API root. Service is running." });
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
    console.log(err.message);
    res.status(err.status || 500);
    res.render('error', {
        message: err.message,
        error: {}
    });
});

module.exports = app;