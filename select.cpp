#include "select.h"


selectComm toSelectQuery(arr<string> query){
    if (query.size <= 3){
        stringstream serr;
        serr << "wrong command: syntax error";
        throw runtime_error(serr.str());
    }
    if (query[2] == "from"){
        stringstream serr;
        serr << "wrong command: there is nothing to select";
        throw runtime_error(serr.str());
    }
    size_t fromPlc = 0;
    for (size_t i = 0; i < query.size; ++i){
        if (query[i] == "from") fromPlc = i;
    }
    if (fromPlc == 0){
        stringstream serr;
        serr << "wrong command: there is no keyword\"from\'";
        throw runtime_error(serr.str());
    }
    if (query[fromPlc + 1] == "where"){
        stringstream serr;
        serr << "wrong command: there is no tables";
        throw runtime_error(serr.str());
    }
    size_t wherePlc = query.size;
    for (size_t i = 0; i < query.size; ++i){
        if (query[i] == "where") wherePlc = i;
    }
    selectComm output;
    for (size_t i = 1; i < fromPlc; ++i){//записать условие
        output.columns.push_back(query[i]);
    }
    for (size_t i = fromPlc + 1; i < wherePlc; ++i){//записать условие
        output.tables.push_back(query[i]);
    }
    if (wherePlc == query.size){
        output.condition = {};
    }
    else {
        for (size_t i = wherePlc + 1; i < query.size; ++i){//записать условие
            output.condition.push_back(query[i]);
        }
    }
    return output;
}


bool isItColumn(json structure, const string& name) {
    for (json::iterator it = structure["structure"].begin(); it != structure["structure"].end(); ++it) {
        for (const auto & colName : it.value()){
            if (colName == name) return true;
        }
    }
    return false;
}


string findTableName(json structure, const string& name){
    for (json::iterator it = structure["structure"].begin(); it != structure["structure"].end(); ++it) {
        for (const auto & colName : it.value()){
            if (colName == name) return it.key();
        }
    }
    return "";
}


bool isItNum(const string& input){
    for (auto ch : input){
        if (ch > '9' || ch < '0') return false;
    }
    return true;
}


int getIndexFromStr(const string& input){
    string number;
    for (const auto& ch : input){
        if (ch == ';'){
            break;
        }
        else {
            number += ch;
        }
    }
    if (!isItNum(number)) return 0;
    return stoi(number);
}


arr<int> addIfNotUnic(arr<int> arr1, arr<int> arr2){
    arr<int> output;
    for (size_t i = 0; i < arr1.size; ++i){
        for (size_t j = 0; j < arr2.size; ++j){
            if (arr1[i] == arr2[j]){
                output.push_back(arr1[i]);
                break;
            }
        }
    }
    return output;
}



arr<int> unicAppend(arr<int> inArr1, arr<int> inArr2) {
    bool isAppend;
    arr<int> outArr;
    if (inArr1.size != 0){
        outArr.push_back(inArr1[0]);
    }
    else if (inArr2.size != 0){
        outArr.push_back(inArr2[0]);
    }
    else {
        return outArr;
    }
    for (size_t i = 0; i < inArr1.size; i++){
        isAppend = true;
        for (size_t j = 0; j < outArr.size; j++){
            if (outArr[j] == inArr1[i]) isAppend = false;
        }
        if (isAppend){
            outArr.push_back(inArr1[i]);
        }
    }
    for (size_t i = 0; i < inArr2.size; i++){
        isAppend = true;
        for (size_t j = 0; j < outArr.size; j++){
            if (outArr[j] == inArr2[i]) isAppend = false;
        }
        if (isAppend){
            outArr.push_back(inArr2[i]);
        }
    }
    return outArr;
}


arr<int> connectCondition(const arr<arr<arr<int>>> &arr1){
    arr<int> outputArr;
    arr<int> andArr;
    for (size_t i = 0; i < arr1.size; i++){
        for (size_t j = 0; j < arr1[i].size; j++){
            if (j == 0){
                andArr = arr1[i][j];
            }
            else{
                andArr = addIfNotUnic(andArr, arr1[i][j]);
            }
        }
        outputArr = unicAppend(outputArr, andArr);
        outputArr.sort();
    }
    return outputArr;
}


void createIndexes(const arr<arr<arr<string>>>& condition, arr<arr<arr<int>>>& indexes) {
    // Убедимся, что массивы идентичных размеров
    size_t size1 = condition.size;
    indexes.resize(size1);
    for (size_t i = 0; i < size1; ++i) {
        size_t size2 = condition[i].size;
        indexes[i].resize(size2);
    
    }
}


