#include <vector>
#include <tuple>
#include "ISensor.h"

using namespace std;
 
// Base class
class Sensor {
public:
   virtual ~Sensor() { };
   virtual vector<string> getValues() = 0;
   
   void setValue(string inValue) {
   		try {
   			valuesVector.push_back(inValue);
   		} catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what();
		}
   }
	
	string getName(int i) {
		try {
			if (i <= dbVectorParameters.size())
				return dbVectorParameters[i].getName();
			else
				return "-1";
		} catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << " within sensor class"<< endl;
		}
	}
		
	string getId(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getId();
		else
			return "-1";
	}	
		
	string getTable(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getTable();
		else
			return "-1";
	}
	
	string getField(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getField();
		else
			return "-1";
	}
	string getMasterId(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getMasterId();
		else
			return "-1";
	}	
		
	string getMasterTable(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getMasterTable();
		else
			return "-1";
	}
	
	string getMasterField(int i) {
		if (i <= dbVectorParameters.size())
			return dbVectorParameters[i].getMasterField();
		else
			return "-1";
	}
	
	string getName() {
		return getName(0);
	}
	
    string getValue() {
		return getValue(0);
    }
    	
	string getId() {
		return getId(0);
	}  
	
	string getTable() {
		return getTable(0);
	}	
	
	string getField() {
		return getField(0);
	}
	
	string getMasterId() {
		return getMasterId(0);
	}  
	
	string getMasterTable() {
		return getMasterTable(0);
	}	
	
	string getMasterField() {
		return getMasterField(0);
	}    
	
	string getValue(int i) {
		try {
			vector<string> stringValues = getValues();
			if (i <= stringValues.size())
				return stringValues.at(i);
			else
				return "-1";
		} catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << endl;
		}
    }
    
    int countValues() {
    	vector<string> stringValues = getValues();
    	return stringValues.size();
    }
    
    void setParametersToSensor(DbParameters newDbParameters) {
		try {
			dbVectorParameters.push_back(newDbParameters);
		} catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << endl;
		}
    }
    
protected:
	vector<string> valuesVector;
    vector<DbParameters> dbVectorParameters;
};