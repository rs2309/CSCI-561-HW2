//
// Created by Rutvik Shah on 2/14/24.
//

#include<iostream>
#include <algorithm>
#include<fstream>
#include <unordered_set>



using namespace std;
#define N 12
#define MAX_DEPTH 10
#define MIN_DEPTH 1
typedef vector<vector<int> > Reversi;

#ifndef CSCI_561_HW2_BOARDHELPER_H
#define CSCI_561_HW2_BOARDHELPER_H

const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

struct Move{
    int x,y;
    bool operator==(const Move& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Move& other) const {
        return x == other.x ? y < other.y : x < other.x;
    }
};
namespace std {
    template <>
    struct hash<Move> {
        size_t operator()(const Move &move) const {
            return hash<int>()(move.x) ^ hash<int>()(move.y);
        }
    };
}
class BoardHelper {
public:
    bool static isValid(int x, int y) {
        return x >= 0 && x < N && y >= 0 && y < N;
    }
    //Evaluator Functions
    ////////////////////
    int static hasAnyMoves(const Reversi& board,int player){
        return !findNextMoves(board,player,false).empty();
    }
    pair<int,int> static playerDiscCounts(const Reversi& board){
        int playerDisks = 0, opponentDisks = 0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == 0) ++playerDisks;
                else if (board[x][y] == 1) ++opponentDisks;
            }
        }
        return {playerDisks,opponentDisks};
    }
    int static getWinner(const Reversi& board,double t1,double t2,bool force_finish=false){
        if(!isGameFinished(board) && !force_finish)
            //game not finished
            return -1;
        else{
            //count stones
            pair<int,int> p= playerDiscCounts(board);
            int p1s = p.first;
            int p2s = p.second+1;

            if(p1s > p2s){
                //p1 wins 0
//                cout<<"O won by:"<<p1s-p2s<<endl;
                return 0;
            }else if (p1s < p2s){
                //p2 wins 1
//                cout<<"X won by:"<<p2s-p1s<<endl;
                return 1;
            }else{
                if(t1>t2){
                    cout<<"O won by time:"<<t1<<endl;
                    return 0;
                }else if(t1<t2){
                    cout<<"1 won by time:"<<t2<<endl;
                    return 1;
                }else{
                    cout<<"1 won by tie breaker:"<<t2<<endl;
                    return 1;
                }

            }
        }
    }
    bool static isValidMove(const Reversi& board,int player, Move m){
        auto moves=BoardHelper::findNextMoves(board,player,false);
        if(find(moves.begin(),moves.end(),m)!=moves.end()){
            return true;
        }else{
//            for(auto m: moves){
//                cout<<m.x<<"-"<<m.y<<" ";
//            }
//            cout<<endl;
//            cout<<res.x<<"-"<<res.y<<endl;
//            cout<<"Invalid Move"<<endl;
            return false;
        }
    }
    ////////////////////
    int static totalDiscCount(const Reversi& board){
        int diskCount=0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] != -1) ++diskCount;

            }
        }
        return diskCount;
    }
    ////////////////////
    bool static isGameFinished(const Reversi& board){
        return !(hasAnyMoves(board,0) || hasAnyMoves(board,1));
    }
