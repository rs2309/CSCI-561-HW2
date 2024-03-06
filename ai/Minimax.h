//
// Created by Rutvik Shah on 2/14/24.
//

#include "BoardHelper.h"
#include "BaseEvaluator.h"

#ifndef CSCI_561_HW2_MINIMAX_H
#define CSCI_561_HW2_MINIMAX_H

//typedef std::string BoardHashMM;
//BoardHashMM hashBoardMM(Reversi board,int player) {
//    string hash=string(5,player?'X':'O');
//    for (const auto& row : board) {
//        for (int cell : row) {
//            // Convert each cell value to a character and append to the hash string
//            // Assuming cell values are -1, 0, and 1, we shift them to '0', '1', '2' for simplicity
//            hash += std::to_string(cell+1);
//        }
//    }
//    return hash;
//}
//struct TTEntryMM {
//    int depth;
//    int value;
//};
//
//void updateTranspositionTable(unordered_map<BoardHashMM, TTEntryMM> &transpositionTable,string hash, int depth, int value) {
//    auto ttEntryIt = transpositionTable.find(hash);
//    if (ttEntryIt == transpositionTable.end() || ttEntryIt->second.depth <= depth) {
//        // Update the transposition table only if the new information is from a deeper or equal depth search
//        transpositionTable[hash] = {depth, value};
//    }
//}

//void estimateCoefficients(const std::vector<DepthTimeData>& data, double& a, double& b) {
//    // Using a simple method to estimate a and b
//    // This method assumes only two points but can be extended or refined
//
//    std::vector<double> logT;
//    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
//    int S = data.size();
//
//    for (const auto& dt : data) {
//        double logTime = std::log(dt.time);
//        logT.push_back(logTime);
//        sumX += dt.depth;
//        sumY += logTime;
//        sumXY += dt.depth * logTime;
//        sumXX += dt.depth * dt.depth;
//    }
//
//    double logB = (S * sumXY - sumX * sumY) / (S * sumXX - sumX * sumX);
//    double logA = (sumY - logB * sumX) / S;
//
//    a = std::exp(logA);
//    b = std::exp(logB);
//}
//void saveModelParameters(double a, double b) {
//    ofstream outFile("playData.txt");
//    if (outFile.is_open()) {
//        outFile << a << " " << b << "\n";
//        outFile.close();
//    } else {
//        std::cerr << "Unable to open file for writing.\n";
//    }
//}
class Minimax {
//    unordered_map<BoardHashMM, TTEntryMM> transpositionTable;
    int pp,fp;
    int current_player;
    unique_ptr<BaseEvaluator> r;
    int nodesExplored;
public:
    Minimax(int player,unique_ptr<BaseEvaluator> E){
        r=move(E);
        current_player=player;
    }
    Move Solve(Reversi board, double timeBudget){
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
                auto eval = MMAB(BoardHelper::performMove(board, sortedMoves[i].first, current_player), d - 1, alpha, beta, false,
                                 1 - current_player);
                sortedMoves[i].second=eval;
                alpha = max(alpha, eval);
            }
            sort(sortedMoves.begin(), sortedMoves.end(),
                 [](const pair<Move, int>& a, const pair<Move, int>& b) {
                     return a.second > b.second; // Descending order by score
                 });
            cout<<current_player<<" Depth "<<d<<" Score:"<<sortedMoves[0].second<<" Randomized Move:("<<sortedMoves[0].first.x<<","<<sortedMoves[0].first.y<<") Nodes Explored: "<<nodesExplored<<" "<<pp<<" "<<fp<<endl;

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

        //Selecting the Randomized Move
//        int highestScore = sortedMoves.front().second;
//        vector<std::pair<Move, int>> highestScoringMoves;
//        for (const auto& move : sortedMoves) {
//            if (move.second == highestScore) {
//                highestScoringMoves.push_back(move);
//            } else {
//                break; // Since the vector is sorted, we can break at the first instance of a lower score
//            }
//        }
//        auto SelectedMove= highestScoringMoves[rand() % highestScoringMoves.size()];
        for(auto m: sortedMoves){
            cout<<"("<<m.first.x<<" "<<m.first.y<<")~ "<<m.second<<" ";
        }
        cout<<endl;
        bestMove=sortedMoves[0].first;
        return bestMove;
    }
//    Move Solve(Reversi board, int depth,bool logging=false){
//
//        vector<DepthTimeData> data;
//        auto moves = BoardHelper::findNextMoves(board, current_player,false);
//        Move bestMove={-1,-1};
//
//        vector<pair<Move, int>> sortedMoves;
//        for(auto move: moves){
//            sortedMoves.push_back({move,0});
//        }
//        //It is guaranteed that move is available
//        for(int d=1; d<=depth; d++){
//            auto start = std::chrono::high_resolution_clock::now();
//
//            pp=0;
//            fp=0;
//            nodesExplored=0;
//            int alpha=numeric_limits<int>::min(), beta=numeric_limits<int>::max();
//            for (int i=0;i<sortedMoves.size();i++) {
//                auto eval = MMAB(BoardHelper::performMove(board, sortedMoves[i].first, current_player), d - 1, alpha, beta, false,
//                                 1 - current_player);
////                cout << move.x << " " << move.y << " " << eval << endl;
////                if (eval > maxEval) {
////                    maxEval = eval;
////                    bestMove = move;
////                }
//                sortedMoves[i].second=eval;
//                alpha = max(alpha, eval);
//            }
//            sort(sortedMoves.begin(), sortedMoves.end(),
//                      [](const pair<Move, int>& a, const pair<Move, int>& b) {
//                          return a.second > b.second; // Descending order by score
//                      });
//
//            auto end = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> duration = end - start;
//            if (logging)
//                data.push_back(DepthTimeData{d,duration.count()});
//            cout<<current_player<<" Depth "<<d<<" Score:"<<sortedMoves[0].second<<" Randomized Move:("<<sortedMoves[0].first.x<<","<<sortedMoves[0].first.y<<") Nodes Explored: "<<nodesExplored<<" "<<pp<<" "<<fp<<endl;
//        }
//        if(logging){
//            double a, b;
//            estimateCoefficients(data, a, b);
//            saveModelParameters(a,b);
//        }
//        //Selecting the Randomized Move
////        int highestScore = sortedMoves.front().second;
////        vector<std::pair<Move, int>> highestScoringMoves;
////        for (const auto& move : sortedMoves) {
////            if (move.second == highestScore) {
////                highestScoringMoves.push_back(move);
////            } else {
////                break; // Since the vector is sorted, we can break at the first instance of a lower score
////            }
////        }
////        auto SelectedMove= highestScoringMoves[rand() % highestScoringMoves.size()];
//        bestMove=sortedMoves[0].first;
////        cout<<current_player<<" Depth "<<depth<<" Score:"<<SelectedMove.second<<" Randomized Move:("<<bestMove.x<<","<<bestMove.y<<") Nodes Explored: "<<nodesExplored<<" "<<pp<<" "<<fp<<endl;
//        return bestMove;
//    }
    int MMAB(Reversi board, int depth, int alpha, int beta, bool maximizingPlayer, int player) {

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
                auto eval = MMAB(BoardHelper::performMove(board, move, player), depth - 1, alpha, beta, false, 1 - player);
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
                auto eval = MMAB(BoardHelper::performMove(board, move, player), depth - 1, alpha, beta, true, 1 - player);
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
