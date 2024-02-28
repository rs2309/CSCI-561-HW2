//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_MINMAXPLAYER_H
#define CSCI_561_HW2_MINMAXPLAYER_H
#include "Player.h"
#include "Minimax.h"

class MinMaxPlayer :public Player{
    Minimax m;
public:
    MinMaxPlayer(int p):Player(p),m(p){
    }
    Move playMove(Reversi & board, double t){
//        cout<<"T"
        if(t<20)
            return m.Solve(board,4);
        else if(t<210)
            return m.Solve(board,4 );
        else
            return m.Solve(board,5);
    }
};


#endif //CSCI_561_HW2_MINMAXPLAYER_H
