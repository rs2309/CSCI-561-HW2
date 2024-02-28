//
// Created by Rutvik Shah on 2/14/24.
//

#include "BoardHelper.h"
#include "Evaluator.h"
//#include "RTEvaluator.h"

#ifndef CSCI_561_HW2_MINIMAX_H
#define CSCI_561_HW2_MINIMAX_H

typedef std::string BoardHashMM;
BoardHashMM hashBoardMM(Reversi board,int player) {
    string hash=string(5,player?'X':'O');
    for (const auto& row : board) {
        for (int cell : row) {
            // Convert each cell value to a character and append to the hash string
            // Assuming cell values are -1, 0, and 1, we shift them to '0', '1', '2' for simplicity
            hash += std::to_string(cell+1);
        }
    }
    return hash;
}
struct TTEntryMM {
    int depth;
    int value;
};

void updateTranspositionTable(unordered_map<BoardHashMM, TTEntryMM> &transpositionTable,string hash, int depth, int value) {
    auto ttEntryIt = transpositionTable.find(hash);
    if (ttEntryIt == transpositionTable.end() || ttEntryIt->second.depth <= depth) {
        // Update the transposition table only if the new information is from a deeper or equal depth search
        transpositionTable[hash] = {depth, value};
    }
}
class Minimax {
    unordered_map<BoardHashMM, TTEntryMM> transpositionTable;
    int pp,fp;
    int current_player;
    Evaluator r;
    int nodesExplored;
public:
    Minimax(int player){
        r=Evaluator();
        current_player=player;
    }
    Move Solve(Reversi board, int depth){
        pp=0;
        fp=0;
        nodesExplored=0;
        auto moves = BoardHelper::findNextMoves(board, current_player,true);

        Move bestMove={-1,-1};

        //It is guaranteed that move is available
        int maxEval = numeric_limits<int>::min();
        int alpha=numeric_limits<int>::min(), beta=numeric_limits<int>::max();
        for (auto& move : moves) {
            vector<Move> flippedDisks = BoardHelper::performMove(board, move, current_player);
            auto eval=MMAB(board, depth-1, alpha,beta, false, 1-current_player);
            BoardHelper::undoMove(board, flippedDisks, move, current_player);
            if(eval>maxEval){
                maxEval=eval;
                bestMove=move;
            }
            alpha = max(alpha, eval);
        }
        cout<<current_player<<" Score:"<<maxEval<<" ("<<bestMove.x<<","<<bestMove.y<<") Nodes Explored: "<<nodesExplored<<endl;
//        cout<<pp<<" "<<fp<<" "<<transpositionTable.size()<<endl;
        return bestMove;
    }
    int MMAB(Reversi board, int depth, int alpha, int beta, bool maximizingPlayer, int player) {
        BoardHashMM hash = hashBoardMM(board,current_player);

        // Check if the board state is already in the transposition table
        auto ttEntry = transpositionTable.find(hash);
        if (ttEntry != transpositionTable.end() && ttEntry->second.depth >= depth) {
            // Use the cached value if it's valid for the current or deeper search
//            cout<<"Returning Hash"<<depth<<endl;
            return ttEntry->second.value;
        }
        nodesExplored++;
        if (depth == 0 || BoardHelper::isGameFinished(board)) {
            int eval = r.eval(board,current_player);
            updateTranspositionTable(transpositionTable,hash, depth, eval); // Here, bestMove might be an invalid move indicating a leaf node
            return eval;
        }

        auto moves = BoardHelper::findNextMoves(board, player,true);

        if (moves.empty()) {
            int eval= MMAB(board,depth-1,alpha,beta,!maximizingPlayer,1-player);
            updateTranspositionTable(transpositionTable,hash, depth, eval);
            return eval;
        }

        if (maximizingPlayer) {
            int maxEval = numeric_limits<int>::min();
            for (auto& move : moves) {
                vector<Move> flippedDisks = BoardHelper::performMove(board, move, player);
                auto eval = MMAB(board, depth - 1, alpha, beta, false, 1 - player);
                BoardHelper::undoMove(board, flippedDisks, move, player);
                maxEval=max(eval, maxEval);
                alpha = max(alpha, eval);
                if (eval >= beta) {
                    pp++;
                    break; // Beta cut-off
                }
            }
            updateTranspositionTable(this->transpositionTable,hash, depth, maxEval);
            return maxEval;
        } else {
            int minEval = numeric_limits<int>::max();
            for (auto& move : moves) {
                vector<Move> flippedDisks = BoardHelper::performMove(board, move, player);
                auto eval = MMAB(board, depth - 1, alpha, beta, true, 1 - player);
                BoardHelper::undoMove(board, flippedDisks, move, player);
                minEval=min(minEval,eval);
                beta = min(beta, eval);
                if (eval <= alpha) {
                    fp++;
                    break; // Alpha cut-off
                }
            }
            updateTranspositionTable(this->transpositionTable,hash, depth, minEval);
            return minEval;
        }

    }

};


#endif //CSCI_561_HW2_MINIMAX_H
