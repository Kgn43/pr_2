#include "makeStructure.h"

struct stat sb;

void makeStructure(nlohmann::json structure){
    if(stat(static_cast<std::string>(structure["name"]).c_str(), &sb) != 0){
        system(("mkdir " + static_cast<std::string>(structure["name"])).c_str());
    }
    std::string path;
    for (nlohmann::json::iterator it = structure["structure"].begin(); it != structure["structure"].end(); ++it) {
        path = static_cast<std::string>(structure["name"]) + "/" + it.key();
        if(stat(path.c_str(), &sb) != 0){
            system(("mkdir " + path).c_str());
        }
        path += "/" + it.key();
        if(stat((path + ".csv").c_str(), &sb) != 0){
            std::string columns = static_cast<std::string>(it.key()) + "_pk;";
            for (const auto &column : it.value() ){
                columns += static_cast<std::string>(column) + ";";
            }
            system(("echo " + static_cast<std::string>("\"") + static_cast<std::string>(columns) + static_cast<std::string>("\\n\"") + " > " + path + ".csv").c_str());
        }
        if(stat((path + "_pk_sequence.txt").c_str(), &sb) != 0){
            system(("echo 1 > " + path + "_pk_sequence.txt").c_str());
        }
        if(stat((path + "_lock.txt").c_str(), &sb) != 0){
            system(("echo 0  > " + path + "_lock.txt").c_str());
        }
    }
}