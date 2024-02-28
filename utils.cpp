//
// Created by Rutvik Shah on 2/13/24.
//

#include "ai/BoardHelper.h"
#include "ai/GameEngine.h"
//#include "ai/MTDFPlayer.h"
#include "ai/ScriptPlayer.h"
#include "ai/MinMaxPlayer.h"

int main(){

//    Reversi board;
//    double t1,t2;
//    int current_player;
//    tie(board,t1,t2,current_player)=BoardHelper::getStartBoard();


    std::unique_ptr<Player> Player2 = std::make_unique<ScriptPlayer>(0);
    std::unique_ptr<Player> Player1 = std::make_unique<MinMaxPlayer>(1);
    GameEngine g(std::move(Player2), std::move(Player1));

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
//    auto start = chrono::steady_clock::now();
//    Minimax m;
//    auto result = m.Solve(board, depth, current_player);
//    cout << "Best move score: " << 0 << ", at position: (" << result.x << ", " << result.y << ")" << endl;
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
