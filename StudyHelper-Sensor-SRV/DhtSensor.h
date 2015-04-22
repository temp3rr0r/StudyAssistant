#include <fstream>
#include <wiringPi.h>
#include <stdio.h>

#define MAX_TIME 85
#define DHT11PIN 1

using namespace std;

class DhtSensor : public Sensor, public ISensor {
private:
	int rawTemp;
	int temp;
	int rawHum;
	int hum;
	int dht11_val[5] = {0,0,0,0,0};
	
	void calibrateTemp(int _temp) {
		temp = _temp * 1;
	}
	
	void calibrateHum(int _hum) {
		hum = _hum * 1;
	}
	
	bool readTempHum(void) {
		try {
			//wiringPiSetup();
		
			for (int i = 0; i < 5; i++)
				dht11_val[i]=0;
				
			uint8_t lststate=HIGH;
			uint8_t counter=0;
			uint8_t j=0,i;
			float farenheit;
			for(i=0;i<5;i++)
				dht11_val[i]=0;
			pinMode(DHT11PIN,OUTPUT);
			digitalWrite(DHT11PIN,LOW);
			delay(18);
			digitalWrite(DHT11PIN,HIGH);
			delayMicroseconds(40);
			pinMode(DHT11PIN,INPUT);
			for(i=0;i<MAX_TIME;i++) {
				counter=0;
				while(digitalRead(DHT11PIN)==lststate) {
					counter++;
					delayMicroseconds(1);
					if(counter==255)
							break;
				}		
				lststate=digitalRead(DHT11PIN);
				if(counter==255)
					break;
				// top 3 transistions are ignored
				if((i>=4)&&(i%2==0)) {
					dht11_val[j/8]<<=1;
					if(counter>16)
						dht11_val[j/8]|=1;
					j++;
				}
			}
			
			// verify cheksum and print the verified data
			if((j>=40)&&(dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF))) {
				rawHum = dht11_val[0]; 
				rawTemp = dht11_val[2];
				return true;
			} else {
				rawTemp = 0;
				rawHum = 0;
				return false;
			}
		} catch(...) { }
	}	

public:
	DhtSensor();
	DhtSensor(DbParameters);
	
	float getTemp(void) {
		while(!readTempHum()) { }
		calibrateTemp(rawTemp);
		return temp;
	}
	
	float getHum(void) {
		while(!readTempHum()) { }
		calibrateHum(rawHum);
		return hum;
	}

	virtual vector<string> getValues() {
		try {
			while(!readTempHum()) { }
			setValue(to_string(getTemp()));
			setValue(to_string(getHum()));
			return valuesVector;
		} catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << endl;
		}
	}
};

DhtSensor::DhtSensor() : DhtSensor(* new DbParameters("DhtSensor", "alid", "analoglist", "value", "did", "digital", "dhtamb")) { }

DhtSensor::DhtSensor(DbParameters newDbParameters) {
	temp = 0;
	rawTemp = -5;
	hum = 0;
	rawHum = 0;
	setParametersToSensor(newDbParameters);
  	newDbParameters.setMasterField("dhthum");
	setParametersToSensor(newDbParameters);
	wiringPiSetup();
}
