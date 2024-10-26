#include "makeStructure.h"
#include "userQuery.h"


int main() {
    string strcrt = "strktr.json";
    json structureJSON;
    try{
        ifstream jsonFile (strcrt);
        if (!jsonFile.is_open()) throw runtime_error("The structure file does not exist");
        else {
            structureJSON = json::parse(jsonFile);
            jsonFile.close();
            makeStructure(structureJSON);
        }
    }
    catch(exception& ex) {
        cout << ex.what() << endl;
        return -1;
    }
    string input;
    bool isExit = false;
    while (!isExit){
        cout << "Enter command" << endl;
        cout << endl;
        getline(cin, input);
        if (input == "EXIT" || input == "exit" || input == "Exit") {
            isExit = true;
            continue;
        }
        else {
            userQuery(structureJSON, input);
        }
    }
    return 0;
}
