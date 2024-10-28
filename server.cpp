#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>//sleep func
#include <future>
#include <mutex>


#include "makeStructure.h"
#include "userQuery.h"


void requestProcessing(const int clientSocket, const sockaddr_in& clientAddress, const nlohmann::json& structure) {
    mutex userMutex;
    char receive[1024] = {};
    string sending;
    bool isExit = false;
    while (!isExit) {
        lock_guard<mutex> guard(userMutex);
        bzero(receive, 1024);
        const ssize_t userRead = read(clientSocket, receive, 1024);
        if (userRead == -1) {
            sending = "Read error\n";
            send(clientSocket, sending.c_str(), sending.size(), 0);
        }
        if (userRead == 0) {
            cerr << "client[" << clientAddress.sin_addr.s_addr << "] disconnected\n";
            isExit = true;
            continue;
        }
        if (receive == "disconnect") {
            isExit = true;
            continue;
        }
        string result = userQuery(receive ,structure);
        send(clientSocket, result.c_str(), result.size(), 0);
    }
    close(clientSocket);
}


void startServer(const json& structureJSON) {
    const int server = socket(AF_INET, SOCK_STREAM, 0);//file descriptor
    if (server == -1) {
        cerr << "Socket creation error" << endl;
        return;
    }
    sockaddr_in address{}; //IPV4 protocol structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //any = 0.0.0.0
    address.sin_port = htons(7432);//host to net short

    if (bind(server, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        cerr << "Binding error" << endl;
        return;
    }

    if (listen(server, 10) == -1) {
        cerr << "Socket listening error" << endl;
        return;
    }

    cout << "Server started" << endl;

    sockaddr_in clientAddress{};
    socklen_t clientAddrLen = sizeof(clientAddress);
    while (true){
        int clientSocket = accept(server, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddrLen);
        if(clientSocket == -1){
            cout << "No one to connect" << endl;
            sleep(5);
            continue;
        }
        cout << "Client[" << clientAddress.sin_addr.s_addr << "] was connected" << endl;
        future<void> isExecuted = async(requestProcessing, clientSocket, clientAddress, structureJSON);

    }
    close(server);
}


int main() {
    string strcrt = "strktr.json";
    json structureJSON;
    try{
        ifstream jsonFile (strcrt);
        if (!jsonFile.is_open()) throw runtime_error("The structure file does not exist");
        structureJSON = json::parse(jsonFile);
        jsonFile.close();
        makeStructure(structureJSON);
    }
    catch(exception& ex) {
        cout << ex.what() << endl;
        return -1;
    }
    startServer(structureJSON);
    return 0;
}
