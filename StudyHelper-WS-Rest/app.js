/**
 * Application starting point for VGTropics web service
 *
 * @type {exports}
 */

// Load the required modules
var express = require('express'),
  https = require('https'),
  path = require('path'),
  fs = require('fs');

// Load the local configuration
var local = require('./local.js');

// Authentication module.
var app = express();

// SSL key and certification files
var privateKey  = fs.readFileSync('keys/StudyHelper-key.pem', 'utf8');
var certificate = fs.readFileSync('keys/StudyHelper-cert.pem', 'utf8');
var credentials = { key: privateKey, cert: certificate };

// Create the https server
var httpsServer = https.createServer(credentials, app);
app.set(credentials);

// Port settings
app.set('port', local.port);

app.use(express.logger('dev')); // Log every request on the console
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// Instantiate the Request Manager
var RequestManager = require('./requestManager').RequestManager;
var requestManagerService = new RequestManager(app);

// Authentication module
app.use(RequestManager.auth);

// Start the http server
httpsServer.listen(app.get('port'), function(){
    console.log('Express server listening on port ' + app.get('port'));
});
