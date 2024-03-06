//
// Created by Rutvik Shah on 2/15/24.
//
#include <utility>

#include "BoardHelper.h"
#include "BaseEvaluator.h"
#ifndef CSCI_561_HW2_RTEVALUATOR_H
#define CSCI_561_HW2_RTEVALUATOR_H


vector<vector<int>> SQUARE_SCORE;

class ReversiOptimized {
public:
    struct EvaluationResult {
        int piecesScore;
        int placementScore;
//        int stabilityScore;
        int mobilityScore;
        int frontierScore;
        int cornerGrabScore;
        int gameReward;
        int isolationScore;
    };

    static EvaluationResult evaluateBoard(Reversi& board, int player) {
        int playerDisks = 0, opponentDisks = 0, myScore = 0, opponentScore = 0;
//        int myStableDiscs = 0, opStableDiscs = 0; // Simplified stability calculation
//
//        int  myIsolatedDiscs=0, opIsolatedDiscs=0;

        vector<Move> playerMoves = BoardHelper::findNextMoves(board, player, false);
        vector<Move> opponentMoves = BoardHelper::findNextMoves(board, 1 - player, false);

        int myFrontier = 0, opFrontier = 0;


        if(player==1)
            playerDisks++;
        else
            opponentDisks++;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int cell = board[i][j];
                if (cell == player) {
                    ++playerDisks;
                    int reversal=BoardHelper::isAdjacentToCornerAndCaptured(board,i,j,player);
                    if(reversal==0)
                        myScore += SQUARE_SCORE[i][j];
                    else
                        myScore += reversal*SQUARE_SCORE[i][j];
//                    myScore += SQUARE_SCORE[i][j];
//                     myStableDiscs += (int)BoardHelper::isStable(board, i, j,player);
//                        myIsolatedDiscs+=(int)BoardHelper::isPotentialIsland(board,i,j,player);
                } else if (cell == 1 - player) {
                    ++opponentDisks;
                    int reversal=BoardHelper::isAdjacentToCornerAndCaptured(board,i,j,1-player);
                    if(reversal==0)
                        opponentScore += SQUARE_SCORE[i][j];
                    else
                        opponentScore += reversal*SQUARE_SCORE[i][j];
//                    opponentScore += SQUARE_SCORE[i][j];
//                    opStableDiscs += (int)BoardHelper::isStable(board, i, j,1-player);
//                    opIsolatedDiscs+=(int)BoardHelper::isPotentialIsland(board,i,j,1-player);
                }

//                 Frontier calculation simplified
                if (BoardHelper::isFrontier(board, {i, j}, player)) ++myFrontier;
                if (BoardHelper::isFrontier(board, {i, j}, 1 - player)) ++opFrontier;
            }
        }

        EvaluationResult result;
        if(playerMoves.size()==0 && opponentMoves.size()==0)
            result.gameReward=playerDisks>opponentDisks?10000:-10000;
        else
            result.gameReward=0;

        result.piecesScore = calculateScore(playerDisks, opponentDisks);
        result.placementScore = calculatePlacementScore(myScore,opponentScore);
//        result.stabilityScore = 0;//calculateStability(myStableDiscs,opStableDiscs);
        result.mobilityScore = calculateMobility(playerMoves.size(), opponentMoves.size());
        result.frontierScore = calculateFrontier(myFrontier, opFrontier);
        result.cornerGrabScore = calculateCornerGrabScore(playerMoves,opponentMoves);
//        result.isolationScore= calculateIsland(myIsolatedDiscs,opIsolatedDiscs);
        result.isolationScore= calculateIslandScore(board,player,playerDisks,opponentDisks);



        return result;
    }