arr<int> getPassNum(const json& structure, const arr<arr<arr<string>>>& condition){
    arr<arr<arr<int>>> indexes(condition.size);
    createIndexes(condition, indexes);
    string firstOperand;
    string secondOperand;
    string oper;
    ifstream firstStream;
    ifstream secondStream;
    string table1Name;
    string table2Name;
    string wayToTable;
    int firstCurrentPk;
    int secondCurrentPk;
    string firstPath;
    string secondPath;
    string firstGottenLine;
    string secondGottenLine;
    arr<string> firstSplitedLine;
    arr<string> secondSplitedLine;
    arr<string> firstTableHeaders;
    arr<string> secondTableHeaders;
    int firstHeaderNumber;
    int secondHeaderNumber;
    for (size_t i = 0; i < condition.size; ++i){
        for (size_t j = 0; j < condition[i].size; ++j){
            if (condition[i][j].size != 3){
                stringstream serr;
                serr << "wrong condition";
                throw runtime_error(serr.str());
            }
            firstOperand = condition[i][j][0];
            oper = condition[i][j][1];
            secondOperand = condition[i][j][2];
            if (isItColumn(structure, secondOperand)){
                if (!isItColumn(structure, firstOperand)){
                    stringstream serr;
                    serr << "wrong table name";
                    throw runtime_error(serr.str());
                }
                else {
                    table1Name = findTableName(structure, firstOperand);//находим имя таблицы
                    tableCheck(table1Name, structure);//проверяем на случай отсутствия
                    table2Name = findTableName(structure, secondOperand);//находим имя таблицы
                    tableCheck(table2Name, structure);//проверяем на случай отсутствия
                    firstPath = static_cast<string>(structure["name"]) + "/" + table1Name + "/" + table1Name;// уже не директории!!
                    secondPath = static_cast<string>(structure["name"]) + "/" + table2Name + "/" + table2Name;
                    firstCurrentPk = getCurrPk(firstPath); //текущий Pk1
                    secondCurrentPk = getCurrPk(secondPath); //текущий Pk2
                    //для всех файлов таблиц
                    for (int k = 0; k <= firstCurrentPk / static_cast<int>(structure["tuples_limit"]) && k <= secondCurrentPk / static_cast<int>(structure["tuples_limit"]); ++k){
                        if (k != 0){//в каком мы файле?
                            wayToTable = "_" + to_string(i) + ".csv";
                        }
                        else {
                            wayToTable = ".csv";
                        }
                        //открываем потоки чтения файлов таблиц
                        firstStream.open(firstPath + wayToTable);
                        secondStream.open(secondPath + wayToTable);
                        //разбиваем заголовки на массив
                        firstTableHeaders = getHeaders(firstPath + wayToTable);
                        secondTableHeaders = getHeaders(secondPath + wayToTable);
                        //находим в этих заголовках индекс колонки из проверяемого условия
                        firstHeaderNumber = firstTableHeaders.find(firstOperand);
                        secondHeaderNumber = secondTableHeaders.find(secondOperand);
                        //считываем строки пока можем
                        while (getline(firstStream, firstGottenLine) && getline(secondStream, secondGottenLine)){
                            if (firstGottenLine == "" || firstGottenLine == " ") continue;
                            if (secondGottenLine == "" || secondGottenLine == " ") continue;
                            if (oper == "="){
                                //разбиваем строку для обращения по индексу колонки
                                firstSplitedLine = splitToArr(firstGottenLine, ';');
                                secondSplitedLine = splitToArr(secondGottenLine, ';');
                                cout << firstSplitedLine << " " << firstHeaderNumber << endl << secondSplitedLine << secondHeaderNumber << endl;
                                //проверка ОДНОГО условия
                                if (firstSplitedLine[firstHeaderNumber] == secondSplitedLine[secondHeaderNumber]) {
                                    indexes[i][j].push_back(getIndexFromStr(firstGottenLine));
                                }
                            }
                            else {
                                stringstream serr;
                                serr << "wrong operator";
                                throw runtime_error(serr.str());
                            }
                        }
                        firstStream.close();
                        secondStream.close();
                    }
                }
            }
            else {
                table1Name = findTableName(structure, firstOperand);//находим имя таблицы
                tableCheck(table1Name, structure);//проверяем на случай отсутствия
                firstPath = static_cast<string>(structure["name"]) + "/" + table1Name + "/" + table1Name;// уже не директория!!
                firstCurrentPk = getCurrPk(firstPath); //текущий Pk1
                //для всех файлов таблиц
                for (int k = 0; k <= firstCurrentPk / static_cast<int>(structure["tuples_limit"]); ++k){
                    if (k != 0){//в каком мы файле?
                        wayToTable = "_" + to_string(i) + ".csv";
                    }
                    else {
                        wayToTable = ".csv";
                    }
                    //открываем потоки чтения файлов таблиц
                    firstStream.open(firstPath + wayToTable);
                    //разбиваем заголовки на массив
                    firstTableHeaders = getHeaders(firstPath + wayToTable);
                    //находим в этих заголовках индекс колонки из проверяемого условия
                    firstHeaderNumber = firstTableHeaders.find(firstOperand);
                    //считываем строки пока можем
                    while (getline(firstStream, firstGottenLine)){
                        if (firstGottenLine == "" || firstGottenLine == " ") continue;
                        if (oper == "="){
                            //разбиваем строку для обращения по индексу колонки
                            firstSplitedLine = splitToArr(firstGottenLine, ';');
                            //проверка ОДНОГО условия
                            if (firstSplitedLine[firstHeaderNumber] == secondOperand) {
                                indexes[i][j].push_back(getIndexFromStr(firstGottenLine));
                            }
                        }
                        else {
                            stringstream serr;
                            serr << "wrong operator";
                            throw runtime_error(serr.str());
                        }
                    }
                    firstStream.close();
                }
            }
        }
    }
    arr<int> pass = connectCondition(indexes);
    cout << pass << endl;
    return pass;
}


