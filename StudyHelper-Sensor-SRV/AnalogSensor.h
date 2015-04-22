#include <wiringPiSPI.h>
#include <math.h>

using namespace std;

class AnalogSensor : public Sensor, public ISensor {
private:
	int rawValue;
	float value;
	float rawRange = 1024; // 3.3v
	float logRange = 5.0; // 3.3v = 10^5 lux
	
	float RawToLux(int raw) {
	  float logLux = raw * logRange / rawRange;
	  return pow(10, logLux);
	}
	
	float calibrateValue(int _value) {
		return RawToLux(_value);
	}
	
public:
	AnalogSensor();
	AnalogSensor(DbParameters);

	virtual vector<string> getValues() {
		wiringPiSPISetup(0, 100000);
		for (int i = 0; i < 8; i++) {
			// To lux, now it causes sql error because it is float value
			//if (i == 0)	// 0 
			//	setValue(to_string(calibrateValue(getAdc(i))));
			//else
				setValue(to_string(getAdc(i)));
		}
		return valuesVector;
	}
	
	int getAdc(int adcnum) {
			uint8_t buff[3];
			int adc;
			if ((adcnum > 7) || (adcnum < 0))
					return -1;

			buff[0] = 1;
			buff[1] = (8 + adcnum)<<4;
			buff[2] = 0;
		try {
				wiringPiSPIDataRW(0, buff, 3);
				adc = ((buff[1]&3) << 8) + buff[2];
		} catch(...) {
			adc = 0;
		}
		return adc;
	}
};

AnalogSensor::AnalogSensor() : AnalogSensor(* new DbParameters("AnalogSensor", "alid", "analoglist", "value", "aid", "analog", "lumlog")) { }

AnalogSensor::AnalogSensor(DbParameters newDbParameters) {
	value = 0;
	rawValue = 0;
	setParametersToSensor(newDbParameters);
}

