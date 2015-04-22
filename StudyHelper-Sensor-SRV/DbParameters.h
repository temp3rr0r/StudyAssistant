#include <string>

using namespace std;

class DbParameters {
private:
	string name;
	string id;
	string table;
	string field;
	string masterId;
	string masterTable;
	string masterField;
public:		
	void setMasterField(string inMasterField) {
		masterField = inMasterField;
	}	
	string getName() {
		return name;
	}		
	string getId() {
		return id;
	}		
	string getTable() {
		return table;
	}		
	string getField() {
		return field;
	}		
	string getMasterId() {
		return masterId;
	}		
	string getMasterTable() {
		return masterTable;
	}		
	string getMasterField() {
		return masterField;
	}	

	DbParameters(string _name, string _id, string _table, string _field, string _masterId, string _masterTable, string _masterField) {
		name = _name;
		id = _id;
		table = _table;
		field = _field;
		masterId = _masterId;
		masterTable = _masterTable;
		masterField = _masterField;
	}
};