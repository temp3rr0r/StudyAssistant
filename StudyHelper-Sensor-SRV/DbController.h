#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "libpq-fe.h"

using namespace std;

class DbController {
private:
	const char *conninfo;
	PGconn *conn;
	PGresult *res;
	const char *command;
	int nFields;
	int i, j;
	const char *paramValues[1];
	static void exit_nicely(PGconn *conn) {
		PQfinish(conn);
	}
	
public:
	DbController();
	bool connect(void) {
		conn = PQconnectdb(conninfo);

		if (PQstatus(conn) != CONNECTION_OK) {
			exit_nicely(conn);
			return false;
		}
		return true;
	}
	void close(void) {
		PQfinish(conn);
	}

	string queryInsertCreator(string table, vector<string> fields, vector<string> values) {
		string output = "INSERT INTO ";
		output.append(table);			
		output.append("(");
		
		for(unsigned i = 0; i < fields.size(); i++) {
			if (i > 0)
				output.append(", ");
			output.append(fields.at(i));
		}
		
		output.append(") VALUES (");
					
		for(unsigned i = 0; i < values.size(); i++) {
			if (i > 0)
				output.append(", ");
			output.append(values.at(i));
		}
		
		output.append(");");			
		return output;	
	}
	
	string querySelectCreator(string id, string table, vector<string> fields, vector<string> values) {
		string output = "SELECT ";
		output.append(id);
		output.append(" FROM ");
		output.append(table);
		output.append(" WHERE ");
		
		for(unsigned i = 0; i < fields.size(); i++) {
			if (i > 0)
				output.append(" AND ");
			output.append(fields.at(i));
			output.append(" = ");
			output.append(values.at(i));
		}
		
		output.append(" LIMIT 1;");
		return output;		
	}	
	
	string execCommandInsert(string commandString) {	
		command = commandString.c_str();
		
		res = PQexec(conn, command);

		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
				fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(conn));
				PQclear(res);
				exit_nicely(conn);
				return "-1";
		}

		PQclear(res);
		
		return selectLastVal();			
	}	
	
	string execCommandSelect(string commandString) {
		string returnString = "0";			
		command = commandString.c_str();

		res = PQexec(conn, command);			

		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			fprintf(stderr, "SELECT command failed: %s", PQerrorMessage(conn));
			PQclear(res);
			exit_nicely(conn);
		}
		
		if (PQntuples(res) > 0)
			returnString = PQgetvalue(res, 0, 0);
		else
			returnString = "0";

		PQclear(res);
		
		return returnString;			
	}
	
	string selectLastVal() {
		return execCommandSelect("SELECT LASTVAL();");
	}	
	
	string insert(string _table, string _field, string _value) {			
		vector<string> fieldsVector;
		vector<string> valuesVector;
		
		fieldsVector.push_back(_field);			
		valuesVector.push_back(_value);
	
		string commandInsert = queryInsertCreator(_table, fieldsVector, valuesVector);
		string output = execCommandInsert(commandInsert);
		
		if (output == "-1")
			return "-1";
		
		return selectLastVal();
	}
	
	string select(string _id, string _table, string _field, string _value) {
			vector<string> fieldsVector;
			vector<string> valuesVector;			
			
			fieldsVector.push_back(_field);
			valuesVector.push_back(_value);
				
			string commandSelect = querySelectCreator(_id, _table, fieldsVector, valuesVector);
			
			return execCommandSelect(commandSelect);
	}

};

DbController::DbController() {
// 	conninfo = "dbname=sensors";
	conninfo = "host=localhost port=5432 dbname=studyhelper connect_timeout=10 user=madks password=ma121284";
}
