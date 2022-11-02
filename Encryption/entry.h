#ifndef entry_H
#define entry_H
#include <string>
#include <vector>

using std::string;
using std::vector;

class Entry {
private:
    string organization;
    string email;
    string username;
    string password;
public:
    Entry(string org, string em, string user, string passwd, vector<vector<int>> keys);

    void copy_email(vector<vector<int>> keys);
    void copy_username(vector<vector<int>> keys);
    void copy_password(vector<vector<int>> keys);

    ~Entry();
};


#endif