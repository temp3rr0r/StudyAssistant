#include <fstream>
#include <future>
#include <wiringPi.h>

using namespace std;

class FlowSensor : public Sensor, public ISensor {
private:
    int rawFlow;
    int flow;
    
    void calibrateValue(void) {
        flow = rawFlow * 1;
    }

public:
    FlowSensor();
    FlowSensor(DbParameters);
    
    int getFlow() {
    	return flow;
    }
    
    virtual vector<string> getValues() {
    	setValue(to_string(flow));
    	return valuesVector;
    }

	void setFlow(int inFlow) {
		flow = inFlow;
	}
};

FlowSensor::FlowSensor() : FlowSensor(* new DbParameters("FlowSensor", "alid", "analoglist", "value", "aid", "analog", "fl")) { }

FlowSensor::FlowSensor(DbParameters newDbParameters) {
	flow = 0;
	rawFlow = 0;
	setParametersToSensor(newDbParameters);
}