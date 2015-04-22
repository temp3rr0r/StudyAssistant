/**
 * Provides data to the User Manager
 *
 * @type {number}
 */

// Load the required modules
var pg = require('pg'),
    DBWrapper = require('node-dbi').DBWrapper,
    DBExpr = require('node-dbi').DBExpr,
    crypto = require('crypto'),
    fs = require('fs');

// Get the server connection credentials
var local = require('./local.js');

// User Provider declarations
RequestProvider = function() {

    // Postgresql
    dbWrapperPostgresql = new DBWrapper( 'pg', local.dbConnectionConfigPostgres );
    dbWrapperPostgresql.connect();

    this.users = [];

    // Fetch all the rows from the user table
    this.fetchAllUsers = function(cb) {
        var selectUsertablePostgresql = dbWrapperPostgresql.getSelect()
            .from('usertable')
            .where('deleted IS NULL');

        console.log('Showing all users');
        dbWrapperPostgresql.fetchAll(selectUsertablePostgresql, function(error, usertable) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, usertable);
            }
        });
    };

    // Fetch the last Image entry
    this.fetchLastImage = function(cb) {
        var selectLastLogPostgresql = 'SELECT ' +
                ' imagename ' +
                ' FROM log ' +
                ' ORDER BY created DESC ' +
                        ' LIMIT 1;'

        console.log('Showing last Log entry');
        dbWrapperPostgresql.fetchAll(selectLastLogPostgresql, function(error, lastLogRow) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, lastLogRow);
            }
        });
    };

    // Fetch the last Log entry
    this.fetchLastLog = function(cb) {
        var selectLastLogPostgresql = 'SELECT ' +
        	' * ' +
        	' FROM log ' +
        	' ORDER BY created DESC ' +
			' LIMIT 1;'

        console.log('Showing last Log entry');
        dbWrapperPostgresql.fetchAll(selectLastLogPostgresql, function(error, lastLogRow) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, lastLogRow);
            }
        });
    };

    // Fetch all the rows from the log table
    this.fetchAllLogs = function(cb) {
        var selectLogstablePostgresql = dbWrapperPostgresql.getSelect()
            .from('log');

        console.log('Showing all logs');
        dbWrapperPostgresql.fetchAll(selectLogstablePostgresql, function(error, logtable) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, logtable);
            }
        });
    };

    // Fetch one row for a specific user id
    this.fetchUserById = function(id, cb) {
        var selectIdUsertableIdPostgresql = dbWrapperPostgresql.getSelect()
            .from('usertable')
            .where('id=?', id)
            .where('deleted IS NULL');

        console.log('Fetching user by Id');
        dbWrapperPostgresql.fetchRow(selectIdUsertableIdPostgresql, function(error, usertable) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, usertable);
            }
        });
    };

    // Fetch all the rows for a specific login and password
    this.fetchUserByUsernamePassword = function(login, password, cb) {
        var selectIdUsertablePostgresql = dbWrapperPostgresql.getSelect()
            .from('usertable')
            .where('login=?', login)
            .where('password=?', password)
            .where('deleted IS NULL');

        console.log('Fetching user by login/pass');
        dbWrapperPostgresql.fetchAll(selectIdUsertablePostgresql, function(error, usertable) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, usertable);
            }
        });
    };

    // Delete all logs from the "log" table in Postgres
    this.deleteAllLogs = function(cb) {
        var deleteAllLogsPostgresql = 'DELETE ' +
        	' FROM log;';

        console.log('Deleting ALL Log entries');
        dbWrapperPostgresql.fetchAll(deleteAllLogsPostgresql, function(error, deleteResult) {
            if (error) {
                cb(error, null);
            } else {
                cb(null, "All log entries were deleted");
            }
        });
    };
    
    // Delete all images in the /images directory
    this.deleteAllImages = function(cb) {
    	var imagesDirectoryPath = "images/";
    	
    	// Read all the element of the directory (not recursive, shallow level)
    	fs.readdir(imagesDirectoryPath, function(error, imageFileNames) {
    		if (error) {
    			cb(error, null);
    		} else {
    		
    			// For every distinct image file detected
    			imageFileNames.forEach(function(currentImageFileName) {    			
    				// If current image does exist
	    			fs.exists(imagesDirectoryPath + currentImageFileName, function(exists) {
						if (!exists) {
							console.log('Image doesn\'t exist');
							cb(error, null);
						} else {
			    			// Do delete the image
	    					fs.unlink(imagesDirectoryPath + currentImageFileName);						
						}   
					});        	
    			});
    			
    			// Done    			
    			cb(null, "All captured images were deleted.");
    		}
    	});
    }

    // Convert the password into sha-256/base64 form
    this.getEncryptedPassword = function(login, password, cb) {

        var returningPassword = 'invalid';

        console.log('Encrypting password field');
        // Perform sha256 encryption
        var shasum = crypto.createHash('sha256');
        shasum.update(password);
        shasum.update('{' + login + '}');

        // Convert to base64
        returningPassword = shasum.digest('base64');

        // Return the value
        if (returningPassword != 'invalid') {
            cb(null, returningPassword);
        } else {
            cb(null, null);
        }
    };
};

exports.RequestProvider = RequestProvider;
