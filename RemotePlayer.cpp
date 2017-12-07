//
// Created by udi on 04/12/17.
//

#include "RemotePlayer.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

using namespace std;

RemotePlayer::RemotePlayer(Sign playerSign = EMPTY, const char *serverIP, int serverPort):
        Player(playerSign), serverIP(serverIP), serverPort(serverPort), clientSocket(0) {
    cout << "RemotePlayer" << endl;
}

RemotePlayer::RemotePlayer(RemotePlayer *otherPlayer) : Player(otherPlayer){

}

Sign RemotePlayer::getPlayerSign() const {
    return playerSign;
}

void RemotePlayer::setPlayerSign(Sign playerSign) {
    this->playerSign = playerSign;
}

int RemotePlayer::getPlayerScore() const {
    return playerScore;
}

void RemotePlayer::setPlayerScore(int addToPlayerScore) {
    this->playerScore += addToPlayerScore;
}

RemotePlayer::~RemotePlayer() {

}

int RemotePlayer::connectToServer() {
    // Create a socket point
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        throw "Error opening socket";
    }
    // Convert the ip string to a network address
    struct in_addr address;
    if (!inet_aton(serverIP, &address)) {
        throw "Can't parse IP address";
    }
    // Get a hostent structure for the given host address
    struct hostent *server;
    server = gethostbyaddr((const void *)&address, sizeof
            address, AF_INET);
    if (server == NULL) {
        throw "Host is unreachable";
    }
    // Create a structure for the server address
    struct sockaddr_in serverAddress;
    bzero((char *)&address, sizeof(address));
    serverAddress.sin_family = AF_INET;
    memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    // htons converts values between host and network byte orders
    serverAddress.sin_port = htons(serverPort);
    // Establish a connection with the TCP server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error connecting to server";
    }
    cout << "Connected to server" << endl;
    cout << "Waiting for other player to join..." << endl;
    return this->clientSocket;
}

string RemotePlayer::sendCell(string newCell) {
// Write the exercise arguments to the socket
    int n = write(clientSocket, &newCell, sizeof(newCell));
    if (n == -1) {
        throw "Error writing the cell to socket";
    }
    /**n = write(clientSocket, &op, sizeof(op));
    if (n == -1) {
        throw "Error writing op to socket";
    }
    n = write(clientSocket, &arg2, sizeof(arg2));
    if (n == -1) {
        throw "Error writing arg2 to socket";
    }
     */
// Read the result from the server
    string result;
    n = read(clientSocket, &result, sizeof(result));
    if (n == -1) {
        throw "Error reading result from socket";
    }
    return result;
}




