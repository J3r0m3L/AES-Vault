#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "entry.h"
#include "encryption.h"

using std::cout;
using std::endl;

int main() {
    // Read Database and Convert Values into Entries in a Vector
    std::ifstream inputFile;
    inputFile.open("./database.csv");
    
    string line = "";
    while (getline(inputFile, line)) {
        string studentId;
        string lastName;
        string firstName;
        int age;
        string phone;
        double gpa;
        string tempString;
        
        std::stringstream inputString(line);
        
        getline(inputString, studentId, ',');
        getline(inputString, lastName, ',');
        getline(inputString, firstName, ',');
        
        // getting the age
        getline(inputString, tempString, ',');
        age = atoi(tempString.c_str());
        
        getline(inputString, phone, ',');
        
        // getting the gpa
        tempString = "";
        getline(inputString, tempString);
        gpa = atof(tempString.c_str());
        
        line = "";
    }
    
    
    // Key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));
    
    // Generate Entry
    Entry google {1, "Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123", keys, false};
    google.copy_email(keys);
    google.copy_username(keys);
    google.copy_password(keys);
    
    // Add Entry to Text File and to Entries Vector
    
    
    
    // Delete Entry from Text File and Entries Vector
    
    return 0;
}