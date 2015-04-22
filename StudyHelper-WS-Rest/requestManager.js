/**
 * Manages user table of the postgresql db
 *
 * @type {exports}
 */

// Load the required modules
var express = require('express'),
    fs = require('fs');

// Instantiate the user provider
var RequestProvider = require('./requestProviderPostgresql').RequestProvider;
var requestProvider = new RequestProvider();

// Manager resopone on requests declarations
RequestManager = function(app) {

    // Show all the logs from the db
    app.get('/logs', RequestManager.auth, function(req, res) {
        requestProvider.fetchAllLogs(function(error, logs) {
            res.send(logs);
        });
    });

    // Show the last log from the db
    app.get('/log', RequestManager.auth, function(req, res) {
        requestProvider.fetchLastLog(function(error, log) {
            res.send(log);
        });
    });

    // Delete all logs from the database
    app.get('/deletelogs', RequestManager.auth, function(req, res) {
        requestProvider.deleteAllLogs(function(error, log) {
            res.send(log);
        });
    });

    // Delete all images from the disk
    app.get('/deleteimages', RequestManager.auth, function(req, res) {
        requestProvider.deleteAllImages(function(error, log) {
            res.send(log);
        });
    });

    // Delete all logs from the database AND images from the disk
    app.get('/deleteall', RequestManager.auth, function(req, res) {
        requestProvider.deleteAllLogs(function(error, log) {
            requestProvider.deleteAllImages(function(error, log) {
            	if(!error) {
       				res.send("ALL images AND log entries were deleted");
            	}
	        });
        });
    });

    // Show the last image
    app.get('/image', RequestManager.auth, function(req, res) {
        requestProvider.fetchLastImage(function(error, image) {
        
			try {
				var imageName = image[0]['imagename'];
		
				var fileName = 'images/' + imageName + '.png';
				console.log('Image requested: ' + fileName);
		
				fs.exists(fileName, function(exists) {
					if (!exists) {
						console.log('Image doesn\'t exist');
						res.send('404 Not Found', 404);
					} else {
						console.log('Image exists');
						console.log('Sending Image');
						res.sendfile(fileName);
					}   
				});
        	} catch (ex) {
        		res.send("No Image captured yet");
        	}
        });
    });

    // Show the last image with stamp
    app.get('/imagestamp', RequestManager.auth, function(req, res) {
        requestProvider.fetchLastImage(function(error, image) {
	
			try {
				var imageName = image[0]['imagename'];
		
				var fileName = 'images/overlay_' + imageName + '.png';
				console.log('Image requested: ' + fileName);
		
				fs.exists(fileName, function(exists) {
					if (!exists) {
						console.log('Image doesn\'t exist');
						res.send('404 Not Found', 404);
					} else {
						console.log('Image exists');
						console.log('Sending Image');
						res.sendfile(fileName);
					}   
				});        	
			} catch (ex) {
        		res.send("No Image captured yet");
        	}

        });
    });

    // Show all the users from the db
	app.get('/users', RequestManager.auth, function(req, res) {
		requestProvider.fetchAllUsers(function(error, users) {
			res.send(users);
		});
	});

    // Show the user with the specific id
	app.get('/users/:id', RequestManager.auth, function(req, res) {
		requestProvider.fetchUserById(req.params.id, function(error, user) {
			if (user == null) {
				res.send(error, 404);
			} else {
				res.send(user);
			}
		});
	});

    // Redirect to login page
    app.get('/', RequestManager.auth, function(req, res) {
        res.redirect('/login');
    });

    // Login page
    app.get('/login', RequestManager.auth, function(req, res) {
        console.log('Login ok')
        res.send("Login OK", 200);
    });

    // Get an image
    app.get('/images/:fileName', RequestManager.auth, function(req, res) {

        var fileName = 'images/' + req.params.fileName;
        console.log('Image requested');

        fs.exists(fileName, function(exists) {
            if (!exists) {
                console.log('Image doesn\'t exist');
                res.send('404 Not Found', 404);
            } else {
                console.log('Image exists');
                console.log('Sending Image');
                res.sendfile(fileName);
            }
        });
    });
};

// Used in http authorization of the user against the db users table
RequestManager.auth = express.basicAuth(function(user, pass, callback) {

    // Call the sha256/base64 conversion method
    requestProvider.getEncryptedPassword(user, pass, function(error, encryptedPassword) {
        // Call the login/password method that compares values form the db
        requestProvider.fetchUserByUsernamePassword(user, encryptedPassword, function(error, users) {
            var userToCheck = user;
            var passwordToCheck = encryptedPassword;

            var dbLogin = "";
            var dbPassword = "";

            try {
                dbLogin = users[0].login;
                dbPassword = users[0].password;
				console.write("login, passwd " + dbLogin + dbPassword);
            } catch (ex) {}
	
            var result = (userToCheck === dbLogin && passwordToCheck === dbPassword);
            callback(null /* error */, result);
        });
    });
});


exports.RequestManager = RequestManager;
