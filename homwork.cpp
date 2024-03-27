//
// Created by Rutvik Shah on 2/13/24.
//

#include "ai/BoardHelper.h"
#include "ai/GameEngine.h"
////#include "ai/MTDFPlayer.h"
#include "ai/ScriptPlayer.h"
//#include "ai/MTDFPlayer.h"
#include "ai/MinMaxPlayer.h"
#include "ai/RTEvaluator.h"
////#include "ai/Evaluator.h"
#include "ai/ConsolePlayer.h"
//#include "ai/GA.h"
#include "ai/RandomPlayer.h"
vector<vector<int>> SQUARE_SCORE1= {
        {1200, -500, 20, 10, 10, 10,10, 10, 10, 20, -500, 1200}, // Corners boosted, edges penalized next to corners
        {-500, -500, -100, -50, -50, -50, -50, -50, -50, -100, -500, -500}, // Adjacent to corners heavily penalized
        {20,  -100, 15,  5,  0,  0, 0,  0,  0, 15,  -100,  20}, // Transition zone between center and edge
        {10,  -50,  5, 20,  1,  1, 1,  1,  1,  5,  -50,  10}, // Central area slightly more valuable
        {10,  -50,  0,  1,  5,  2, 2,  2,  5,  0,  -50,  10}, // Near central area, promoting flexibility
        {10,  -50,  0,  1,  2,  3, 3,  3,  2,  0,  -50,  10},
        {10,  -50,  0,  1,  2,  3,3 ,  3,  2,  0,  -50,  10},
        {10,  -50,  0,  1,  5,  2, 2,   2,  5,  0,  -50,  10},
        {10,  -50,  5, 20,  1,  1, 1,  1,  1,  5,  -50,  10},
        {20,  -100, 15,  5,  0,  0,  0, 0,  0, 15,  -100,  20},
        {-500, -500, -100, -50, -50, -50, -50, -50, -50, -100, -500, -500},
        {1200, -500, 20, 10, 10, 10, 10, 10, 10, 20, -500, 1200},

};
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
vector<int> timingSet2({
                              0, 10, 20, 30, 50, 60, 70, 80, 90,100, 110,  10, 130, 144});
vector<vector<int>> weightSet2( {
// Early Game: Emphasize mobility and strategic positioning
                                       {35, 10, 5, 25, 15, 120},
                                       {33, 12, 6, 23, 17, 120},
                                       {31, 14, 7, 21, 19, 120},
                                       {30, 16, 8, 19, 21, 120},
                                       {29, 18, 10, 17, 23, 120},
                                       {28, 20, 12, 15, 25, 120},
                                       {27, 22, 14, 13, 27, 120},
                                       {26, 24, 16, 11, 29, 120},
                                       {26, 26, 18, 11, 31, 120},
                                       {25, 28, 20, 11, 33, 120},
                                       {25, 30, 22, 11, 35, 120},
                                       {24, 32, 24, 11, 37, 120},
                                       {24, 34, 26, 11, 39, 120},
                                       {24, 36, 28, 11, 41, 120}
});

vector<int> timingSet1={
        18, 36, 54, 72, 90, 108, 126, 144
};
vector<vector<int>> weightSet1( {
// Early Game: Emphasize mobility and strategic positioning
                                        {30, 5, -8, 40, 2, 20, 1, 10}, // Opening
                                        {25, 10, -4, 25, 10, 15, 5, 9}, // Early Mid-Game
                                        {20, 15, 10, 20, 15, 10, 5, 5}, // Mid-Game
                                        {15, 20, 15, 15, 15, 10, 5, 5}, // Mid-Game Transition
                                        {10, 15, 20, 10, 20, 5, 10, 10}, // Early End-Game
                                        {8, 10, 25, 5, 25, 15, 8, 0}, // End-Game Preparation
                                        {8, 10, 30, 5, 25, 15, 8, 0}, // Late End-Game
                                        {2, 5, 40, 0, 30, 15, 8, 0}

                                });
vector<int> timingSet3({
                               0,124,126,128,130,133,135,137,140,144});
