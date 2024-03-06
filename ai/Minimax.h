//
// Created by Rutvik Shah on 2/14/24.
//

#include "BoardHelper.h"
#include "BaseEvaluator.h"

#ifndef CSCI_561_HW2_MINIMAX_H
#define CSCI_561_HW2_MINIMAX_H


class Minimax {
    int pp,fp;
    int current_player;
    unique_ptr<BaseEvaluator> r;
    int nodesExplored;
public:
    Minimax(int player,unique_ptr<BaseEvaluator> E){
        pp=0;
        fp=0;
        nodesExplored=0;
        r=std::move(E);
        current_player=player;
    }
    Move Solve(Reversi& board, double timeBudget){
        auto startTime = std::chrono::high_resolution_clock::now();
        double timeTaken = 0;

        auto moves = BoardHelper::findNextMoves(board, current_player,false);
        Move bestMove={-1,-1};

        vector<pair<Move, int>> sortedMoves;
        for(auto move: moves){
            sortedMoves.push_back({move,0});
        }
        //It is guaranteed that move is available
        for(int d=MIN_DEPTH; d<=MAX_DEPTH; d++){
            auto depthStartTime = std::chrono::high_resolution_clock::now();

            pp=0;
            fp=0;
            nodesExplored=0;
            int alpha=numeric_limits<int>::min(), beta=numeric_limits<int>::max();
            for (int i=0;i<sortedMoves.size();i++) {
                Reversi tempBoard=board;
                BoardHelper::performMove(board, sortedMoves[i].first, current_player);
                auto eval = MMAB(board , d - 1, alpha, beta, false,
                                 1 - current_player);
                board=tempBoard;

                sortedMoves[i].second=eval;
                alpha = max(alpha, eval);
            }
            sort(sortedMoves.begin(), sortedMoves.end(),
                 [](const pair<Move, int>& a, const pair<Move, int>& b) {
                     return a.second > b.second; // Descending order by score
                 });
            cout<<current_player<<" Depth "<<d<<" Score:"<<sortedMoves[0].second<<" Selected Move:("<<sortedMoves[0].first.x<<","<<sortedMoves[0].first.y<<") Nodes Explored: "<<nodesExplored<<" "<<pp<<" "<<fp<<endl;

            auto depthEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> depthTime = depthEndTime - depthStartTime;
            timeTaken += depthTime.count();

            // Predict if we have time for another depth based on exponential growth assumption
            if ((timeTaken * 2) + timeTaken > timeBudget) break;

            // Check if the time budget has been exceeded
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> totalTime = currentTime - startTime;
            if (totalTime.count() > timeBudget) break;
        }

        for(auto m: sortedMoves){
            cout<<"("<<m.first.x<<" "<<m.first.y<<")~ "<<m.second<<" ";
        }
        cout<<endl;
        bestMove=sortedMoves[0].first;
        return bestMove;
    }

    int MMAB(Reversi& board, int depth, int alpha, int beta, bool maximizingPlayer, int player) {

        nodesExplored++;
        if (depth == 0 || BoardHelper::isGameFinished(board)) {
            int eval = r->eval(board,current_player);
            return eval;
        }

        auto moves = BoardHelper::findNextMoves(board, player,false);

        if (moves.empty()) {
            int eval= MMAB(board,depth-1,alpha,beta,!maximizingPlayer,1-player);
            return eval;
        }

        if (maximizingPlayer) {
            int maxEval = numeric_limits<int>::min();
            for (auto& move : moves) {
                Reversi tempBoard=board;
                BoardHelper::performMove(board, move, player);
                auto eval = MMAB(board, depth - 1, alpha, beta, false, 1 - player);
                board=tempBoard;

                maxEval=max(eval, maxEval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    pp++;
                    break; // Beta cut-off
                }
            }
            return maxEval;
        } else {
            int minEval = numeric_limits<int>::max();
            for (auto& move : moves) {
                Reversi tempBoard=board;
                BoardHelper::performMove(board, move, player);
                auto eval = MMAB(board, depth - 1, alpha, beta, true, 1 - player);
                board=tempBoard;

                minEval=min(minEval,eval);
                beta = min(beta, eval);
                if (beta <= alpha) {
                    fp++;
                    break; // Alpha cut-off
                }
            }
            return minEval;
        }

    }
//
};


#endif //CSCI_561_HW2_MINIMAX_H
