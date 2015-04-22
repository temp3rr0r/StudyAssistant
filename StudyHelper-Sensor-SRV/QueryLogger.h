using namespace std;

class QueryLogger {
private:
     static bool instanceFlag;
     static QueryLogger *single;
     QueryLogger() {
         //private constructor
     }
public:
    static QueryLogger* getInstance();
    string level3(vector<tuple<string, string>>, DbController);
    vector<tuple<string, string>> level2(vector<tuple<Sensor*, string>>, DbController, vector<tuple<string, string>>);
    ~QueryLogger() {
        instanceFlag = false;
    }
};

bool QueryLogger::instanceFlag = false;
QueryLogger* QueryLogger::single = NULL;
QueryLogger* QueryLogger::getInstance() {
    if(! instanceFlag) {
        single = new QueryLogger();
        instanceFlag = true;
        return single;
    } else {
        return single;
    }
}

vector<tuple<string, string>> QueryLogger::level2(vector<tuple<Sensor*, string>> analogVector, DbController myController, vector<tuple<string, string>> id2) {
	try {
		string analogSelectQuery;
		vector<string> fieldsVector, valuesVector;
		string id1, name;
		int j = 0;
		for (int i = 0; i < analogVector.size(); i++) {
			fieldsVector.push_back((get<0>(analogVector.at(i)))->getMasterField(j));
			valuesVector.push_back(get<1>(analogVector.at(i)));
			name = (get<0>(analogVector.at(i)))->getName();
			if ((name == "DhtSensor" || name == "I2C") && j == 0)
				j++;
			else
				j = 0;
		}
		analogSelectQuery = myController.querySelectCreator((get<0>(analogVector.at(0)))->getMasterId(), (get<0>(analogVector.at(0)))->getMasterTable(), fieldsVector, valuesVector);		
		id1 = myController.execCommandSelect(analogSelectQuery);
		if (id1 == "0") {
			string analogInsertQuery = myController.queryInsertCreator((get<0>(analogVector.at(0)))->getMasterTable(), fieldsVector, valuesVector);
			id1 = myController.execCommandInsert(analogInsertQuery);
		}
		id2.push_back(make_tuple((get<0>(analogVector.at(0)))->getMasterId(), id1));
		return id2;
	} catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << endl;
	}
}

string QueryLogger::level3(vector<tuple<string, string>> id2, DbController myController) { 
	try {
		// Added created field reference
		id2.push_back(make_tuple("created", "current_timestamp"));

		vector<string> fieldsVector, valuesVector;
	
		for (int i = 0; i < id2.size(); i++) {
			fieldsVector.push_back(get<0>(id2.at(i)));
			valuesVector.push_back(get<1>(id2.at(i)));
		}
	
		// Last query for the log table
		string logSelectQuery = myController.queryInsertCreator("log", fieldsVector, valuesVector);
		return myController.execCommandInsert(logSelectQuery);
	} catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << " within level 3" << endl;
	}
}