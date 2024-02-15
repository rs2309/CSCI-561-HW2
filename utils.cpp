//
// Created by Rutvik Shah on 2/13/24.
//
#include<iostream>
using namespace std;
#include <fstream>
#include <algorithm>
#define N 12

const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int pp=0,fp=0;

// Generate a simple hash for a board state
typedef std::string BoardHash;
BoardHash hashBoard(const std::vector<std::vector<int>>& board) {
    string hash="";
    for (const auto& row : board) {
        for (int cell : row) {
            // Convert each cell value to a character and append to the hash string
            // Assuming cell values are -1, 0, and 1, we shift them to '0', '1', '2' for simplicity
            hash += std::to_string(cell+1);
        }
    }
    return hash;
}
struct TTEntry {
    int depth;
    int value;
    pair<int, int> bestMove;
};

unordered_map<BoardHash, TTEntry> transpositionTable;

bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}
bool checkDirection(vector<vector<int>> board, int x, int y, int player, int dir) {
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


bool isCorner(int x, int y) {
    return (x == 0 || x == N - 1) && (y == 0 || y == N - 1);
}
bool isSafeEdge(int x, int y) {
    return (x == 0 || x == N - 1 || y == 0 || y == N - 1) &&
           !isCorner(x-1, y) && !isCorner(x+1, y) && !isCorner(x, y-1) && !isCorner(x, y+1);
}
bool isAdjacentToCorner(int x, int y) {
    if ((x == 1 || x == N - 2) && (y == 0 || y == 1 || y == N - 2 || y == N - 1))
        return true;
    if ((y == 1 || y == N - 2) && (x == 0 || x == 1 || x == N - 2 || x == N - 1))
        return true;
    return false;
}
bool isInnerBorder(int x,int y){
    if(x==1 || x==N-1 || y==1 || y==N-1)
        return true;
    return false;
}
int mobilityImpact(vector<vector<int>>& board, int player, const pair<int, int>& move);
bool isNextToStableDisk(int x, int y, const vector<vector<int>>& board, int player) {
    const int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1},
                                  {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};


    for (const auto& dir : directions) {
        int nx = x + dir[0];
        int ny = y + dir[1];

        // Check bounds
        if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;

        // For simplicity, assume a disk is stable if it's in a corner or on an edge.
        // This is a simplification and might not accurately reflect all stable disks.
        if (board[nx][ny] == player && (isCorner(nx, ny) || isSafeEdge(nx, ny))) {
            return true;
        }
    }

    return false;
}
int scoreMove(const std::pair<int, int>& move, vector<vector<int>>& board, int player) {
    int x = move.first, y = move.second;
    int score = 0;

    if (isCorner(x, y)) score += 100; // Highest priority
    else if (isSafeEdge(x, y)) score += 75;
    if(isInnerBorder(x,y)) score-=50;
    if (isAdjacentToCorner(x, y)) score -= 50; // Penalize moves that might give opponent a corner
//    if (isNextToStableDisk(x, y, board, player)) score += 30; // Prefer moves next to stable disks

//    score += mobilityImpact(board, player, move); // Adjust based on mobility impact

    return score;
}
void sortMoves(std::vector<std::pair<int, int>>& moves,vector<vector<int>>& board, int player) {
    std::sort(moves.begin(), moves.end(), [&board, &player](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return scoreMove(a,board,player) > scoreMove(b,board,player); // Sort in descending order of score
    });
}
vector<pair<int,int>> findNextMoves(vector<vector<int>> board,int player){
    vector<pair<int, int>> legalMoves;

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
//    for(auto it:legalMoves){
//        cout<<it.first<<" "<<it.second<<endl;
//    }
    sortMoves(legalMoves,board,player);
    return legalMoves;

}
vector<pair<int,int>> performMove(vector<vector<int>> & board, int x, int y, int player) {
    vector<pair<int,int>> flippedDisks;
    board[x][y] = player; // Place the disk

    for (int dir = 0; dir < 8; ++dir) { // Check all eight directions
        vector<pair<int, int>> toFlip;
        int nx = x + dx[dir], ny = y + dy[dir];

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
void undoMove(vector<vector<int>> & board,vector<pair<int,int>> flippedDisks, int x, int y, int player){
    for (const auto& pos : flippedDisks) {
        board[pos.first][pos.second] = 1 - player; // Flip back the disks to the opponent's color.
    }
    // Remove the disk that was placed by setting the cell to empty.
    board[x][y] = -1;
}
int mobilityImpact(vector<vector<int>>& board, int player, const pair<int, int>& move) {
    // Create a copy of the board to simulate the move
    vector<vector<int>> tempBoard = board;

    // Simulate the move
    vector<pair<int, int>> flippedDisks = performMove(tempBoard, move.first, move.second, player);

    // Calculate the number of legal moves for the player after the move
    int playerMobilityAfterMove = findNextMoves(tempBoard, player).size();

    // Calculate the number of legal moves for the opponent after the move
    int opponentMobilityAfterMove = findNextMoves(tempBoard, 1 - player).size();

    // Undo the move on the temporary board
//    undoMove(tempBoard, flippedDisks, move.first, move.second, player);

    // The mobility impact favors moves that increase the player's mobility and decrease the opponent's
    return playerMobilityAfterMove - opponentMobilityAfterMove;
}

int diskCountScore(vector<vector<int>> board,int player) {
    int playerDisks = 0, opponentDisks = 0;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            if (board[x][y] == player) ++playerDisks;
            else if (board[x][y] == 1 - player) ++opponentDisks;
        }
    }
    return playerDisks - opponentDisks;
}
int safeEdgeCountScore(vector<vector<int>> board,int player) {
    int score=0;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            if (board[x][y] == player && isSafeEdge(x,y)) ++score;
            else if (board[x][y] == 1 - player && isSafeEdge(x,y)) --score;
        }
    }
    return score*5;
}
int mobilityScore(vector<vector<int>> board,int player) {
    int playerMoves = findNextMoves(board, player).size();
    int opponentMoves = findNextMoves(board, 1 - player).size();
    return  playerMoves - opponentMoves;
}
int cornerScore(vector<vector<int>> board,int player) {
    int score = 0;
    int corners[4][2] = {{0, 0}, {0, N - 1}, {N - 1, 0}, {N - 1, N - 1}};

    for (auto& corner : corners) {
        if (board[corner[0]][corner[1]] == player) score += 1;
        else if (board[corner[0]][corner[1]] == 1 - player) score -= 1;
    }
    return score*25; // Weight corners more heavily
}
int evaluateBoard(vector<vector<int>> board,int player) {
    // Adjust weights as needed based on strategy
    int weightDiskCount = 10;
    int weightMobility = 5;
    int weightSafeEdge=50;
    int weightCorners = 1000;

    int score = 0;
    score += weightDiskCount * diskCountScore(board, player);
//    score += weightMobility * mobilityScore(board, player);
    score += weightSafeEdge * safeEdgeCountScore(board,player);
    score += weightCorners * cornerScore(board, player);
    // Include other scoring components as desired

    return score;
}
void updateTranspositionTable(string hash, int depth, int value, const pair<int, int>& bestMove) {
    auto ttEntryIt = transpositionTable.find(hash);
    if (ttEntryIt == transpositionTable.end() || ttEntryIt->second.depth <= depth) {
        // Update the transposition table only if the new information is from a deeper or equal depth search
        transpositionTable[hash] = {depth, value, bestMove};
    }
}
pair<int, pair<int, int>> alphaBeta(vector<vector<int>> board, int depth, int alpha, int beta, bool maximizingPlayer, int player) {
    pair<int, int> bestMove = {-1, -1};
    BoardHash hash = hashBoard(board);

    // Check if the board state is already in the transposition table
    auto ttEntry = transpositionTable.find(hash);
    if (ttEntry != transpositionTable.end() && ttEntry->second.depth >= depth) {
        // Use the cached value if it's valid for the current or deeper search
//        cout<<"Returning Hash"<<depth<<endl;
        return {ttEntry->second.value, ttEntry->second.bestMove};
    }

    if (depth == 0) {
        int eval = evaluateBoard(board, player);
        updateTranspositionTable(hash, depth, eval, bestMove); // Here, bestMove might be an invalid move indicating a leaf node
        return {eval, bestMove};
    }

    auto moves = findNextMoves(board, player);

    if (moves.empty()) {
        int eval = evaluateBoard(board, player);
        updateTranspositionTable(hash, depth, eval, bestMove); // Here, bestMove might be an invalid move indicating a leaf node
        return {eval, bestMove};
    }

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();
        for (auto& move : moves) {
            vector<pair<int,int>> flippedDisks = performMove(board, move.first, move.second, player);
            auto result = alphaBeta(board, depth - 1, alpha, beta, false, 1 - player);
            undoMove(board, flippedDisks, move.first, move.second, player);
            int eval=result.first;
            if (eval > maxEval) {
                maxEval = eval;
                bestMove = move;
            }
            alpha = max(alpha, eval);
            if (eval >= beta) {
                pp++;
                break; // Beta cut-off
            }
        }
        updateTranspositionTable(hash, depth, maxEval, bestMove);
        return {maxEval, bestMove};
    } else {
        int minEval = numeric_limits<int>::max();
        for (auto& move : moves) {
            vector<pair<int,int>> flippedDisks = performMove(board, move.first, move.second, player);
            auto result = alphaBeta(board, depth - 1, alpha, beta, true, 1 - player);
            undoMove(board, flippedDisks, move.first, move.second, player);
            int eval = result.first;
            if (eval < minEval) {
                minEval = eval;
                bestMove = move;
            }
            beta = min(beta, eval);
            if (eval <= alpha) {
                fp++;
                break; // Alpha cut-off
            }
        }
        updateTranspositionTable(hash, depth, minEval, bestMove);
        return {minEval, bestMove};
    }

}
void printBoardState(vector<vector<int>> board){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
}
int main(){
    cout<<"Current Board Size"<<N<<endl;

    ifstream inputFile("input.txt");
    string p;
    double t1,t2;

    inputFile>>p;
    inputFile>>t1>>t2;
    cout<<p<<" "<<t1<<" "<<t2<<endl;
    int current_player=(p=="X")?1:0;
    vector<vector<int>> board(N,vector<int>(N,-1));

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

//    for(int i=0;i<N;i++){
//
//        for(int j=0;j<N;j++){
//            cout<<board[i][j]<<" ";
//        }
//        cout<<endl;
//    }

    int depth = 10; // Depth of the search
//    cout<<current_player<<endl;
//    vector<pair<int,int>> oppMoves=findNextMoves(board,current_player);
//    for(auto it:oppMoves){
//        cout<<it.first<<" "<<it.second<<endl;
//    }

//    t1=300.00;
//    while(findNextMoves(board,current_player).size()>0 || findNextMoves(board,current_player-1).size()>0){
//        auto start = chrono::steady_clock::now();
//        auto result = alphaBeta(board, depth, numeric_limits<int>::min(), numeric_limits<int>::max(), true, current_player);
//        cout << "Best move score: " << result.first << ", at position: (" << result.second.first << ", " << result.second.second << ")" << endl;
//        auto end = chrono::steady_clock::now();
//
//        //TIME
//        auto diff = end - start;
//        t1-= chrono::duration_cast<std::chrono::duration<double>>(diff).count();
//
//        cout << i << " "<< chrono::duration <double, milli> (diff).count() << " ms \nTime Left: "<<t1<<" s"<< endl;
//        cout<<pp<<" "<<fp<<" "<<transpositionTable.size()<<endl;
//
//        if(result.second.first!=-1 && result.second.second!=-1){
//            cout<<"Next Move: "<<(char)(97+result.second.second)<<N-result.second.first<<endl;
//            performMove(board,result.second.first,result.second.second,current_player);
//        }
//
//        vector<pair<int,int>> oppMoves=findNextMoves(board,1-current_player);
//        for(auto it:oppMoves){
//            cout<<char(97+it.second)<<N-it.first<<" ";
//        }
//        cout<<endl;
//        if(oppMoves.size()>0){
////            int y=-1;
////            char x='x';
////            while((!isValid(8-y,(int)x-97)) && (find(oppMoves.begin(),oppMoves.end(),make_pair(8-y,(int)x-97))==oppMoves.end())){
////                cin>>x>>y;
////            }
////            cout<<"Valid Move: "<<x<<y<<endl;
////            performMove(board,8-y,(int)x-97,current_player-1);
//            performMove(board,oppMoves[0].first,oppMoves[0].second,1-current_player);
//        }
//        printBoardState(board);
//
//    }

//UNCOMMENT for Submission
    auto start = chrono::steady_clock::now();
    auto result = alphaBeta(board, depth, numeric_limits<int>::min(), numeric_limits<int>::max(), true, current_player);
    cout << "Best move score: " << result.first << ", at position: (" << result.second.first << ", " << result.second.second << ")" << endl;
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


    outputFile<<(char)(97+result.second.second)<<result.second.first+1<<endl;
    outputFile.close();


    return 1;
}
