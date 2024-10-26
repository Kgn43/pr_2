#ifndef SELECT_H
#define SELECT_H

#include <string>
#include "nlohmann/json.hpp"
#include "arr.h"
#include "common.h"


struct selectComm{
    arr<string> tables;
    arr<string> columns;
    arr<string> condition;

};


void select(const json& structure, arr<string> inputQuery);
string getValueByIndex(json structure, const string& tableName, const arr<string>& columnsName, int index);
arr<int> getPassNum(const json& structure, const arr<arr<arr<string>>>& condition);
arr<int> connectCondition(const arr<arr<arr<int>>> &arr1);
arr<int> addIfNotUnic(arr<int> arr1, arr<int> arr2);
int getIndexFromStr(const string& input);
string findTableName(json structure, const string& name);
bool isItColumn(json structure, const string& name);
selectComm toSelectQuery(arr<string> query);

#endif //SELECT_H
