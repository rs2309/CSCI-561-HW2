//
// Created by Rutvik Shah on 3/6/24.
//
#include "BoardHelper.h"
#include "Player.h"
#include <random>
#ifndef CSCI_561_HW2_RANDOMPLAYER_H
#define CSCI_561_HW2_RANDOMPLAYER_H


class RandomPlayer: public Player {
public:
    RandomPlayer(int p): Player(p){}
    Move playMove(Reversi& board,double t){
        auto moves=BoardHelper::findNextMoves(board,current_player,false);
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 eng(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(0, moves.size() - 1); // Define the range

        // Select a random move
        int randomIndex = distr(eng); // Generate a random index
        return moves[randomIndex];
    }
};


#endif //CSCI_561_HW2_RANDOMPLAYER_H