private:
    static int calculateIslandScore(Reversi& board, int player, int playerDiscCount, int opDiscCount) {
        std::vector<std::vector<bool>> visited(N, std::vector<bool>(N, false));
        int myScore = 0, opScore = 0;
        int totalDiscs = BoardHelper::totalDiscCount(board);
        double boardFilledPercentage = static_cast<double>(totalDiscs) / (N * N) * 100;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!visited[i][j] && board[i][j] != -1) {
                    int size = BoardHelper::dfs(i, j, board[i][j], board, visited);
                    int score = 0; // Initialize score for each cell evaluation
                    if (boardFilledPercentage < 33) {
//                        score = -size * 0.5; // Early game
                        score = (size < 4) ? -size * 2 : size * 1.5;
                    } else if (boardFilledPercentage < 66) {
                        score = (size < 4) ? -size * 2 : size * 1.5; // Mid game
                    } else {
                        score = (size < 4) ? -size * 3 : size * 2; // Late game
                    }

                    if(board[i][j] == player)
                        myScore += score;
                    else
                        opScore += score;
                }
            }
        }

        // Adjust scores by the proportion of discs
        double myScoreAdjusted = myScore * (static_cast<double>(playerDiscCount) / totalDiscs);
        double opScoreAdjusted = opScore * (static_cast<double>(opDiscCount) / totalDiscs);
//        cout<<myScore<<" "<<opScore<<endl;
        // The function should return an integer score difference
        return static_cast<int>(myScoreAdjusted - opScoreAdjusted);
    }
    static int calculatePlacementScore(int myScore, int opponentScore){
        return myScore-opponentScore;
    }
    static int calculateScore(int playerDisks, int opponentDisks) {
        return 100 * (playerDisks - opponentDisks) / (playerDisks + opponentDisks);
    }
//    static int calculateIsland(int  myIsolatedDiscs, int opIsolatedDiscs){
//        return 100*((opIsolatedDiscs-myIsolatedDiscs)/(myIsolatedDiscs+opIsolatedDiscs+1));
//    }
//    static int calculateStability(int myStableDiscs, int opStableDiscs) {
//        // Simplified stability score calculation
//        return 100 * (myStableDiscs - opStableDiscs) / (myStableDiscs + opStableDiscs+1);
//    }
    static int calculateMobility(int playerMoves, int opponentMoves) {
        if (playerMoves == 0 && opponentMoves == 0) return 0; // Simplified logic
        return 100 * (playerMoves - opponentMoves) / (playerMoves + opponentMoves+1);
    }

    static int calculateFrontier(int myFrontier, int opFrontier) {
        return 100 * (opFrontier-myFrontier) / (myFrontier + opFrontier+1);
    }
    static int  calculateCornerGrabScore(vector<Move> playerMoves, vector<Move> opMoves){
        int myCornerGrab=0,opCornerGrab=0;
        for (const auto& move : playerMoves) {
            if ((move.x == 0 && move.y == 0) || (move.x == 0 && move.y == N-1) ||
                (move.x == N-1 && move.y == 0) || (move.x == N-1 && move.y == N-1)) {

                myCornerGrab+=100; // Highly prioritize corner captures
            }
        }
        for (const auto& move : opMoves) {
            if ((move.x == 0 && move.y == 0) || (move.x == 0 && move.y == N-1) ||
                (move.x == N-1 && move.y == 0) || (move.x == N-1 && move.y == N-1)) {
                opCornerGrab+=100; // Highly prioritize corner captures
            }
        }
        return 100*(myCornerGrab-opCornerGrab)/(myCornerGrab+opCornerGrab+1);
    }
};

