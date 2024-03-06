//
// Created by Rutvik Shah on 2/27/24.
//

#ifndef CSCI_561_HW2_BASEEVALUATOR_H
#define CSCI_561_HW2_BASEEVALUATOR_H

#include "BoardHelper.h"

class BaseEvaluator {
public:
    virtual int eval(Reversi board,int player)=0;
};


#endif //CSCI_561_HW2_BASEEVALUATOR_H
