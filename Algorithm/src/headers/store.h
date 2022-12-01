#pragma once
#define store_H
#include <vector>
#include <string>

using std::vector;
using std::string;

// create DB if does not exist
void createDB();

// create table if does not exist
void createTable();

// select all entries of credentials table
vector<vector<string>> selectData();

// select an entry of users table
string selectUser(int& ID);

// insert data into credentials table
void insertData(string org, string email, string user, string pass);

// insert user by ID into users table
void insertUser(string user);

// delete row by ID of credentials table
void deleteData(int ID);

