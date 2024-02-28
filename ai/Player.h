//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_PLAYER_H
#define CSCI_561_HW2_PLAYER_H
#include "BoardHelper.h"

class Player {

public:
    int current_player;

    Player(int player):current_player(player){}
    virtual Move playMove(Reversi &board, double t)=0;
};


#endif //CSCI_561_HW2_PLAYER_H
