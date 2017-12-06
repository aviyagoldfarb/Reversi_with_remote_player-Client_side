//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX2_HUMANENEMYGAMEFLOW_H
#define EX2_HUMANENEMYGAMEFLOW_H

#include "GameFlow.h"
//hellow

/**
 * the class HumanEnemyGameFlow defines HumanEnemyGameFlow object.
 */
class RemoteEnemyGameFlow: public GameFlow {
public:
    /**
     * constructor of HumanEnemyGameFlow object.
     * @param myPlayer
     * @param otherPlayer
     * @param gameLogic
     * @param DisplayGameOnConsole
     */
    RemoteEnemyGameFlow(Player *myPlayer, Player *otherPlayer, AbstractGameLogic *gameLogic, DisplayGame *displayGameOnConsole);
    /**
     * checks if the chosen cell is in the possibleMovesVector.
     * @param possibleMovesVector
     * @param chosenCell
     * @return boolean
     */
    bool chosenCellValidity(vector<Point> possibleMovesVector, Point chosenCell);
    /**
     * responsible for the turn exchanges between the players.
     */
    void setNextTurn();
    /**
     * responsible for the game's running.
     */
    void playTheGame();
    /**
     * destructor of HumanEnemyGameFlow object.
     */
    virtual ~HumanEnemyGameFlow();

};

#endif //EX2_HUMANENEMYGAMEFLOW_H
