//
// Created by Rutvik Shah on 2/14/24.
//

#include "BoardHelper.h"
#include "BaseEvaluator.h"

#ifndef CSCI_561_HW2_EVALUATOR_H
#define CSCI_561_HW2_EVALUATOR_H


class Evaluator:public BaseEvaluator {
public:
    int  diskCountScore(Reversi board,int player) {
        int playerDisks = 0, opponentDisks = 1;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == player) ++playerDisks;
                else if (board[x][y] == 1 - player) ++opponentDisks;
            }
        }
        return 10*((int)(playerDisks - (double)opponentDisks)/(playerDisks+opponentDisks));
    }
    int  safeEdgeCountScore(vector<vector<int>> board,int player) {
        int score=0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == player && BoardHelper::isSafeEdge(x,y)) ++score;
                else if (board[x][y] == 1 - player && BoardHelper::isSafeEdge(x,y)) --score;
            }
        }
        return score*5;
    }
    int  mobilityScore(vector<vector<int>> board,int player) {
        int playerMoves = BoardHelper::findNextMoves(board, player, false).size();
        int opponentMoves = BoardHelper::findNextMoves(board, 1 - player, false).size();
        return  10*(playerMoves - opponentMoves)/(playerMoves + opponentMoves+1);
    }
    int  cornerScore(vector<vector<int>> board,int player) {
        int score = 0;
        int corners[4][2] = {{0, 0}, {0, N - 1}, {N - 1, 0}, {N - 1, N - 1}};
        int playerCorners=0,opponentCorners=0;
        for (auto& corner : corners) {
            if (board[corner[0]][corner[1]] == player) ++playerCorners;
            else if (board[corner[0]][corner[1]] == 1 - player) ++opponentCorners;
        }
        return 10*(playerCorners - opponentCorners )/(playerCorners  + opponentCorners +1); // Weight corners more heavily
    }
//    int static mobilityImpact(vector<vector<int>>& board, int player, const pair<int, int>& move) {
//        // Create a copy of the board to simulate the move
//        vector<vector<int>> tempBoard = board;
//
//        // Simulate the move
//        vector<pair<int, int>> flippedDisks = performMove(tempBoard, move.first, move.second, player);
//
//        // Calculate the number of legal moves for the player after the move
//        int playerMobilityAfterMove = findNextMoves(tempBoard, player).size();
//
//        // Calculate the number of legal moves for the opponent after the move
//        int opponentMobilityAfterMove = findNextMoves(tempBoard, 1 - player).size();
//
//        // Undo the move on the temporary board
////    undoMove(tempBoard, flippedDisks, move.first, move.second, player);
//
//        // The mobility impact favors moves that increase the player's mobility and decrease the opponent's
//        return playerMobilityAfterMove - opponentMobilityAfterMove;
//    }
    int  eval(Reversi board,int player) {
        // Adjust weights as needed based on strategy

        int weightDiskCount = 10;
        int weightMobility = 5*100;
        int weightSafeEdge= 50;
        int weightCorners = 1000*100;

        int score = 0;
        score += weightDiskCount * diskCountScore(board, player);
        score += weightMobility * mobilityScore(board, player);
        score += weightSafeEdge * safeEdgeCountScore(board,player);
        score += weightCorners * cornerScore(board, player);
        // Include other scoring components as desired
//        BoardHelper::printBoardState(board);
//        cout<<player<<" "<<score<<endl;

        return score;
    }
};


#endif //CSCI_561_HW2_EVALUATOR_H
