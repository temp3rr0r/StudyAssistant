/**
 * This file contains local settings
 *
 * @type {number}
 */

module.exports = {

    // Server connection credentials
   dbConnectionConfigPostgres: {
        host: 'localhost',
        user: 'madks',
        password: 'ma121284',
        database: 'sensors'
    },
    
// Server HTTP port
    port: process.env.PORT || 8443, // node.js public access WS port

    // Development mode
    environment: process.env.NODE_ENV || 'development'
};
