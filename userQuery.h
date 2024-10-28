#ifndef USER_QUERY_H
#define USER_QUERY_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "common.h"
#include "arr.h"
#include "insert.h"
#include "delete.h"
#include "select.h"

using namespace std;
using json = nlohmann::json;


string userQuery(const string& userQuery, const json& structureJSON);

enum commands{
    Insert,
    Del,
    Select,
    Wrong
};

commands commandRequest(const std::string &input);

#endif //USER_QUERY_H
