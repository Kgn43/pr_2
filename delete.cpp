#include "delete.h"


delComm toDelQuery(arr<string> query){
    if (query.size < 3){
        stringstream serr;
        serr << "wrong command: syntax error";
        throw runtime_error(serr.str());
    }
    if (query[1] != "from"){
        stringstream serr;
        serr << "wrong command: there is no keyword \"from\"";
        throw runtime_error(serr.str());
    }
    delComm output;
    output.target = query[2];
    if (query.size > 4) {
        if (query[3] != "where") {
            stringstream serr;
            serr << "wrong command: there is no keyword \"where\", or there more than one table";
            throw runtime_error(serr.str());
        }
        arr<string> tmp;
        for (size_t i = 4; i < query.size; ++i){//записать условие
            tmp.push_back(query[i]);
        }
        output.condition = unsplit(tmp, ' ');
    }
    else {
        output.condition = "";
    }
    return output;
}


bool delCondCheck(const string& conds, const arr<string>& headers, const string& line){
    arr<string> cond = splitToArr(conds, " OR ");
    arr<arr<string>> condition;
    for (size_t i = 0; i < cond.size; ++i){
        condition.push_back(splitToArr(cond[i], " AND "));
    }
    arr<string> splittenLine = splitToArr(line, ';');
    int headerNumber;
    string ColumnName;
    string oper;
    string inputValue;
    arr<string> currentCondition;
    bool isDelForAND = true;
    bool isDelForOR = false;
    for (size_t i = 0; i < condition.size; ++i){
        for (size_t j = 0; j < condition[i].size; ++j){
            currentCondition = splitToArr(condition[i][j], ' ');
            if (currentCondition.size != 3){
                stringstream serr;
                serr << "wrong condition";
                throw runtime_error(serr.str());
            }
            ColumnName = currentCondition[0];
            oper = currentCondition[1];
            inputValue = currentCondition[2];
            headerNumber = headers.find(ColumnName);
            if (headerNumber == -1){
                stringstream serr;
                serr << "wrong column name";
                throw runtime_error(serr.str());
            }
            if (oper == "="){
                if (splittenLine[headerNumber] == inputValue) {
                    isDelForAND = isDelForAND && true;
                }
                else {
                    isDelForAND = false;
                }
            }
            else if (oper == "!="){
                if (splittenLine[headerNumber] != inputValue) {
                    isDelForAND = isDelForAND && true;
                }
                else {
                    isDelForAND = false;
                }
            }
            else {
                stringstream serr;
                serr << "wrong operator";
                throw runtime_error(serr.str());
            }
        }
        isDelForOR = isDelForAND || isDelForOR;
    }
    return isDelForOR;
}


void del(const json& structure, arr<string> inputQuery){
    delComm query = toDelQuery(inputQuery);//получаем имя таблицы и условия для удаления
    tableCheck(query.target, structure);//существует ли таблица?
    string path = static_cast<string>(structure["name"]) + "/" + query.target; //директория таблицы
    string wayToTable;
    fstream stream; // поток в файл
    int currentPk = getCurrPk(path + "/" + query.target); //текущий Pk
    lock(path + "/" + query.target); //блокируем доступ
    if (query.condition == ""){ //нет условия для удаления - удалить всю таблицу
        for(int i = 0; i <= currentPk / static_cast<int>(structure["tuples_limit"]); ++i){//для всех файлов таблиц
            if (i != 0){//в каком мы файле?
                wayToTable = "/" + query.target + "_" + to_string(i) + ".csv";
                stream.open(path + wayToTable, std::ios::out); //открываем файл для записи == стираем данные
                stream.close();
            }
            else {
                wayToTable = "/" + query.target + ".csv";
                string headers; //нужно сохранить названия колонок
                stream.open(path + wayToTable, std::ios::in);  // открываем файл для считывания заголовков
                stream >> headers;
                stream.close();
                stream.open(path + wayToTable, std::ios::out); //открываем файл для записи заголовков
                stream << headers << endl;
                stream.close();
            }
        }
        ofstream pkStream (path + "/" + query.target  + "_pk_sequence.txt"); //обнуляем pk
        pkStream << 1;
        pkStream.close();
        unlock(path + "/" + query.target); //разблокируем доступ
    }
    else {
        bool isDel;
        fstream tableStream;
        fstream bufferStream;
        string gottenLine;
        arr<string> headers = getHeaders(path + "/" + query.target + ".csv");
        for(int i = 0; i <= currentPk / static_cast<int>(structure["tuples_limit"]); ++i){//для всех файлов таблиц
            bufferStream.open("buffer.csv", ios::out);
            if (i != 0){//в каком мы файле?
                wayToTable = "/" + query.target + "_" + to_string(i) + ".csv";
            }
            else {
                wayToTable = "/" + query.target + ".csv";
            }
            tableStream.open(path + wayToTable, ios::in);
            while (getline(tableStream, gottenLine)){
                if (gottenLine == "" || gottenLine == " "){
                    bufferStream << endl;
                    continue;
                }
                try {
                    isDel = delCondCheck(query.condition, headers, gottenLine);
                } catch (exception& ex) {
                    unlock(path + "/" + query.target); //разблокируем доступ
                    throw runtime_error(ex.what());
                }
                if (isDel){
                    bufferStream << endl;
                }
                else {
                    bufferStream << gottenLine << endl;
                }
            }
            tableStream.close();
            bufferStream.close();
            tableStream.open(path + wayToTable, ios::out);
            bufferStream.open("buffer.csv", ios::in);
            while (getline(bufferStream, gottenLine)){
                tableStream << gottenLine << endl;
            }
        }
    }
    unlock(path + "/" + query.target); //разблокируем доступ
}