//    bool static isValidMove(const Reversi& board,int player, Move m){
//        auto moves=BoardHelper::findNextMoves(board,player,false);
//        if(find(moves.begin(),moves.end(),m)!=moves.end()){
//            return true;
//        }else{
////            for(auto m: moves){
////                cout<<m.x<<"-"<<m.y<<" ";
////            }
////            cout<<endl;
////            cout<<res.x<<"-"<<res.y<<endl;
////            cout<<"Invalid Move"<<endl;
//            return false;
//        }
//    }
    ////////////////////
    vector<Move> static getStableDisks(const Reversi& board, int player, Move move) {
        unordered_set<Move> stableDiscsSet; // Use an unordered_set for efficient lookups and insertions

        for (auto dir:directions) {
            vector<Move> temp;
            int mi = move.x + dir.first;
            int mj = move.y + dir.second;

            while (isValid(mi, mj) && board[mi][mj] == player) {
                Move currentMove = {mi, mj};
                // Directly insert into the set, avoiding duplicates inherently
                stableDiscsSet.insert(currentMove);

                mi += dir.first;
                mj += dir.second;
            }
        }

        // Convert the set back to a vector for the return value
        vector<Move> stableDiscs(stableDiscsSet.begin(), stableDiscsSet.end());
        return stableDiscs;
    }
    bool static isFrontier(const Reversi& board, Move m, int player) {// Must be an empty square
        if (board[m.x][m.y] != player) return false;
        for(auto dir:directions) {
            int x = m.x + dir.first;
            int y = m.y + dir.second;

            // Check if the adjacent square is within bounds and empty
            if (x >= 0 && x < N && y >= 0 && y < N && board[x][y] == -1) {
                return true; // Adjacent to at least one empty square
            }
        }

        return false; // Not a frontier disk
    }

    int static dfs(int x, int y, int player, const Reversi& board, vector<std::vector<bool>>& visited) {
        if (!isValid(x, y) || visited[x][y] || board[x][y] != player) return 0;
        visited[x][y] = true;
        int size = 1;
        for (const auto& dir : directions) {
            size += dfs(x + dir.first, y + dir.second, player, board, visited);
        }
        return size;
    }
    int static isAdjacentToCornerAndCaptured(const Reversi& board, int x, int y, int player) {
        // Check for upper left corner and its adjacent cells
        if ((x == 0 && y == 1) || (x == 1 && y == 0) || (x == 1 && y == 1)) {
            return board[0][0] == player ? -1 : 1;
        }
        // Check for upper right corner and its adjacent cells
        if ((x == 0 && y == N-2) || (x == 1 && y == N-1) || (x == 1 && y == N-2)) {
            return board[0][N-1] == player ? -1 : 1;
        }
        // Check for lower left corner and its adjacent cells
        if ((x == N-2 && y == 0) || (x == N-1 && y == 1) || (x == N-2 && y == 1)) {
            return board[N-1][0] == player ? -1 : 1;
        }
        // Check for lower right corner and its adjacent cells
        if ((x == N-2 && y == N-2) || (x == N-2 && y == N-1) || (x == N-1 && y == N-2)) {
            return board[N-1][N-1] == player ? -1 : 1;
        }

        // If not adjacent to any corner
        return 0;
    }


    vector<Move> static findNextMoves(const Reversi& board,int player,bool prioritize_moves){
        vector<Move> legalMoves;
        unordered_set<Move> checkedMoves;
        // Assuming a square board
        int opponent = 1-player;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == player) {
                    for (auto& dir : directions) {
                        int x = i + dir.first, y = j + dir.second;
                        bool opponentPieceFound = false;
                        while (x >= 0 && x < N && y >= 0 && y < N) {
                            if (board[x][y] == opponent) {
                                opponentPieceFound = true;
                            } else if (board[x][y] == -1) {
                                if (opponentPieceFound && checkedMoves.insert({x, y}).second) {
                                    legalMoves.push_back({x, y});
                                }
                                break;
                            } else {
                                break;
                            }
                            x += dir.first;
                            y += dir.second;
                        }
                    }
                }
            }
        }

        return legalMoves;

    }
    static void performMove(Reversi&  board, Move move, int player) {
        board[move.x][move.y] = player; // Place the disk

        for (auto dir:directions) { // Check all eight directions
            vector<pair<int, int>> toFlip;
            int nx = move.x + dir.first, ny = move.y + dir.second;

            while (isValid(nx, ny) && board[nx][ny] == 1 - player) { // Move through opponent's disks
                toFlip.push_back({nx, ny});
                nx += dir.first;
                ny += dir.second;
            }

            if (isValid(nx, ny) && board[nx][ny] == player) { // Confirm the flip
                for (auto& p : toFlip) {
                    board[p.first][p.second] = player; // Flip the disks
                }
            }
        }
    }

    tuple<Reversi,double,double,int> static getStartBoard(bool fromInputFile=true){
        cout<<"Current Board Size: "<<N<<endl;

        ifstream inputFile("input.txt");
        string p;
        double t1,t2;

        inputFile>>p;
        inputFile>>t1>>t2;
        cout<<p<<" "<<t1<<" "<<t2<<endl;
        int current_player=(p=="X")?1:0;
        Reversi board(N,vector<int>(N,-1));

        int i=-1,j=0;
        char ch;
        while(inputFile.get(ch)){

            if(ch=='\n'){
                i++;
                j=0;
            }else{
//            cout<<ch<<" "<<i<<" "<<j<<endl;
                if(ch=='X')
                    board[i][j]=1;
                else if(ch=='O')
                    board[i][j]=0;
                j++;
            }


        }
        inputFile.close();

        return {board,t1,t2,current_player};
    }
    void static printBoardState(const Reversi& board){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                string s=board[i][j]==1?"X":(board[i][j]==0?"O":".");
                cout<<s<<" ";
            }
            cout<<endl;
        }
    }

};


#endif //CSCI_561_HW2_BOARDHELPER_H
