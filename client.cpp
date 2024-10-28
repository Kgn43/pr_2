#include "client.h"

using std::cerr, std::endl, std::cout, std::cin, std::string;


int main() {
    int socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        cerr << "Socket creation error" << endl;
        return -1;
    }
    struct sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(7432);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
    }
    if (connect(socket, reinterpret_cast<sockaddr *>(&server), sizeof(struct sockaddr_in)) < 0) {
        cerr << "Connection Failed" << endl;
    }
    cout << "Client connected" << endl;
    string buffer;
    bool isDisconnect = false;
    while (!isDisconnect) {
        cout << "Enter string" << endl;
        getline(cin, buffer);
        if (buffer == "disconnect") {
            isDisconnect = true;
            send(socket, buffer.c_str(), buffer.size(), 0);
            continue;
        }
        send(socket, buffer.c_str(), buffer.size(), 0);
        auto nrecv = recv(socket, &buffer, buffer.size(), 0);
        if (nrecv == -1) {
            cerr << "read failed" << endl;
            continue;
        }
        if (nrecv == 0) {
            cerr << "EOF occured" << endl;
        }

    }
    return 0;
}

