//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "Client.h"
#include "RemoteEnemyGameFlow.h"
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

RemoteEnemyGameFlow::RemoteEnemyGameFlow(Player *myPlayer, Player *otherPlayer, AbstractGameLogic *gameLogic, DisplayGame *displayGameOnConsole) :
        GameFlow(myPlayer, otherPlayer, gameLogic, displayGameOnConsole){}

bool RemoteEnemyGameFlow::chosenCellValidity(vector<Point> possibleMovesVector, Point chosenCell){
    for(int i = 0; i < possibleMovesVector.size(); i++){
        if(possibleMovesVector[i].isEqual(chosenCell)){
            return true;
        }
    }
    return false;
}

void RemoteEnemyGameFlow::setNextTurn(){
    if(this->turn == blackPlayer){
        this->turn = whitePlayer;
        this->nextTurn = blackPlayer;
    }
    else{
        this->turn = blackPlayer;
        this->nextTurn = whitePlayer;
    }
}

void RemoteEnemyGameFlow::playTheGame() {
    //initialize the turns

    int clientSocket, n;
    string myColor, enemyColor;
    string myChoice;
    string enemyChoice;

    this->turn = blackPlayer;
    this->nextTurn = whitePlayer;

    //create a vector that will get the return value of possibleMoves function
    vector<Point> possibleMovesVector;
    int x, y;

    cout << "Waiting for other player's move..." << endl;
    try {

        int n = read(clientSocket, &enemyChoice, sizeof(enemyChoice));
        if (n == -1) {
            throw "Error in reading the other player's move from socket";
        }
    } catch (const char *msg) {
        cout << "Reason: " << msg << endl;
    }



        try {
            enemyChoice = client.sendCell(myChoice);
            cout << enemyColor << " played:" << " (" << enemyChoice << ")" << endl;
        } catch (const char *msg) {
            cout << "Failed to send the new cell to server. Reason: " << msg << endl;
        }
        if (findNewCell(enemyChoice)) {
            return 0;
        }


    //running until the end of the game criteria
    do{
        cout << "Current board:" << endl;
        cout << endl;
        //printing the board using printGameBoard function from DisplayGameOnConsole class
        this->displayGameOnConsole->printGameBoard();
        cout << string(1, this->turn->getPlayerSign()) << ":" << " It's your move." << endl;
        //check for the possible moves
        possibleMovesVector = gameLogic->possibleMoves(this->turn, this->nextTurn);
        //check if the vector is empty
        if(possibleMovesVector.size() == 0){
            cout << "No possible moves. Play passes back to other player." << endl;
            cout << endl;
            this->setNextTurn();
            continue;
        }
        cout << "Your possible moves: ";
        //display the optional cells
        for(int i = 0; i < possibleMovesVector.size(); i++){
            possibleMovesVector[i].pointToPrint();
            if(i < possibleMovesVector.size() - 1){
                cout << ",";
            }
        }
        cout << endl;
        cout << endl;
        //loop until the player enters appropriate cell
        do {
            cout << "Please enter your move row col: ";
            cin >> x >> y;
            if(cin.fail()){
                cout << "This is not a number. Try again." << endl;
                cout << endl;
                cin.clear();
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            //create a point from the player's input
            Point chosenCell(x, y);
            //check if the player's input is valid
            if (this->chosenCellValidity(possibleMovesVector, chosenCell)) {
                gameLogic->moveMaker(chosenCell, this->turn, this->nextTurn);
                break;
            } else {
                //keep looping
                cout << "The chosen cell is not valid. Try again." << endl;
                cout << endl;
            }
        }while(1);
        possibleMovesVector.clear();
        this->setNextTurn();
        //end of the game criteria
    }while(!(gameLogic->fullBoard()) && (gameLogic->possibleMoves(this->turn, this->nextTurn).size() != 0 ||
                                         gameLogic->possibleMoves(this->nextTurn, this->turn).size() != 0));

    //printing the board using printGameBoard function from DisplayGameOnConsole class
    this->displayGameOnConsole->printGameBoard();
    cout << endl;
    cout << endl;
    //display the results of the game
    cout << "The winner is: ";
    if(blackPlayer->getPlayerScore() > whitePlayer->getPlayerScore()){
        cout << "The black player with " << blackPlayer->getPlayerScore() << " cells.";
    }
    if(blackPlayer->getPlayerScore() < whitePlayer->getPlayerScore()){
        cout << "The white player with " << whitePlayer->getPlayerScore() << " cells.";
    }
    if(blackPlayer->getPlayerScore() == whitePlayer->getPlayerScore()){
        cout << "No winner, both players are even with " << blackPlayer->getPlayerScore() << " cells each.";
    }
    cout << endl;
}

RemoteEnemyGameFlow::~HumanEnemyGameFlow() {

