//
// Created by udi on 04/12/17.
//

#ifndef EX4_REMOTEPLAYER_H
#define EX4_REMOTEPLAYER_H

#include "Player.h"

#include "Point.h"

/*
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
*/
class RemotePlayer: public Player{
public:
    /**
     * constructor of RemotePlayer object.
     * @param playerSign
     * @param serverIP
     * @param serverPort
     */
    RemotePlayer(Sign playerSign, const char *serverIP, int serverPort);

    /**
     * copy constructor.
     * @param otherPlayer
     */
    RemotePlayer(RemotePlayer *otherPlayer);
    /**
     * get the sign of the player(BLACK or WHITE).
     * @return sign of the player.
     */
    Sign getPlayerSign() const;
    /**
     * set the sign of the player(BLACK or WHITE).
     * @param playerSign.
     */
    void setPlayerSign(Sign playerSign);
    /**
     * get the score of the player.
     * @return playerScore.
     */
    int getPlayerScore() const;
    /**
     * set the score of the player.
     * @param addToPlayerScore number to add to the current score.
     */
    void setPlayerScore(int addToPlayerScore);
    /**
     * destructor of HumanPlayer object.
     */
    virtual ~RemotePlayer();

    int connectToServer();
    void sendCell(int x, int y);
    Point receiveCell();
private:
    const char *serverIP;
    int serverPort;
    int clientSocket;
};


#endif //EX4_REMOTEPLAYER_H
