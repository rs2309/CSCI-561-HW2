//
// Created by Rutvik Shah on 2/13/24.
//

#include "ai/BoardHelper.h"
#include "ai/MinMaxPlayer.h"
#include "ai/RTEvaluator.h"


vector<vector<int>> SQUARE_SCORE2 = {
        {100, -100, 10,  8,  7,  7,  7,  7,  8, 10, -100, 100},
        {-100, -100, -5, -5, -3, -3, -3, -3, -5, -5, -100, -100},
        {10,  -5,  5,  1,  1,  1,  1,  1,  1,  5,  -5,  10},
        {8,   -5,  1,  3,  2,  2,  2,  2,  3,  1,  -5,  8},
        {7,   -3,  1,  2,  1,  1,  1,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  0,  0,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  0,  0,  1,  2,  1,  -3,  7},
        {7,   -3,  1,  2,  1,  1,  1,  1,  2,  1,  -3,  7},
        {8,   -5,  1,  3,  2,  2,  2,  2,  3,  1,  -5,  8},
        {10,  -5,  5,  1,  1,  1,  1,  1,  1,  5,  -5,  10},
        {-100, -100, -5, -5, -3, -3, -3, -3, -5, -5, -100, -100},
        {100, -100, 10,  8,  7,  7,  7,  7,  8, 10, -100, 100}
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
