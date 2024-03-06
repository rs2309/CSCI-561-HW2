//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_MINMAXPLAYER_H
#define CSCI_561_HW2_MINMAXPLAYER_H
#include "Player.h"
#include "Minimax.h"
#include "BaseEvaluator.h"


double adjustTimeForMove(int moveIndex, int totalMoves, double baseTimePerMove) {
    double timeMultiplier = 1.0; // Default
    int third = totalMoves / 3;

    if (moveIndex <= third) {
        timeMultiplier = 0.5; // Early game
    } else if (moveIndex <= 2 * third) {
        timeMultiplier = 1.0; // Middle game
    } else {
        timeMultiplier = 1.5; // Late game
    }

    return baseTimePerMove * timeMultiplier;
}
class MinMaxPlayer :public Player{
    Minimax m;
//    int depth;
public:
    MinMaxPlayer(int p,unique_ptr<BaseEvaluator> E):Player(p),m(p, std::move(E)){
    }
    Move playMove(Reversi& board, double t){
        int totalCount=BoardHelper::totalDiscCount(board);
        int moveIndex=totalCount/2+1;
        int movesLeft=(N*N-(totalCount))/2;

        double timeForMove= adjustTimeForMove(moveIndex,(N*N)/2,(t-5)/movesLeft);
        return m.Solve(board,timeForMove);

//    }else{
//            if(t<35)
//                return m.Solve(board,3,true);
//            else if(BoardHelper::totalDiscCount(board)<30)
//                return m.Solve(board,4,true);
//            else
//                return m.Solve(board,depth,true);
//        }
//        cout<<"T"
//        if(t<35)
//            return m.Solve(board,3);
//        else if(BoardHelper::totalDiscCount(board)<30)
//            return m.Solve(board,3);
//        else
//            return m.Solve(board,depth);
    }
};


#endif //CSCI_561_HW2_MINMAXPLAYER_H
