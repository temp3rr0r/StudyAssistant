#include <fstream>

using namespace std;

class TempOneWire : public Sensor, public ISensor, public FileReader {
private:
    int rawTemperature;
    int temperature;
    string deviceAddress;
    
    void calibrateValue() {
        temperature = rawTemperature * 1;
    }
    
    void readValue() {
    	rawTemperature = readIntFromFile("/sys/bus/w1/devices/" + deviceAddress + "/w1_slave", 6, 69);
    }

public:
    TempOneWire();
    TempOneWire(DbParameters);

    int getTemp(void) {
        readValue();
        calibrateValue();
        return temperature;
    }
    
    virtual vector<string> getValues() {
    	setValue(to_string(getTemp()));
    	return valuesVector;
    }
    
};

TempOneWire::TempOneWire() : TempOneWire(* new DbParameters("TempOneWire" , "dlid", "digitallist", "value", "did", "digital", "aq")) { }

TempOneWire::TempOneWire(DbParameters newDbParameters) {
	temperature = 0;
	rawTemperature = 0;
	deviceAddress = "28-0000061535a5";
	setParametersToSensor(newDbParameters);
}