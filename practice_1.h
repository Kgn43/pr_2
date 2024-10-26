#ifndef PRACTICE_1_H
#define PRACTICE_1_H

#include "makeStructure.h"
#include "arr.h"
#include "common.h"
#include "insert.h"
#include "delete.h"
#include "select.h"


#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;
using json = nlohmann::json;


enum commands{
    Exit,
    Insert,
    Del,
    Select,
    Wrong
};


commands commandRequest(const std::string &input){
    if (input == "exit") return commands::Exit;
    if(input == "insert") return commands::Insert;
    if (input == "delete") return commands::Del;
    if (input == "select") return commands::Select;
    return commands::Wrong;
}

#endif //PRACTICE_1_H
