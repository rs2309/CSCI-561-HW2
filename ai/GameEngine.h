//
// Created by Rutvik Shah on 2/26/24.
//

#ifndef CSCI_561_HW2_GAMEENGINE_H
#define CSCI_561_HW2_GAMEENGINE_H
#include "BoardHelper.h"
#include "Player.h"

class GameEngine {
public:
    Reversi board;
    vector<double> time_remaining={0.0,0.0};
    bool game_over;
    std::vector<unique_ptr<Player>> players;
    int current_player;
    int moveCount;

    GameEngine(std::unique_ptr<Player> OPlayer, std::unique_ptr<Player> XPlayer){

        game_over= false;
        tie(board,time_remaining[0], time_remaining[1],current_player)=BoardHelper::getStartBoard();
        players.push_back(std::move(OPlayer));
        players.push_back(std::move(XPlayer));
    }

    double run(){
        moveCount=0;
        while(!BoardHelper::isGameFinished(board) && !game_over){
            if(BoardHelper::hasAnyMoves(board,current_player)){
                auto start = chrono::steady_clock::now();
                auto result = players[current_player]->playMove(board,time_remaining[current_player]);

                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                time_remaining[current_player] -= std::chrono::duration<double>(diff).count();



                if (BoardHelper::isValidMove(board,current_player,result) ){
                    board=BoardHelper::performMove(board,result,current_player);
                    moveCount++;
                }
                else {
                    cout<<current_player<<" lost the game early."<<endl;
                    break;
                }
                cout << current_player << ", at position:"<< (char)(97+result.y)<<result.x+1<< endl;
                cout <<chrono::duration <double, milli> (diff).count() << " ms" << endl;
            }

            current_player=1-current_player;
            cout<<time_remaining[0]<<" "<<time_remaining[1]<<endl;
            BoardHelper::printBoardState(board);
//            cout<<endl;
        }
        cout<<BoardHelper::getWinner(board,time_remaining[0], time_remaining[1],true)<<endl;


        return calculateStats();
    }
    double calculateStats() {
        double fitness = 0.0;
        // Assume we have functions or methods to calculate or retrieve these metrics
        int winner = BoardHelper::getWinner(board,time_remaining[0], time_remaining[1],true); // Simplified; actual implementation may vary
        int p1,p2;
        tie(p1,p2)=BoardHelper::playerDiscCounts(board);
        int movesPlayed = moveCount;
        double timeUsed = 300-time_remaining[0];


        if (winner == players[0]->current_player) {
            cout<<"GA won"<<endl;
            fitness += 100; // Base score for winning
            fitness += (p1-p2) * 25; // Add weighted margin of victory
            fitness -= movesPlayed * 5; // Penalize longer games
            fitness -= timeUsed*5;
            // Adjust for time efficiency, etc.
        } else {
            // Adjust fitness based on how close the game was or other considerations for a loss
            cout<<"Default won"<<endl;
            fitness -= 100; // Base score for winning
            fitness -= (p2-p1) * 25; // Add weighted margin of victory
            fitness -= movesPlayed * 5;
            fitness -= timeUsed*5;
        }

        return fitness;
    }

};


#endif //CSCI_561_HW2_GAMEENGINE_H
