//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_MINMAXPLAYER_H
#define CSCI_561_HW2_MINMAXPLAYER_H
#include "Player.h"
#include "Minimax.h"
#include "BaseEvaluator.h"

//bool loadModelParameters(double& a, double& b) {
//    ifstream inFile("playData.txt");
//    if (inFile.is_open()) {
//        inFile >> a >> b;
//        inFile.close();
//        return true;
//    } else {
//        std::cerr << "Unable to open file for reading. playData\n";
//        return false;
//    }
//}
//int predictDepth(double availableTime, double a, double b) {
//    // Given the simplified model and the goal to predict depth, we start with an initial guess for depth
//    // and iteratively adjust it based on the estimated time taken to reach that depth.
//    int depth = 1;
//    while (depth<=MAX_DEPTH) {
//        double predictedTime = a * std::pow(b, depth);
//        cout<<depth<<" "<<predictedTime<<endl;
//        if (predictedTime > availableTime) {
//            depth--;
//            break;// Last depth that fits within the budget
//        }
//        depth++;
//    }
//    // In theory, we should never reach here due to the loop's condition
//    return depth;
////    return max(MIN_DEPTH,min(depth,MAX_DEPTH));
//}
double adjustTimeForMove(int moveIndex, int totalMoves, double baseTimePerMove) {
    double timeMultiplier = 1.0; // Default
    int third = totalMoves / 3;

    if (moveIndex <= third) {
        timeMultiplier = 0.5; // Early game
    } else if (moveIndex <= 2 * third) {
        timeMultiplier = 1.0; // Middle game
    } else {
        timeMultiplier = 1.5; // Late game
    }

    return baseTimePerMove * timeMultiplier;
}
class MinMaxPlayer :public Player{
    Minimax m;
    int depth;
public:
    MinMaxPlayer(int p,unique_ptr<BaseEvaluator> E,int d):Player(p),m(p, std::move(E)),depth(d){
    }
    Move playMove(Reversi board, double t){
        double a,b;

        int totalCount=BoardHelper::totalDiscCount(board);
        int moveIndex=totalCount/2+1;
        int movesLeft=(N*N-(totalCount))/2;

        double timeForMove= adjustTimeForMove(moveIndex,(N*N)/2,(t-5)/movesLeft);
        return m.Solve(board,timeForMove);

//    }else{
//            if(t<35)
//                return m.Solve(board,3,true);
//            else if(BoardHelper::totalDiscCount(board)<30)
//                return m.Solve(board,4,true);
//            else
//                return m.Solve(board,depth,true);
//        }
//        cout<<"T"
//        if(t<35)
//            return m.Solve(board,3);
//        else if(BoardHelper::totalDiscCount(board)<30)
//            return m.Solve(board,3);
//        else
//            return m.Solve(board,depth);
    }
};


#endif //CSCI_561_HW2_MINMAXPLAYER_H
