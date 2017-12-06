//
// Created by udi on 04/12/17.
//
#include "Client.h"
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main() {
    Client client("127.0.0.1", 8000);
    int clientSocket, n;
    string myColor, enemyColor;
    string myChoice;
    string enemyChoice;
    try {
        clientSocket = client.connectToServer();
    } catch (const char *msg) {
        cout << "Failed to connect to server. Reason:" << msg << endl;
        exit(-1);
    }
    int myNumberColor;
    try {
         n = read(clientSocket, &myNumberColor, sizeof(myNumberColor));
        if (n == -1) {
            throw "Error reading number of the player from socket";
        }
    } catch(const char *msg) {
        cout << "Reason: " << msg << endl;
    }

    if (myNumberColor == 1) {
        myColor = "X";
        enemyColor = "O";
        cout << "You are X." << endl;
        myChoice = hegf.playTheGame;
    }
    if (myNumberColor == 2) {
        myColor = "O";
        enemyColor = "X";
        cout << "You are O." << endl;
        cout << "Waiting for other player's move..." << endl;
        try {

            n = read(clientSocket, &enemyChoice, sizeof(enemyChoice));
            if (n == -1) {
                throw "Error in reading the other player's move from socket";
            }
        } catch (const char *msg) {
            cout << "Reason: " << msg << endl;
        }
    }
    
    int x, y;
    while (true) {

        try {
            enemyChoice = client.sendCell(myChoice);
            cout << enemyColor << " played:" << " (" << enemyChoice << ")" << endl;
        } catch (const char *msg) {
            cout << "Failed to send the new cell to server. Reason: " << msg << endl;
        }
        if (findNewCell(enemyChoice)) {
            return 0;
        }
    }
}

