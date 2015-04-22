using namespace std;

class ThermalZone : public ISensor, public Sensor, public FileReader {
private:
	int rawValue;
	int value;
	int thermalZone;
	
	void calibrateValue(int _value) {
		value = _value * 1;
	}
	
	void readThermalZone(void) {
		thermalZone = readIntFromFile("/sys/class/thermal/thermal_zone0/temp", 6);
	}	
	int getRawValue(void) {
		return rawValue;
	}

public:
	ThermalZone();
	ThermalZone(DbParameters);

	virtual vector<string> getValues() {
		setValue(to_string(getThermalZone()));
		return valuesVector;
	}	
	
	int getThermalZone(void) {
		readThermalZone();
		calibrateValue(rawValue);
		return thermalZone;
	}
};

ThermalZone::ThermalZone() : ThermalZone(* new DbParameters("ThermalZone", "tid", "thermalzone", "temp", "lcid", "local", "tid")) { }
ThermalZone::ThermalZone(DbParameters newDbParameters) {
	value = 0;
	rawValue = 0;
	setParametersToSensor(newDbParameters);
}