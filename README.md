# Практика №2
перед сборкой исполняемых файлов нужно записать ip машины в локальной сети в файл ip.h

обязательно проверить наличие **crossJoin.csv** и **strktr.json** в дирректории исполняемых файлов
* сборка клиента: g++ client.cpp -o client 
* сборка сервера: g++ server.cpp makeStructure.cpp arr.cpp insert.cpp common.cpp delete.cpp select.cpp userQuery.cpp -o server 