string getValueByIndex(json structure, const string& tableName, const arr<string>& columnsName, int index){
    string path = static_cast<string>(structure["name"]) + "/" + tableName + "/" + tableName;
    arr<string> headers = getHeaders(path + ".csv");
    int ind;
    for (size_t i = 0; i < columnsName.size; ++i){
        ind = headers.find(columnsName[i]);
        if (ind != -1) break;
    }
    if (ind == -1) {
        stringstream serr;
        serr << "there is no such column name in any tabulations";
        throw runtime_error(serr.str());
    }
    if (index >= 1000){//в каком мы файле?
        path += "_" + to_string((index/1000)) + ".csv";
    }
    else {
        path += ".csv";
    }
    ifstream stream;
    string gottenLine;
    stream.open(path);
    arr<string> splitedLine;
    while (getline(stream, gottenLine)){
        if (gottenLine == "" || gottenLine == " ") continue;
        if (getIndexFromStr(gottenLine) == index){
            splitedLine = splitToArr(gottenLine, ';');
            return splitedLine[ind];
        }
    }
    return "";
}


void select(const json& structure, arr<string> inputQuery){
    selectComm query = toSelectQuery(inputQuery);//получаем имена таблиц, колонки и условия для выборки
    for (size_t i = 0; i < query.tables.size; ++i){//для всех таблиц проверяем их существование
        tableCheck(query.tables[i], structure);
    }
    if (query.condition.size != 0){
        arr<string> cond; //младший сын
        cond = splitToArr(unsplit(query.condition), " OR ");
        arr<arr<string>> condit; //средний сын
        for (size_t i = 0; i < cond.size; ++i){
            condit.push_back(splitToArr(cond[i], " AND "));
        }
        arr<arr<arr<string>>> condition(condit.size); //старший сын
        condition.size = condit.size;
        for (size_t i = 0; i < condit.size; ++i){
            for (size_t j = 0; j < condit[i].size; ++j){
                condition[i].push_back(splitToArr(condit[i][j], ' '));
            }
        }
        arr<int> nums;
        for (size_t i = 0; i < query.tables.size; ++i){
            lock(static_cast<string>(structure["name"]) + "/" + query.tables[i] + "/" + query.tables[i]);
        }
        try {
            nums = getPassNum(structure, condition);
        } catch (exception& ex) {
            for (size_t i = 0; i < query.tables.size; ++i){
                unlock(static_cast<string>(structure["name"]) + "/" + query.tables[i] + "/" + query.tables[i]);
            }
            throw runtime_error(ex.what());
        }
        string firstWord;
        string secondWord;
        ofstream crossJoin("crossJoin.csv");
        for (size_t i = 0; i < nums.size; ++i){
            firstWord = getValueByIndex(structure, query.tables[0], query.columns, nums[i]);
            if (firstWord == "" ||firstWord == " ") continue;
            for (size_t j = 0; j < nums.size; ++j){
                secondWord = getValueByIndex(structure, query.tables[1], query.columns, nums[j]);
                if (secondWord == "" || secondWord == " ") continue;
                crossJoin << firstWord << ';' << secondWord << endl;
            }
        }
        for (size_t i = 0; i < query.tables.size; ++i){
            unlock(static_cast<string>(structure["name"]) + "/" + query.tables[i] + "/" + query.tables[i]);
        }
        crossJoin.close();
    }
    else{
        string firstWord;
        string secondWord;
        ofstream crossJoin("crossJoin.csv");
        string path1 = static_cast<string>(structure["name"]) + "/" + query.tables[0] + "/" + query.tables[0];
        string path2 = static_cast<string>(structure["name"]) + "/" + query.tables[1] + "/" + query.tables[1];
        int currPk1 = getCurrPk(path1);
        int currPk2 = getCurrPk(path2);
        for (size_t i = 1; i < currPk1; ++i){
            firstWord = getValueByIndex(structure, query.tables[0], query.columns, i);
            if (firstWord == "" ||firstWord == " ") continue;
            for (size_t j = 1; j < currPk2; ++j){
                secondWord = getValueByIndex(structure, query.tables[1], query.columns, j);
                if (secondWord == ""||firstWord == " ")  continue;
                crossJoin << firstWord << ';' << secondWord << endl;
            }
        }
        crossJoin.close();
    }
}
