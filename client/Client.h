//
// Created by udi on 04/12/17.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "string.h"

class Client {
public:
    Client(const char *serverIP, int serverPort);
    int connectToServer();
    string sendCell(string newCell);
private:
    const char *serverIP;
    int serverPort;
    int clientSocket;
};


#endif //EX4_CLIENT_H
