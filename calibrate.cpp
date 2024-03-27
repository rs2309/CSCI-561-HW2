//
// Created by Rutvik Shah on 2/13/24.
//

#include "ai/BoardHelper.h"
//#include "ai/GameEngine.h"
//#include "ai/MTDFPlayer.h"
//#include "ai/ScriptPlayer.h"
//#include "ai/MTDFPlayer.h"
#include "ai/MinMaxPlayer.h"
#include "ai/RTEvaluator.h"
//#include "ai/Evaluator.h"

vector<vector<int>> SQUARE_SCORE2 = {
        {100, -20, 10,  8,  7,  7,  7,  7,  8, 10, -20, 100},
        {-20, -40, -5, -5, -3, -3, -3, -3, -5, -5, -40, -20},
        {10,  -5,  5,  1,  1,  1,  1,  1,  1,  5,  -5,  10},
        {8,   -5,  1,  3,  2,  2,  2,  2,  3,  1,  -5,  8},
        {7,   -3,  1,  2,  1,  1,  1,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  0,  0,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  0,  0,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  1,  1,  1,  2,  1,  -3,  7},
        {8,   -5,  1,  3,  2,  2,  2,  2,  3,  1,  -5,  8},
        {10,  -5,  5,  1,  1,  1,  1,  1,  1,  5,  -5,  10},
        {-20, -40, -5, -5, -3, -3, -3, -3, -5, -5, -40, -20},
        {100, -20, 10,  8,  7,  7,  7,  7,  8, 10, -20, 100}
};

vector<int> timingSet3({
                               0,124,126,128,130,133,135,137,140,144});
std::vector<std::vector<int>> weightSet3 = {

        {8, 85, -5, 10, 210, 520, 20},
        {8, 85, -2, 10, 210, 520, 20},
        {33, -50, -1, 4, 416, 2153, 20},
        {46, -50, 0, 3, 612, 4141, 20},
        {51, -50, 62, 3, 595, 3184, 20},
        {33, -5,  66, 2, 384, 2777, 20},
        {44, 50, 163, 0, 443, 2568, 20},
        {13, 50, 66, 0, 121, 986, 20},
        {4, 50, 31, 0, 27, 192, 20},
        {8, 500, 77, 0, 36, 299, 20}
//        {25, 85, -40, 10, 210, 520},    // Early Game
//        {28, 95, -40, 10, 310, 720},    // Just before Mid-game Transition
//        {33, -30, -15, 4, 616, 2553},  // Early Mid-game
//        {46, -20, -1, 3, 912, 4741},   // Mid-game
//        {51, -10, 62, 3, 995, 3984},   // Mid to Late Mid-game
//        {33, 0, 116, 2, 1134, 4277},   // Transition to Endgame
//        {44, 300, 223, 0, 1343, 4368},  // Early Endgame
//        {40, 350, 136, 0, 1171, 4369},  // Mid Endgame
//        {38, 400, 300, 0, 1227, 4370},   // Late Endgame
//        {36, 455, 350, 0, 1386, 4372}

//        {25, 85, -40, 10, 210, 520},    // Early Game
//        {28, 95, -40, 10, 310, 720},    // Just before Mid-game Transition
//        {33, -30, -15, 4, 616, 2553},  // Early Mid-game
//        {46, -20, -1, 3, 912, 4741},   // Mid-game
//        {51, -10, 62, 3, 995, 3984},   // Mid to Late Mid-game
//        {33, 0, 116, 2, 1134, 4277},   // Transition to Endgame
//        {44, 20, 223, 0, 1343, 4368},  // Early Endgame
//        {13, 15, 136, 0, 1171, 3086},  // Mid Endgame
//        {4, 10, 111, 0, 1227, 2592},   // Late Endgame
//        {8, 455, 167, 0, 1386, 2999}   // Final Phase
};

int main(){

    Reversi board;
    double t1,t2;
    int current_player;
    tie(board,t1,t2,current_player)=BoardHelper::getStartBoard();


    std::unique_ptr<RTEvaluator> rt2=std::make_unique<RTEvaluator>(timingSet3,weightSet3,SQUARE_SCORE2);
    std::unique_ptr<Player> Player1 = std::make_unique<MinMaxPlayer>(current_player,std::move(rt2));





////UNCOMMENT for Submission
    auto start = chrono::steady_clock::now();
    auto result = Player1->playMove(board, t1);
    auto end = chrono::steady_clock::now();

    //TIME
    auto diff = end - start;
    cout <<chrono::duration <double, milli> (diff).count() << " ms" << endl;
    ofstream outputFile("output.txt");

    // Check if the file is open/valid
    if (!outputFile.is_open()) {
        cerr << "Unable to open file" << endl;
        return 502; // Exit the program with an error code
    }


    outputFile<<(char)(97+result.y)<<result.x+1<<endl;
    outputFile.close();


    return 0;
}
