//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_MTDFPLAYER_H
#define CSCI_561_HW2_MTDFPLAYER_H
#include "Player.h"
#include "NMTDF.h"
#include "BaseEvaluator.h"

class MTDFPlayer: public Player {
    NMTDF m;
public:
    MTDFPlayer(int p,unique_ptr<BaseEvaluator> E):Player(p),m(p, std::move(E)){
    }
    Move playMove(Reversi board, double t){
        if(t<20)
            return m.iterativeDeepening(board,3);
        else if(t<210)
            return m.iterativeDeepening(board,3 );
        else
            return m.iterativeDeepening(board,3);
    }
};


#endif //CSCI_561_HW2_MTDFPLAYER_H
