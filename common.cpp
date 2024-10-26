#include "common.h"


void lock(const string &name){
    int lockerValue;
    ifstream lockStream (name + "_lock.txt");
    lockStream >> lockerValue;
    lockStream.close();
    if (lockerValue == 0){
        FILE *lockFile = fopen((name + "_lock.txt").c_str(), "w");
        fputs("1",lockFile);
        fclose(lockFile);
    }
    else {
        stringstream serr;
        serr << "someone already work with this file";
        throw runtime_error(serr.str());
    }
}


void unlock(const string &name){
    FILE *lockFile = fopen((name + "_lock.txt").c_str(), "w");
    fputs("0",lockFile);
    fclose(lockFile);
}


int getCurrPk(const string &path){
    ifstream fileStream (path + "_pk_sequence.txt");
    int out;
    fileStream >> out;
    fileStream.close();
    return out;
}


void tableCheck(const string& tableName, const json& structure){
    if (structure["structure"].find(tableName) == structure["structure"].end()){
        stringstream serr;
        serr << "wrong tabulation";
        throw runtime_error(serr.str());
    }
}


arr<string> getHeaders(const string& name){
    string headers; //нужно сохранить названия колонок
    ifstream stream;
    stream.open(name, std::ios::in);  // открываем файл для считывания заголовков
    stream >> headers;
    stream.close();
    return splitToArr(headers, ';');
}