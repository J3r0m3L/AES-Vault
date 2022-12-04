#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

const char* dir = "./credentials.db";

// create db if not exists
void createDB() {
	sqlite3* DB;
	int exit = sqlite3_open(dir, &DB);
	sqlite3_close(DB);
}

// create tables if not exists
void createTables() {
	sqlite3* DB;
	string cmd1 = "CREATE TABLE IF NOT EXISTS USERS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"USER		TEXT NOT NULL);";
	
	string cmd2 = "CREATE TABLE IF NOT EXISTS CRED("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"ORG		TEXT NOT NULL,"
		"EMAIL		TEXT NOT NULL,"
		"USER		TEXT NOT NULL,"
		"PASS		TEXT NOT NULL);";

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		char* msgError = NULL;
		sqlite3_exec(DB, cmd1.c_str(), NULL, 0, &msgError);
		if (msgError != NULL) {
			cout << "Table 1: " << msgError << endl;
			sqlite3_free(msgError);
		}

		sqlite3_exec(DB, cmd2.c_str(), NULL, 0, &msgError);
		if (msgError != NULL) {
			cout << "Table 2: " << msgError << endl;
			sqlite3_free(msgError);
		}

		sqlite3_close(DB);

	} else {
		cout << "Failed to Open DB" << endl;
	}
}

// select all data
vector<vector<string>> selectData() {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	vector<vector<string>> selectedData = { {}, {}, {}, {}, {} };

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		sqlite3_prepare(DB, "SELECT * from CRED;", -1, &stmt, NULL);
		sqlite3_step(stmt);
		while (sqlite3_column_text(stmt, 0)) {
			for (int i = 0; i < 5; i++)
				selectedData.at(i).push_back(string((char*)sqlite3_column_text(stmt, i)));
			sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
	return selectedData;
}

// select last data
vector<string> selectCred() {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	vector<string> selectedData = {"", "", "", "", ""};

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		sqlite3_prepare(DB, "SELECT * FROM CRED WHERE ID = (SELECT MAX(ID) FROM CRED); ", -1, &stmt, NULL);
		sqlite3_step(stmt);
		for (int i = 0; i < 5; i++)
			selectedData.at(i) = string((char*)sqlite3_column_text(stmt, i));
		sqlite3_finalize(stmt);
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
	return selectedData;
}

// select first user
string selectUser() {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	string selectedUser = "";

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		sqlite3_prepare(DB, "SELECT * from USERS;", -1, &stmt, NULL);
		sqlite3_step(stmt);
		if (sqlite3_column_text(stmt, 0))
			selectedUser = string((char*)sqlite3_column_text(stmt, 1));
		sqlite3_finalize(stmt);
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
	
	return selectedUser;
}

// add data
void insertData(string org, string email, string user, string pass) {
	sqlite3* DB;
	char* msgError = NULL;
	
	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		string cmd = "INSERT INTO CRED (ORG, EMAIL, USER, PASS) VALUES('" + org + "','" + email + "','" + user + "','" + pass + "');";
		sqlite3_exec(DB, cmd.c_str(), NULL, 0, &msgError);
		if (msgError != NULL) {
			cout << msgError << endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
}

// add user
void insertUser(string user) {
	sqlite3* DB;
	char* msgError = NULL;

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		string cmd = "INSERT INTO USERS (USER) VALUES('" + user + "');";
		sqlite3_exec(DB, cmd.c_str(), NULL, 0, &msgError);
		if (msgError != NULL) {
			cout << msgError << endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
}

// delete credentials
void deleteData(int ID) {
	sqlite3* DB;
	char* msgError = NULL;

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		string cmd = "DELETE FROM CRED WHERE ID =" + std::to_string(ID) + ";";
		sqlite3_exec(DB, cmd.c_str(), NULL, NULL, &msgError);
		sqlite3_close(DB);
		if (msgError != NULL) {
			cout << msgError << endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
}

// delete all
void deleteAll() {
	sqlite3* DB;
	char* msgError = NULL;

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		string cmd = "DELETE FROM CRED;";
		sqlite3_exec(DB, cmd.c_str(), NULL, NULL, &msgError);
		if (msgError != NULL) {
			cout << "Table 2: " << msgError << endl;
			sqlite3_free(msgError);
		}
		cmd = "DELETE FROM USERS;";
		sqlite3_exec(DB, cmd.c_str(), NULL, NULL, &msgError);
		if (msgError != NULL) {
			cout << "Table 1: " << msgError << endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	else {
		cout << "Failed to Open DB" << endl;
	}
}