std::vector<std::vector<int>> weightSet3 = {

        {8, 85, -40, 10, 210, 520, 20},
        {8, 85, -40, 10, 210, 520, 20},
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
vector<int> timingSet4({
                              0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 144
                      });
std::vector<std::vector<int>> weightSet4 = {
/// Early Game: Increase emphasis on mobility and corner grabbing
        {100, 20, 5, 30, 10, 150},
        {95, 22, 6, 28, 12, 150},
        {90, 24, 7, 26, 14, 150},
        {85, 26, 8, 24, 16, 150},
        // Mid-Early Game: Begin to balance between mobility and stability
        {80, 28, 10, 22, 20, 140},
        {75, 30, 12, 20, 25, 140},
        {70, 32, 14, 18, 30, 140},
        {65, 34, 16, 16, 35, 140},
        // Mid Game: Stability and piece control become more critical
        {60, 36, 20, 15, 40, 130},
        {55, 38, 25, 14, 45, 130},
        {50, 40, 30, 13, 50, 130},
        {45, 42, 35, 12, 55, 130},
        // Late Game: Maximize stability and piece count
        {40, 44, 40, 11, 60, 120},
        {35, 46, 45, 10, 65, 120},
        {30, 48, 50, 10, 70, 120}
}; // Late Endgame
int main(){

//    Reversi board;
//    double t1,t2;
//    int current_player;
//    tie(board,t1,t2,current_player)=BoardHelper::getStartBoard();
//    GA::GArun();

////
    std::unique_ptr<RTEvaluator> rt=std::make_unique<RTEvaluator>(timingSet3,weightSet3,SQUARE_SCORE2);
    std::unique_ptr<Player> Player1 = std::make_unique<MinMaxPlayer>(0,std::move(rt));

//    std::unique_ptr<RTEvaluator> rt2=std::make_unique<RTEvaluator>(timingSet3,weightSet3,SQUARE_SCORE2);
//    std::unique_ptr<Player> Player2 = std::make_unique<MinMaxPlayer>(1,std::move(rt2));
////    std::unique_ptr<Evaluator> st=std::make_unique<Evaluator>();
//


//    std::unique_ptr<Player> Player2 = std::make_unique<ConsolePlayer>(1);
//    std::unique_ptr<Player> Player2 = std::make_unique<ScriptPlayer>(0,"FolderName/Player1");
//    std::unique_ptr<Player> Player2 = std::make_unique<ScriptPlayer>(1,"FolderName/Player2");
    std::unique_ptr<Player> Player2 = std::make_unique<RandomPlayer>(1);
    GameEngine g(std::move(Player1), std::move(Player2));

    g.run();

//    int depth = 4; // Depth of the search
//    t1=300.00;
//    double t2=300.00;
////    NMTDF m(current_player);
//    current_player=1-current_player;
//    Minimax n(current_player);
//    int moveCount=0;
//    while(!BoardHelper::isGameFinished(board)){
//
////        if(BoardHelper::hasAnyMoves(board,1-current_player)){
////            auto res = BoardHelper::makeFileForOpponent(board,t1,t2,1-current_player);
////            auto moves=BoardHelper::findNextMoves(board,1-current_player,false);
////            cout << 1-current_player<< ", at position:"<< (char)(97+res.y)<<res.x+1<< endl;
////            if(find(moves.begin(),moves.end(),res)!=moves.end()){
////                BoardHelper::performMove(board,res,1-current_player);
////            }else{
////                for(auto m: moves){
////                    cout<<m.x<<"-"<<m.y<<" ";
////                }
////                cout<<endl;
////                cout<<res.x<<"-"<<res.y<<endl;
////                cout<<"Invalid Move"<<endl;
////                break;
////            }
////            moveCount++;
////
////        }
//
//        if(BoardHelper::hasAnyMoves(board,current_player)){
//            auto start = chrono::steady_clock::now();
//
//            auto result = n.Solve(board, moveCount>4?4:4);
//            BoardHelper::performMove(board,result,current_player);
//            cout << current_player << ", at position:"<< (char)(97+result.y)<<result.x+1<< endl;
//            moveCount++;
//
//            auto end = chrono::steady_clock::now();
//            auto diff = end - start;
//            cout <<chrono::duration <double, milli> (diff).count() << " ms" << endl;
//        }
//
////        BoardHelper::printBoardState(board);
////        if(BoardHelper::hasAnyMoves(board,1-current_player)){
////            auto result = m.iterativeDeepening(board,4);
////            BoardHelper::performMove(board,result,1-current_player);
////            cout << 1-current_player<< ", at position:"<< (char)(97+result.y)<<result.x+1<< endl;
////            moveCount++;
//////            pair<int,int> greedyMove=BoardHelper::findNextMoves(board,1-current_player,true)[0];
//////            BoardHelper::performMove(board,greedyMove.first,greedyMove.second,1-current_player);
////        }
//
//
//
//        BoardHelper::printBoardState(board);
//        cout<<endl;
//    }
//    cout<<BoardHelper::getWinner(board)<<endl;

////UNCOMMENT for Submission

//
//    std::unique_ptr<RTEvaluator> rt=std::make_unique<RTEvaluator>(timingSet2,weightSet2);
//    // std::unique_ptr<RTEvaluator> rt2=std::make_unique<RTEvaluator>();
////std::unique_ptr<Evaluator> st=std::make_unique<Evaluator>();
//
//    std::unique_ptr<Player> Player1 = std::make_unique<MinMaxPlayer>(current_player,std::move(rt),3);
//
//
//    auto start = chrono::steady_clock::now();
//    auto result = Player1->playMove(board, t1);
//    auto end = chrono::steady_clock::now();
//
//    //TIME
//    auto diff = end - start;
//    cout <<chrono::duration <double, milli> (diff).count() << " ms" << endl;
//    ofstream outputFile("output.txt");
//
//    // Check if the file is open/valid
//    if (!outputFile.is_open()) {
//        cerr << "Unable to open file" << endl;
//        return 502; // Exit the program with an error code
//    }
//
//
//    outputFile<<(char)(97+result.y)<<result.x+1<<endl;
//    outputFile.close();


    return 0;

}
