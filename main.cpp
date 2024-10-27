#include <sys/socket.h>
#include <mutex>
#include "makeStructure.h"
#include "userQuery.h"


#define PORT 7432

int main() {

    int serverSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error of create socket" << endl;
        return;
    }
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cerr << "Error of setting parameters of socket" << endl;
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Error of binding" << endl;
        return;
    }

    if (listen(serverSocket, 1) < 0) {
        cerr << "Error of socket listening" << endl;
        return;
    }



    cout << "Server started" << endl;



    while (true){
        sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
        if(clientSocket < 0){
            cout << "Error to connect client" << endl;
            continue;
        }

        if(cntThreads <= MAX_CLIENTS){
            char* clientIP = inet_ntoa(clientAddress.sin_addr);
            cout << "Client[" << clientIP << "] was connected" << endl;
            thread(serve_client, clientSocket, schemaName).detach();

            string answer = "Successfully connected to the server";
            send(clientSocket, answer.c_str(), answer.size(), 0);
        }
        else{
            string answer = "A lot of clients now, try it later";
            send(clientSocket, answer.c_str(), answer.size(), 0);
            close(clientSocket);
        }
    }

    close(serverSocket);
}












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
    while (true){
            userQuery(structureJSON);
    }
    return 0;
}
