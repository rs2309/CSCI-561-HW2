//
// Created by Rutvik Shah on 2/28/24.
//
#include "BoardHelper.h"
#include "Player.h"

#ifndef CSCI_561_HW2_CONSOLEPLAYER_H
#define CSCI_561_HW2_CONSOLEPLAYER_H


class ConsolePlayer:public Player {
public:
    ConsolePlayer(int p): Player(p){
    }
    Move playMove(Reversi board,double t) override{
        string opMove;
        cin>>opMove;
        Move m{stoi(opMove.substr(1,2))-1,(int)opMove[0]-97};
        return m;
    }
};


#endif //CSCI_561_HW2_CONSOLEPLAYER_H
