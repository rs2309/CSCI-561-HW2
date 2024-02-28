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
    std::vector<std::unique_ptr<Player>> players;
    int current_player;

    GameEngine(std::unique_ptr<Player> OPlayer, std::unique_ptr<Player> XPlayer){

        game_over= false;
        tie(board,time_remaining[0], time_remaining[1],current_player)=BoardHelper::getStartBoard();
        players.push_back(std::move(OPlayer));
        players.push_back(std::move(XPlayer));
    }

    void run(){
        int moveCount=0;
        while(!BoardHelper::isGameFinished(board) && !game_over){
            if(BoardHelper::hasAnyMoves(board,current_player)){
                auto start = chrono::steady_clock::now();
                auto result = players[current_player]->playMove(board,time_remaining[current_player]);

                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                time_remaining[current_player]-=((diff).count() / 1000.0);


                if (BoardHelper::isValidMove(board,current_player,result)){
                    BoardHelper::performMove(board,result,current_player);
                    moveCount++;
                }
                else
                    game_over=true;
                cout << current_player << ", at position:"<< (char)(97+result.y)<<result.x+1<< endl;
                cout <<chrono::duration <double, milli> (diff).count() << " ms" << endl;
            }

            current_player=1-current_player;
            BoardHelper::printBoardState(board);
            cout<<endl;
        }
        cout<<BoardHelper::getWinner(board,time_remaining[0], time_remaining[1],true)<<endl;
    }

};


#endif //CSCI_561_HW2_GAMEENGINE_H
