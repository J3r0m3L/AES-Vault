#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

// create db if not exists
void createDB() {
	sqlite3* DB;
	const char* dir = "./credentials.db";

	int exit = sqlite3_open(dir, &DB);
	sqlite3_close(DB);
}

// create table if not exists
void createTable() {
	sqlite3* DB;
	string cmd1 = "CREATE TABLE IF NOT EXISTS USERS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"USER		TEXT NOT NULL);";
	const char* dir1 = "./users.db";
	
	try {
		int exit = sqlite3_open(dir1, &DB);
		char* msgError;
		exit = sqlite3_exec(DB, cmd1.c_str(), NULL, 0, &msgError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table" << std::endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
	}

	string cmd2 = "CREATE TABLE IF NOT EXISTS CRED("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"ORG		TEXT NOT NULL,"
		"EMAIL		TEXT NOT NULL,"
		"USER		TEXT NOT NULL,"
		"PASS		TEXT NOT NULL);";
	const char* dir2 = "./credentials.db";

	try {
		int exit = sqlite3_open(dir2, &DB);
		char* msgError;
		exit = sqlite3_exec(DB, cmd2.c_str(), NULL, 0, &msgError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table" << std::endl;
			sqlite3_free(msgError);
		}
		sqlite3_close(DB);
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
	}
}

// grab data
vector<vector<string>> selectData() {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	const char* dir = "./credentials.db";
	
	vector<vector<string>> selectedData;
	for (int i = 0; i < 5; i++)
		selectedData.push_back(vector<string>());

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		sqlite3_prepare(DB, "SELECT * from CRED;", -1, &stmt, NULL);
		sqlite3_step(stmt);
		while (sqlite3_column_text(stmt, 0)) {
			for (int i = 0; i < 5; i++)
				selectedData.at(i).push_back(string((char*)sqlite3_column_text(stmt, i)));
			sqlite3_step(stmt);
		}
		sqlite3_close(DB);
	}
	else {
		std::cerr << "Failed to Open DB" << std::endl;
	}
	
	return selectedData;
}

// grab user
string selectUser(int& ID) {
	sqlite3* DB;
	sqlite3_stmt* stmt;
	const char* dir = "./users.db";

	string selectedUser;
	string cmd = "SELECT * from USERS WHERE ID = " + std::to_string(ID) + ";";

	if (sqlite3_open(dir, &DB) == SQLITE_OK) {
		sqlite3_prepare(DB, cmd.c_str(), -1, &stmt, NULL);
		sqlite3_step(stmt);
		selectedUser = string((char*)sqlite3_column_text(stmt, 1));
	}
	else {
		std::cerr << "Failed to open db" << std::endl;
	}
	sqlite3_close(DB);
	return selectedUser;
}

// add data
void insertData(string org, string email, string user, string pass) {
	sqlite3* DB;
	char* msgError;
	const char* dir = "./credentials.db";

	int exit = sqlite3_open(dir, &DB);
	string cmd = "INSERT INTO CRED (ORG, EMAIL, USER, PASS) VALUES('" + org + "','" + email + "','" + user + "','" + pass + ");";
	exit = sqlite3_exec(DB, cmd.c_str(), NULL, 0, &msgError);
	sqlite3_close(DB);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(msgError);
	}
}

// add user
void insertUser(string user) {
	sqlite3* DB;
	char* msgError;
	const char* dir = "./users.db";

	int exit = sqlite3_open(dir, &DB);
	string cmd = "INSERT INTO CRED (USER) VALUES('" + user + "');";
	exit = sqlite3_exec(DB, cmd.c_str(), NULL, 0, &msgError);
	sqlite3_close(DB);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(msgError);
	}
}

// delete user
void deleteData(int ID) {
	sqlite3* DB;
	char* msgError;
	const char* dir = "./credentials.db";

	int exit = sqlite3_open(dir, &DB);
	string sql = "DELETE FROM CRED WHERE ID =" + std::to_string(ID) + ";";
	sqlite3_exec(DB, sql.c_str(), 0, NULL, NULL);
}