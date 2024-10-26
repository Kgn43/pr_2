#ifndef USER_QUERY_H
#define USER_QUERY_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "arr.h"
#include "common.h"
#include "insert.h"
#include "delete.h"
#include "select.h"

using namespace std;
using json = nlohmann::json;


string userQuery(const json& structureJSON ,const std::string& input);

enum commands{
    Insert,
    Del,
    Select,
    Wrong
};


commands commandRequest(const std::string &input){
    if(input == "insert") return commands::Insert;
    if (input == "delete") return commands::Del;
    if (input == "select") return commands::Select;
    return commands::Wrong;
}

#endif //USER_QUERY_H
