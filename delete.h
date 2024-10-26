#ifndef DELETE_H
#define DELETE_H

#include <string>
#include "nlohmann/json.hpp"
#include "arr.h"
#include "common.h"

using namespace std;
using json = nlohmann::json;

struct delComm{
    string target;
    string condition;
};


delComm toDelQuery(arr<string> query);
bool delCondCheck(const string& conds, const arr<string>& headers, const string& line);
void del(const json& structure, arr<string> inputQuery);



#endif //DELETE_H
