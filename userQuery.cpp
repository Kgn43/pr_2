#include "userQuery.h"


commands commandRequest(const std::string &input){
    if(input == "insert") return commands::Insert;
    if (input == "delete") return commands::Del;
    if (input == "select") return commands::Select;
    return commands::Wrong;
}


string userQuery(const json& structureJSON){
    string input;
    cout << "Enter command" << endl;
    getline(cin, input);
    arr<string> query = splitToArr(input);
    if (query.size == 0){
        return "query was not entered";
    }
    commands comm = commandRequest(query[0]);
    try{
        switch (comm) {
            case Insert:
                insert(structureJSON, query);
                break;
            case Del:
                del(structureJSON, query);
                break;
            case Select:
                select(structureJSON, query);
                break;
            case Wrong:
                throw runtime_error("Wrong syntax");
        }
    }
    catch(exception& ex) {
        return ex.what();
    }
    return "Success!";
}
