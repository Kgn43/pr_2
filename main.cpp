#include <winsock.h>
#include <sys/socket.h>

#include "makeStructure.h"
#include "userQuery.h"


#define PORT 7432

int main() {

    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);


    //same shit
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ServSock == INVALID_SOCKET) {
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        cout << "Server socket initialization is OK" << endl;


    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("In sockets");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);












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
