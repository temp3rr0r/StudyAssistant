using namespace std;

class CpuFrequency : public ISensor, public Sensor, public FileReader {
private:
	int rawValue;
	int value;
	
	void calibrateValue(int _value) {
		value = _value * 1;
	}

	void readCpuFreq(void) {
		rawValue = readIntFromFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", 7);
	}
	int getRawValue(void) {
		return rawValue;
	}
	

public:
	CpuFrequency();
	CpuFrequency(DbParameters);
	
	int getCpuFreq(void) {
		readCpuFreq();
		calibrateValue(rawValue);
		return value;
	}
	virtual vector<string> getValues() {
		setValue(to_string(getCpuFreq()));
		return valuesVector;
	}
};

CpuFrequency::CpuFrequency() : CpuFrequency(* new DbParameters("CpuFrequency", "cid", "cpufreq", "mhz", "lcid", "local", "cid")) { }

CpuFrequency::CpuFrequency(DbParameters newDbParameters)
{	
	value = 0;
	rawValue = 0;
	setParametersToSensor(newDbParameters);
}