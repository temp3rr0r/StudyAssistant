#ifndef ARDUINOANALOGSENSOR_H
#define ARDUINOANALOGSENSOR_H

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <fstream>
 
#define ADDRESS 0x04
static const char *devName = "/dev/i2c-1";

using namespace std;

class ArduinoAnalogSensor {
private:
    
	int alcohol;
	int methaine;
	int loudness;
	int light;
    
	int getAnalogValue(int analogId) {
		
		int returningValue = -1;
		int file; 
		
		try {
			// If device file exists
			if ((file = open(devName, O_RDWR)) >= 0) {	/// With the declared address
				if (ioctl(file, I2C_SLAVE, ADDRESS) >= 0) {	/// Send value
					unsigned char cmd[16];
					cmd[0] = analogId;
					if (write(file, cmd, 1) == 1) {
						// 1ms seems to be enough but it depends on what workload it has
						usleep(10000);

						char buf[1];
						if (read(file, buf, 1) == 1) {
							int temp = (int) buf[0];
							returningValue = temp;
						}
					}

					// Now wait else you could crash the arduino by sending requests too fast
					usleep(10000);
				}			
				close(file);
			} 
		} catch (...) {}
			
		return returningValue;
	}

public:
    ArduinoAnalogSensor();

	int getAlcohol(void) {
		// Method that reads arduino analog 0
		alcohol = getAnalogValue(0);
		return alcohol;
	}
	
	int getMethaine(void) {
		// Method that reads arduino analog 1
		methaine = getAnalogValue(1);
		return methaine;
	}
	
	int getLoudness(void) {
		// Method that reads arduino analog 2
		int min = 1024;
		int max = 0;
		int currentValue = -1;

		for (int i = 0; i < 100; i++) {	
			currentValue  = getAnalogValue(2);
			if (currentValue < min)
				min = currentValue;
			if (currentValue > max)
				max = currentValue;
		}
		loudness = max - min;
		return loudness;
	}
	
	int getLight(void) {
		// Method that reads arduino analog 3
		light = getAnalogValue(3);
		return light;
	}
    
};

ArduinoAnalogSensor::ArduinoAnalogSensor() {
	alcohol = 0;
	methaine = 0;
	loudness = 0;
	light = 0;
}

#endif /* ARDUINOANALOGSENSOR_H */