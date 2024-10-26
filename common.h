#ifndef COMMON_H
#define COMMON_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "arr.h"


using namespace std;
using json = nlohmann::json;

void lock(const string &name);
void unlock(const string &name);
int getCurrPk(const string &path);
void tableCheck(const string& tableName, const json& structure);
arr<string> getHeaders(const string& name);

#endif //COMMON_H
