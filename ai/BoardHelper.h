//
// Created by Rutvik Shah on 2/14/24.
//
#include <cstdlib>

#include<iostream>
#include<fstream>
using namespace std;
#define N 12
typedef vector<vector<int>> Reversi;

#ifndef CSCI_561_HW2_BOARDHELPER_H
#define CSCI_561_HW2_BOARDHELPER_H

const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
struct Move{
    int x,y;
    bool operator==(const Move& other) const {
        return x == other.x && y == other.y;
    }
};

class BoardHelper {
public:
    bool static isValid(int x, int y) {
        return x >= 0 && x < N && y >= 0 && y < N;
    }
    bool static checkDirection(Reversi board, int x, int y, int player, int dir) {
        int opp = 1 - player; // Assuming player is either 0 or 1
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        bool hasOpponentDisk = false;

        while (isValid(nx, ny) && board[nx][ny] == opp) {
            nx += dx[dir];
            ny += dy[dir];
            hasOpponentDisk = true;
        }

        return isValid(nx, ny) && board[nx][ny] == player && hasOpponentDisk;
    }

    //Move Heuristics
    bool static isCorner(int x, int y) {
        return (x == 0 || x == N - 1) && (y == 0 || y == N - 1);
    }
    bool static isSafeEdge(int x, int y) {
        return (x == 0 || x == N - 1 || y == 0 || y == N - 1) &&
               !isCorner(x-1, y) && !isCorner(x+1, y) && !isCorner(x, y-1) && !isCorner(x, y+1);
    }
    bool static isAdjacentToCorner(int x, int y) {
        if ((x == 1 || x == N - 2) && (y == 0 || y == 1 || y == N - 2 || y == N - 1))
            return true;
        if ((y == 1 || y == N - 2) && (x == 0 || x == 1 || x == N - 2 || x == N - 1))
            return true;
        return false;
    }
    bool static isInnerBorder(int x,int y){
        if(x==1 || x==N-1 || y==1 || y==N-1)
            return true;
        return false;
    }
    int static scoreMove(const Move& move, Reversi& board, int player) {
        int x = move.x, y = move.y;
        int score = 25;

        if (isCorner(x, y)) score += 100; // Highest priority
        else if (isSafeEdge(x, y)) score += 75;
        if(isInnerBorder(x,y)) score-=50;
        if (isAdjacentToCorner(x, y)) score -= 50; // Penalize moves that might give opponent a corner
//    if (isNextToStableDisk(x, y, board, player)) score += 30; // Prefer moves next to stable disks

//    score += mobilityImpact(board, player, move); // Adjust based on mobility impact

        return score;
    }
    void static sortMoves(std::vector<Move>& moves,Reversi& board, int player) {
        sort(moves.begin(), moves.end(), [&board, &player](const Move& a, const Move& b) {
            return scoreMove(a,board,player) > scoreMove(b,board,player); // Sort in descending order of score
        });
    }
    bool static isValidMove(Reversi &board,int player, Move m){
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
    //Evaluator Functions
    int static hasAnyMoves(Reversi board,int player){
        return findNextMoves(board,player,false).size()>0;
    }
    pair<int,int> static playerDiscCounts(Reversi board){
        int playerDisks = 0, opponentDisks = 0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] == 0) ++playerDisks;
                else if (board[x][y] == 1) ++opponentDisks;
            }
        }
        return {playerDisks,opponentDisks};
    }
    int static totalDiscCount(Reversi board){
        int diskCount=0;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] != -1) ++diskCount;

            }
        }
        return diskCount;
    }
    bool static isGameFinished(Reversi board){
        return !(hasAnyMoves(board,0) || hasAnyMoves(board,1));
    }
    int static getWinner(Reversi board,double t1,double t2,bool force_finish=false){
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
                cout<<"O won by:"<<p1s-p2s<<endl;
                return 0;
            }else if (p1s < p2s){
                //p2 wins 1
                cout<<"X won by:"<<p2s-p1s<<endl;
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
    vector<Move> static getStableDisks(const Reversi& board, int player, Move move) {
        vector<Move> stableDiscs;
        int mi, mj;
        int oplayer=1-player; // Assuming player values are 0 and 1

        for (int dir = 0; dir < 8; ++dir) {
            vector<Move> temp;
            mi = move.x + dx[dir];
            mj = move.y + dy[dir];
            while (isValid(mi, mj) && board[mi][mj] == player) {
                temp.push_back({mi, mj});
                mi += dx[dir];
                mj += dy[dir];
            }
            // Add to stableDiscs if not already present
            for (const auto& sd : temp) {
                if (find(stableDiscs.begin(), stableDiscs.end(), sd) == stableDiscs.end()) {
                    stableDiscs.push_back(sd);
                }
            }
        }

        return stableDiscs;
    }
    vector<Move> static getFrontierSquares(const Reversi& board, int player) {
        vector<Move> frontiers;
        int oplayer = 1-player; // Assuming player values are 0 and 1

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == oplayer) {
                    for (int dir = 0; dir < 8; ++dir) {
                        int ni = i + dx[dir], nj = j + dy[dir];
                        if (isValid(ni, nj) && board[ni][nj] == -1) { // Assuming empty squares are marked as -1
                            Move pf = {ni, nj};
                            if (find(frontiers.begin(), frontiers.end(), pf) == frontiers.end()) {
                                frontiers.push_back(pf);
                            }
                        }
                    }
                }
            }
        }

        return frontiers;
    }

    //Fundamental Functions
    vector<Move> static findNextMoves(Reversi board,int player,bool prioritize_moves){
        vector<Move> legalMoves;

        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (board[x][y] != -1) continue; // Skip non-empty cells

                for (int dir = 0; dir < 8; ++dir) {
                    if (checkDirection(board, x, y, player, dir)) {
                        legalMoves.push_back({x, y});
                        break; // No need to check other directions if one is valid
                    }
                }
            }
        }

        if(prioritize_moves)
            sortMoves(legalMoves,board,player);
        return legalMoves;

    }
    vector<Move> static performMove(Reversi & board, Move move, int player) {
        vector<Move> flippedDisks;
        board[move.x][move.y] = player; // Place the disk

        for (int dir = 0; dir < 8; ++dir) { // Check all eight directions
            vector<pair<int, int>> toFlip;
            int nx = move.x + dx[dir], ny = move.y + dy[dir];

            while (isValid(nx, ny) && board[nx][ny] == 1 - player) { // Move through opponent's disks
                toFlip.push_back({nx, ny});
                nx += dx[dir];
                ny += dy[dir];
            }

            if (isValid(nx, ny) && board[nx][ny] == player) { // Confirm the flip
                for (auto& p : toFlip) {
                    board[p.first][p.second] = player; // Flip the disks
                    flippedDisks.push_back({p.first,p.second});
                }
            }
        }
        return flippedDisks;
    }
    void static undoMove(Reversi & board,vector<Move> &flippedDisks, Move move, int player){
        for (const auto& pos : flippedDisks) {
            board[pos.x][pos.y] = 1 - player; // Flip back the disks to the opponent's color.
        }
        // Remove the disk that was placed by setting the cell to empty.
        board[move.x][move.y] = -1;
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
    void static printBoardState(Reversi board){
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
