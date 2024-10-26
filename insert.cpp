#include "insert.h"


void sizeCheck(int insertionSize, const string& tableName, const json& structure){
    int tableStrLen = static_cast<int>(structure["structure"][tableName].size());
    if (tableStrLen < insertionSize){// < из-за Pk
        stringstream serr;
        serr << "wrong command to many values";
        throw runtime_error(serr.str());
    }
}


insComm toInsQuery(arr<string> query){
    if (query.data[1] != "into"){
        stringstream serr;
        serr << "wrong command: there is no \"into\"";
        throw runtime_error(serr.str());
    }
    size_t valuesPlc = 0;
    for (size_t i = 0; i < query.size; ++i){
        if (query[i] == "values") valuesPlc = i;
    }
    if (valuesPlc == 0){
        stringstream serr;
        serr << "wrong command: there is no \"values\"";
        throw runtime_error(serr.str());
    }
    if (valuesPlc + 1 == query.size){//если после values ничего нет
        stringstream serr;
        serr << "wrong command: nothing to insert";
        throw runtime_error(serr.str());
    }
    insComm output;
    for (size_t i = 2; i < query.size; ++i){
        if (i < valuesPlc) output.target.push_back(query[i]);
        else if (i > valuesPlc) output.data.push_back(query[i]);
    }
    return output;
}


void insert(const json& structure, arr<string> inputQuery){
    insComm query = toInsQuery(inputQuery); //проверяем правильность команды по синтаксису, отделяем таблицы от значений
    for (size_t i = 0; i < query.target.size; ++i){//вставка во все таблицы
        tableCheck(query.target[i], structure);//существует ли таблица?
        sizeCheck(static_cast<int>(query.data.size), query.target[i], structure); //не слишком ли много колонок для этой таблицы?
        int tableStrLen = static_cast<int>(structure["structure"][query.target[i]].size());
        string values = unsplit(query.data, ';');
        string path = static_cast<string>(structure["name"]) + "/" + query.target[i]; //директория таблицы
        int currentPk = getCurrPk(path + "/" + query.target[i]); //текущий Pk
        values = to_string(currentPk) + ";" + values;//добавили pk к вводу
        if (tableStrLen >= query.data.size){
            while (tableStrLen != query.data.size){
                tableStrLen--;
                values += ";";
            }
        }
        string wayToTable;
        if (currentPk >= 1000){//в какой файл записать?
            wayToTable = "/" + query.target[i] + "_" + to_string(currentPk % 1000) + ".csv";
        }
        else {
            wayToTable = "/" + query.target[i] + ".csv";
        }
        if (currentPk % 1000 == 0){
            system(("touch " + path + wayToTable).c_str());
        }
        lock(path + "/" + query.target[i]); //блокируем доступ
        ofstream output; // поток в файл
        output.open(path + wayToTable, std::ios::app);  // открываем файл для записи в конец
        output << values << endl; // сама запись
        output.close();
        output.open(path + "/" + query.target[i] + "_pk_sequence.txt");
        output << ++currentPk;
        output.close();
        unlock(path + "/" + query.target[i]);
    }
}