class RTEvaluator: public BaseEvaluator {
    vector<int> timingSet;
    vector<vector<int>> weightSet;
public:
    RTEvaluator(vector<int> newTimingSet,vector<vector<int>> newWeightSet,vector<vector<int>> newSquareArray): BaseEvaluator(){
        SQUARE_SCORE=newSquareArray;
        timingSet=newTimingSet;
        weightSet=newWeightSet;
        weightSetForDiscCount.resize(N*N+1, std::vector<int>(weightSet[0].size(), 0));
        for (int dc = 0; dc <= N*N; ++dc) {
            int w = 0;
            for (size_t i = 0; i < timingSet.size(); ++i) {
                if (dc <= timingSet[i]) {
                    w = i;
                    break;
                }
            }
            if (w == 0) {
                weightSetForDiscCount[dc] = weightSet[0];
                continue;
            }
            double factor = static_cast<double>(dc - timingSet[w - 1]) / (timingSet[w] - timingSet[w - 1]);
            for (size_t i = 0; i < weightSet[w].size(); ++i) {
                weightSetForDiscCount[dc][i] = static_cast<int>(std::round(factor * weightSet[w][i] + (1 - factor) * weightSet[w - 1][i]));
            }
        }
//        int cc=0;
//        for(auto row:weightSetForDiscCount){
//            cout<<cc++<<" ";
//            for(auto w:row){
//                cout<<w<<" ";
//            }
//            cout<<endl;
//        }

    }
    int eval(Reversi board, int player){

//        int score = 0;
        int totalStones = BoardHelper::totalDiscCount(board);
//        cout<<totalStones<<" "<<weightSetForDiscCount.size()<<endl;

        vector<int> weights = weightSetForDiscCount[totalStones];
//        for(auto weight:weights){
//            cout<<weight<<" ";
//
//        }
//        cout<<endl;

//        score += weights[0] != 0 ? weights[0] * mobility(board, player) : 0;
//        score += weights[1] != 0 ? weights[1] * frontier(board, player) : 0;
//        score += weights[2] != 0 ? weights[2] * pieces(board, player) : 0;
//        score += weights[3] != 0 ? weights[3] * placement(board, player) : 0;
//        score += weights[4] != 0 ? weights[4] * stability(board, player) : 0;
//        score += weights[5] != 0 ? weights[5] * cornerGrab(board, player) : 0;

        ReversiOptimized::EvaluationResult res=ReversiOptimized::evaluateBoard(board,player);
        int nscore=0;
        nscore += weights[0] != 0 ? weights[0] * res.mobilityScore: 0;
        nscore += weights[1] != 0 ? weights[1] * res.frontierScore : 0;
        nscore += weights[2] != 0 ? weights[2] * res.piecesScore : 0;
        nscore += weights[3] != 0 ? weights[3] * res.placementScore : 0;
        nscore += weights[4] != 0 ? weights[4] * stability(board, player) : 0;
        nscore += weights[5] != 0 ? weights[5] * res.cornerGrabScore : 0;
//        nscore += weights.size()>=7 ? weights[6]*res.stabilityScore : 10*res.stabilityScore ;
        nscore += weights.size()>=8 ? weights[7]*res.isolationScore : 20*res.isolationScore;
        nscore += 100*res.gameReward;

//
//        cout<<"Comp"<<endl;
//        BoardHelper::printBoardState(board);
//        cout<<mobility(board,player)<<" "<<res.mobilityScore<<endl;
//        cout<<frontier(board,player)<<" "<<res.frontierScore<<endl;
//        cout<<pieces(board,player)<<" "<<res.piecesScore<<endl;
//        cout<<placement(board,player)<<" "<<res.placementScore<<endl;
//        cout<<stability(board,player)<<" "<<stability(board,player)<<endl;
//        cout<<cornerGrab(board,player)<<" "<<res.cornerGrabScore<<endl;


//        cout<<score<<" "<<nscore<<endl;
        return nscore;
    }

private:
    vector<vector<int>> weightSetForDiscCount;
    static int pieces(const Reversi board, int player){
        int playerDisks = 0, opponentDisks = 2;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == player) ++playerDisks;
                else if (board[x][y] == 1 - player) ++opponentDisks;
            }
        }

        return 100*(playerDisks - opponentDisks)/(playerDisks+opponentDisks);
    }

    int placement(const Reversi board, int player){
        int myScore = 0, opponentScore = 0;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == player) myScore += SQUARE_SCORE[i][j];
                else if (board[i][j] == 1-player) opponentScore += SQUARE_SCORE[i][j];
            }
        }

        return myScore - opponentScore;
    }
    int stability(const Reversi& board, int player) {
        int oplayer = 1 - player; // Flip player: if 0 then 1, if 1 then 0

        int myStableDiscs = 0;
        int opStableDiscs = 0;

        // Check corners for the current player's stable discs
        if(board[0][0] == player) myStableDiscs += BoardHelper::getStableDisks(board, player, {0, 0}).size();
        if(board[0][N-1] == player) myStableDiscs += BoardHelper::getStableDisks(board, player, {0, N-1}).size();
        if(board[N-1][0] == player) myStableDiscs += BoardHelper::getStableDisks(board, player, {N-1, 0}).size();
        if(board[N-1][N-1] == player) myStableDiscs += BoardHelper::getStableDisks(board, player, {N-1, N-1}).size();

        // Check corners for the opponent's stable discs
        if(board[0][0] == oplayer) opStableDiscs += BoardHelper::getStableDisks(board, oplayer, {0, 0}).size();
        if(board[0][N-1] == oplayer) opStableDiscs += BoardHelper::getStableDisks(board, oplayer, {0, N-1}).size();
        if(board[N-1][0] == oplayer) opStableDiscs += BoardHelper::getStableDisks(board, oplayer, {N-1, 0}).size();
        if(board[N-1][N-1] == oplayer) opStableDiscs += BoardHelper::getStableDisks(board, oplayer, {N-1, N-1}).size();

        // Calculate the stability score
        if (myStableDiscs + opStableDiscs == 0) return 0; // To avoid division by zero
        return 100*(myStableDiscs - opStableDiscs) / (myStableDiscs + opStableDiscs);
    }
