#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>

#include "Sensor.h"
#include "TempOneWire.h"
//#include "I2C.cpp"
#include "DhtSensor.h"
#include "Bmp085.h"
#include "DbController.h"
#include "ThermalZone.h"
#include "CpuFrequency.h"
#include "TextLogger.h"
#include "StringPrecision.h"

#define SERVICE 0
#define DEBUG 1
#define TEXT_LOG 1

#define LOG_INTERVAL 5*60	// Seconds

using namespace std;

void Init(void) {
    system("scripts/modprobe.sh");
    system("scripts/one-wire.sh");
	system("scripts/i2c.sh");
}

void makeService(void) {
	// Our process ID and Session ID
	pid_t pid, sid;
		
	// Fork off the parent process
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	
	// If we got a good PID, then we can exit the parent process
	if (pid > 0)
		exit(EXIT_SUCCESS);

	// Change the file mode mask
	umask(0);
		   
	// Create a new SID for the child process
	sid = setsid();
	if (sid < 0) {
			// Log the failure
			exit(EXIT_FAILURE);
	}
	
	// Change the current working directory
	if ((chdir("/")) < 0) {
			// Log the failure
			exit(EXIT_FAILURE);
	}
	
	// Close out the standard file descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Daemon-specific initialization goes here
}

int main(void)
{
	cout << "Starting SleepHelper service" << endl;
	if (SERVICE)
		makeService();
	
	// Load mods/libraries and start the flow thread	
    Init();
	
	while(1) {
	
		// Instantiate Sensors
//		I2C mpl;
		TempOneWire digitalTemp;
		DhtSensor dht;    
		ThermalZone thermal;
		CpuFrequency cpuFreq;		
		Bmp085 bmp085;

		// Instantiate Controllers
		DbController myController;	
		TextLogger *tx;

		tx = TextLogger::getInstance();

		// Variables	
		time_t timestamp;
		time(&timestamp);
		string currentTimestamp = to_string(timestamp);
		
               if(TEXT_LOG)
                      tx->log(currentTimestamp + ": Init.");
               
		if (DEBUG) {
			cout << "Digital Temp: " << digitalTemp.getTemp() << endl;
			cout << "Cpu frequency: " << cpuFreq.getCpuFreq() << endl;
			cout << "Thermal zone: " << thermal.getThermalZone() << endl;			
			cout << "DHT temp: " << dht.getTemp() << endl;
			cout << "DHT humidity: " << dht.getHum() << endl;

			cout << "Bmp085 Temp: " << bmp085.getTemp() << endl;
		}	
		
		/*
		  ds18b20temp integer,
		  dht11temp real,
		  dht11hum real,
		  bmp180temp real,
		  bmp180pressure real,
		  cpufreq integer,
		  thermalzone integer,
		  created timestamp with time zone,
		*/
		
		string insertString;
		insertString = "INSERT INTO \"log\" (ds18b20temp, dht11temp, dht11hum, bmp180temp, bmp180pressure, cpufreq, thermalzone, created) VALUES (";
		insertString.append(to_string(digitalTemp.getTemp()));
		insertString.append(", ");
		insertString.append(to_string(dht.getTemp()));
		insertString.append(", ");
		insertString.append(to_string(dht.getHum()));
		insertString.append(", ");
		
		insertString.append(to_string(bmp085.getTemp()));
		insertString.append(", ");
		insertString.append(to_string(bmp085.getPressure()));
		insertString.append(", ");

		insertString.append(to_string(cpuFreq.getCpuFreq()));
		insertString.append(", ");
		insertString.append(to_string(thermal.getThermalZone()));
		insertString.append(", to_timestamp(");
		insertString.append(currentTimestamp); // created timestamp
		insertString.append("));");
		
		if (DEBUG)
			cout << insertString << endl;

		try {
			if(myController.connect()) {
		
				myController.execCommandInsert(insertString);
				myController.close();
			}
		} catch (...) {
			cout << "Postgres Exception." << endl;
	               if(TEXT_LOG)
	                        tx->log(currentTimestamp + ": Postgres Exception.");

		}		
	
		sleep(LOG_INTERVAL);
	}
	exit(EXIT_SUCCESS);
}	

