//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "Board.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "DisplayGameOnConsole.h"
#include "HumanEnemyGameFlow.h"
#include "AIEnemyGameFlow.h"
#include "RemotePlayer.h"
#include "RemoteEnemyGameFlow.h"
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>



void createRemoteEnemyGameFlow(Player **blackPlayer, Player **whitePlayer, AbstractGameLogic *gameLogic, DisplayGame *displayGameOnConsole, GameFlow **gameFlow) {
    RemotePlayer *client = new RemotePlayer(EMPTY, "127.0.0.1", 8000);
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
            throw "Error- failed on reading number of player from socket";
        }
    } catch(const char *msg) {
        cout << "Reason: " << msg << endl;
    }

    if (myNumberColor == 1) {
        myColor = "X";
        enemyColor = "O";
        cout << "You are X." << endl;
        *blackPlayer = new HumanPlayer(BLACK);
        client->setPlayerSign(WHITE);
        *whitePlayer = client;
        *gameFlow = new RemoteEnemyGameFlow(*blackPlayer, *whitePlayer, gameLogic, displayGameOnConsole);
        return;
    }
    if (myNumberColor == 2) {
        myColor = "O";
        enemyColor = "X";
        cout << "You are O." << endl;
        *whitePlayer = new HumanPlayer(WHITE);
        client->setPlayerSign(BLACK);
        *blackPlayer = client;
        *gameFlow = new RemoteEnemyGameFlow(*whitePlayer, *blackPlayer, gameLogic, displayGameOnConsole);
        return;
    }
}


void gameMenu(Player **blackPlayer, Player **whitePlayer, AbstractGameLogic *gameLogic, DisplayGame *displayGameOnConsole, GameFlow **gameFlow){
    char playerInput;
    cout << "Please choose your enemy:" << endl;
    cout << "1. Human Player (press H)" << endl;
    cout << "2. AI player (press A)" << endl;
    cout << "3. Remote player (press R)" << endl;
    cin >> playerInput;
    switch (playerInput) {
        case 'H':
        case 'h':
            {
                //the player choose human player as enemy
                *blackPlayer = new HumanPlayer(BLACK);
                *whitePlayer = new HumanPlayer(WHITE);
                *gameFlow = new HumanEnemyGameFlow(*blackPlayer, *whitePlayer, gameLogic, displayGameOnConsole);
                break;
            }
        case 'A':
        case 'a':
            {
                //the player choose AI player as enemy
                *blackPlayer = new HumanPlayer(BLACK);
                *whitePlayer = new AIPlayer(WHITE);
                *gameFlow = new AIEnemyGameFlow(*blackPlayer, *whitePlayer, gameLogic, displayGameOnConsole);
                break;
            }
        case 'R':
        case 'r':
            {
                //the player choose a remote player as enemy
                createRemoteEnemyGameFlow(blackPlayer, whitePlayer, gameLogic, displayGameOnConsole, gameFlow);
                break;
            }
    }
}

int main() {

    //creating an instance of Board object
    Board *board = new Board();
   // Player *blackPlayer = new HumanPlayer(BLACK);
    Player *blackPlayer;
    Player *whitePlayer;
    AbstractGameLogic *gameLogic = new GameLogic(board);
    DisplayGame *displayGameOnConsole = new DisplayGameOnConsole(board);
    GameFlow *gameFlow;

    gameMenu(&blackPlayer, &whitePlayer, gameLogic, displayGameOnConsole, &gameFlow);

    gameFlow->playTheGame();

    //delete created objects

    /*we put 'delete board' in comment because in the current implementation the board is
    deleted within the 'GameLogic' destructor*/
    //delete board;
    delete blackPlayer;
    delete whitePlayer;
    //deletes the board as well
    delete gameLogic;
    delete displayGameOnConsole;
    delete gameFlow;
    return 0;
}