//    int mobility(const Reversi& board, int player){
//        int playerMoves = BoardHelper::findNextMoves(board, player, false).size();
//        int opponentMoves = BoardHelper::findNextMoves(board, 1 - player, false).size();
//
////        cout<<"Player Moves:"<<playerMoves<<" Opp Moves:"<<opponentMoves<<endl;
//        return  100*(playerMoves - opponentMoves)/(playerMoves + opponentMoves+1);
//
//    }
//    int frontier(const Reversi& board, int player) {
//        vector<Move> myFrontierSquares = BoardHelper::getFrontierSquares(board, player);
//        vector<Move> opFrontierSquares = BoardHelper::getFrontierSquares(board, 1 - player);
//
//        int myFrontier = myFrontierSquares.size();
//        int opFrontier = opFrontierSquares.size();
//
//        // Normalize the difference to avoid bias due to board size or game phase
//        if (myFrontier + opFrontier == 0) return 0; // Avoid division by zero if there are no frontier squares
//
//        return 100*(opFrontier - myFrontier) / (float)(myFrontier + opFrontier);
//    }
//    int cornerGrab(const Reversi board, int player){
//        int cornerGrab=0;
//        for (const auto& move : BoardHelper::findNextMoves(board, player,false)) {
//            if ((move.x == 0 && move.y == 0) || (move.x == 0 && move.y == N-1) ||
//                (move.x == N-1 && move.y == 0) || (move.x == N-1 && move.y == N-1)) {
//                return 100;
////                cornerGrab=100; // Highly prioritize corner captures
//            }
//        }
////        for (const auto& move : BoardHelper::findNextMoves(board, 1-player,false)) {
////            if ((move.x == 0 && move.y == 0) || (move.x == 0 && move.y == N-1) ||
////                (move.x == N-1 && move.y == 0) || (move.x == N-1 && move.y == N-1)) {
////                cornerGrab-=500; // Highly prioritize corner captures
////            }
////        }
//        return cornerGrab;
//    }

    // Assuming BoardHelper class exists and provides necessary functionality.
};



#endif //CSCI_561_HW2_RTEVALUATOR_H
