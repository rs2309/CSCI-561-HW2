//
// Created by Rutvik Shah on 2/15/24.
//
#include "BoardHelper.h"
#ifndef CSCI_561_HW2_RTEVALUATOR_H
#define CSCI_561_HW2_RTEVALUATOR_H

static int SQUARE_SCORE[N][N] = {
        {120, -30, 20, 10, 10, 10,10, 10, 10, 20, -30, 120}, // Corners boosted, edges penalized next to corners
        {-30, -50, -10, -5, -5, -5, -5, -5, -5, -10, -50, -30}, // Adjacent to corners heavily penalized
        {20,  -10, 15,  5,  0,  0, 0,  0,  0, 15,  -10,  20}, // Transition zone between center and edge
        {10,  -5,  5, 20,  1,  1, 1,  1,  1,  5,  -5,  10}, // Central area slightly more valuable
        {10,  -5,  0,  1,  5,  2, 2,  2,  5,  0,  -5,  10}, // Near central area, promoting flexibility
        {10,  -5,  0,  1,  2,  3, 3,  3,  2,  0,  -5,  10},
        {10,  -5,  0,  1,  2,  3,3 ,  3,  2,  0,  -5,  10},
        {10,  -5,  0,  1,  5,  2, 2,   2,  5,  0,  -5,  10},
        {10,  -5,  5, 20,  1,  1, 1,  1,  1,  5,  -5,  10},
        {20,  -10, 15,  5,  0,  0,  0, 0,  0, 15,  -10,  20},
        {-30, -50, -10, -5, -5, -5, -5, -5, -5, -10, -50, -30},
        {120, -30, 20, 10, 10, 10, 10, 10, 10, 20, -30, 120},

};
vector<int> timingSet({0, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144});
vector<vector<int>> weightSet( {
// Early Game: Emphasize mobility and strategic positioning
{15, 75, -20, 10, 190, 480},  // More emphasis on mobility and less penalty for piece differential
{15, 75, -20, 10, 190, 480},

// Transition to Midgame: Balance between mobility, piece differential, and stability
{20, 70, -15, 15, 195, 485},
{25, 65, -10, 20, 200, 490},
{25, 60, -5, 25, 205, 495},
{30, 55, 0, 30, 210, 500},
{30, 50, 5, 35, 215, 505},
{35, 45, 10, 40, 220, 510},

// Mid to Late Game: Increasing emphasis on stability and corner captures
{35, 40, 15, 45, 225, 515},
{40, 35, 20, 50, 230, 520},
{40, 30, 25, 55, 235, 525},
{45, 25, 30, 60, 240, 530},
{45, 20, 35, 65, 245, 535},
{50, 15, 40, 70, 250, 540},

// Late Game: Maximizing score through corner captures and maintaining lead
{50, 10, 45, 75, 255, 545},
{55, 5, 50, 80, 260, 550},
{55, 0, 55, 85, 265, 555},
{60, -5, 60, 90, 270, 560},
{60, -5, 60, 90, 270, 560}
});

class RTEvaluator {
public:
    RTEvaluator(){
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
    int eval(const Reversi& board, int player){

        int score = 0;
        int totalStones = BoardHelper::totalDiscCount(board);
//        cout<<totalStones<<" "<<weightSetForDiscCount.size()<<endl;

        vector<int> weights = weightSetForDiscCount[totalStones];

        score += weights[0] != 0 ? weights[0] * mobility(board, player) : 0;
        score += weights[1] != 0 ? weights[1] * frontier(board, player) : 0;
        score += weights[2] != 0 ? weights[2] * pieces(board, player) : 0;
        score += weights[3] != 0 ? weights[3] * placement(board, player) : 0;
        score += weights[4] != 0 ? weights[4] * stability(board, player) : 0;
        score += weights[5] != 0 ? weights[5] * cornerGrab(board, player) : 0;
//        cout<<score<<endl;
        return score;
    }

private:
    vector<vector<int>> weightSetForDiscCount;
    static int pieces(const Reversi& board, int player){
        int playerDisks = 0, opponentDisks = 0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == player) ++playerDisks;
                else if (board[x][y] == 1 - player) ++opponentDisks;
            }
        }
        return 100*(int)(playerDisks - (double)opponentDisks)/(playerDisks+opponentDisks);
    }

    static int placement(const Reversi& board, int player){
        int myScore = 0, opponentScore = 0;

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == player) myScore += SQUARE_SCORE[i][j];
                else if (board[i][j] == 1-player) opponentScore += SQUARE_SCORE[i][j];
            }
        }

        return myScore - opponentScore;
    }
    static int stability(const Reversi& board, int player) {
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
        return 100 * (myStableDiscs - opStableDiscs) / (myStableDiscs + opStableDiscs);
    }
    static int mobility(const Reversi& board, int player){
        int playerMoves = BoardHelper::findNextMoves(board, player, false).size();
        int opponentMoves = BoardHelper::findNextMoves(board, 1 - player, false).size();
        return  (playerMoves - opponentMoves)/(playerMoves + opponentMoves+1);
    }
    static int frontier(const Reversi& board, int player){
        int myFrontier = BoardHelper::getFrontierSquares(board, player).size();
        int opFrontier = BoardHelper::getFrontierSquares(board, 1-player).size();
        return 100 * (myFrontier - opFrontier) / (myFrontier + opFrontier + 1);
    }
    static int cornerGrab(const Reversi& board, int player){
        for (const auto& move : BoardHelper::findNextMoves(board, player,false)) {
            if ((move.x == 0 && move.y == 0) || (move.x == 0 && move.y == 7) ||
                (move.x == 7 && move.y == 0) || (move.x == 7 && move.y == 7)) {
                return 100; // Highly prioritize corner captures
            }
        }
        return 0;
    }

    // Assuming BoardHelper class exists and provides necessary functionality.
};




#endif //CSCI_561_HW2_RTEVALUATOR_H
