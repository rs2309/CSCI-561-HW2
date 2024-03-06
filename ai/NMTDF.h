//
// Created by Rutvik Shah on 2/25/24.
//

#ifndef CSCI_561_HW2_NMTDF_H
#define CSCI_561_HW2_NMTDF_H
#include "BoardHelper.h" // Assumed to exist from your snippet
#include "BaseEvaluator.h"

typedef std::string BoardHash;


template<> struct hash<Move> {
    std::size_t operator()(const Move& move) const noexcept {
        return std::hash<int>()(move.x) ^ std::hash<int>()(move.y);
    }
};

// Define a hash function for std::pair<std::vector<std::vector<int>>, Move>
struct MyPairHash {
    std::size_t operator()(const std::pair<std::vector<std::vector<int>>, Move>& p) const noexcept {
        std::size_t hash = 0;
        // Hash the vector of vectors
        for (const auto& vec : p.first) {
            for (int i : vec) {
                hash ^= std::hash<int>()(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        // Combine hash of Move
        hash ^= std::hash<Move>()(p.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};
BoardHash hashBoard(Reversi board,int player) {
    string hash=string(5,player?'X':'O');
    for (const auto& row : board) {
        for (int cell : row) {
            // Convert each cell value to a character and append to the hash string
            // Assuming cell values are -1, 0, and 1, we shift them to '0', '1', '2' for simplicity
            hash += std::to_string(cell+1);
        }
    }
    return hash;
}

class TranspositionTableEntry {
public:
    int value;
    string flag;
    int depth;
};

class  NMTDF{
    unordered_map<BoardHash , TranspositionTableEntry> transpositionTable;
    unordered_map<pair<Reversi,Move> , int, MyPairHash> moveOrder;
    Move bestMove;
    int currentDepth;
    unique_ptr<BaseEvaluator> r;
    int current_player;

    int alphaBetaWithMemory(Reversi board, int depth, int alpha, int beta, int player) {
//        cout<<"RSSS"<<endl;
        BoardHash hash = hashBoard(board,current_player);
        if (transpositionTable.find(hash) != transpositionTable.end() && depth <= transpositionTable[hash].depth) {
            auto& entry = transpositionTable[hash];
            if (entry.flag == "EXACT") return entry.value;
            else if (entry.flag == "LOWERBOUND") alpha = std::max(alpha, entry.value);
            else if (entry.flag == "UPPERBOUND") beta = std::min(beta, entry.value);
            if (alpha >= beta) return entry.value;
        }

        if (depth == 0 || BoardHelper::isGameFinished(board)) {
            return r->eval(board,current_player);
        }

        int bestValue = std::numeric_limits<int>::min();
        auto moves = BoardHelper::findNextMoves(board, player,false);

        if (moves.empty()) {
            return -alphaBetaWithMemory(board, depth - 1, -beta, -alpha, 1-player);
        }
        // Sort moves based on previously evaluated scores (move ordering)
        std::sort(moves.begin(), moves.end(), [&](Move a, Move b) {
            return moveOrder[{board,a}]>moveOrder[{board,b}];
        });
//        cout<<"RSSS"<<endl;
        for (auto& move : moves) {
            int value = -alphaBetaWithMemory(BoardHelper::performMove(board, move, player), depth - 1, -beta, -alpha, 1-player);
            if (value > bestValue) {
                bestValue = value;
                if (depth == currentDepth) {
                    bestMove = move; // Assuming GameState can represent moves directly
                }
            }
            if (value >= beta) {
                transpositionTable[hash] = {value, "LOWERBOUND", depth};
                moveOrder[{board,move}] = value;
                return value;
            }
            if (value > alpha) {
                alpha = value;
                transpositionTable[hash] = {value, "EXACT", depth};
                moveOrder[{board,move}] = value;
            } else {
                transpositionTable[hash] = {alpha, "UPPERBOUND", depth};
            }
        }
        return bestValue;
    }

    int mtdf(Reversi board, int firstGuess, int depth) {
        int g = firstGuess;
        int beta;
        int upperBound = std::numeric_limits<int>::max();
        int lowerBound = std::numeric_limits<int>::min();

        while (lowerBound < upperBound) {
            if (g == lowerBound) {
                beta = g + 1;
            } else {
                beta = g;
            }

            g = alphaBetaWithMemory(board,  depth,beta - 1, beta,current_player);

            if (g < beta) {
                upperBound = g;
            } else {
                lowerBound = g;
            }
        }
        return g;
    }

public:
    NMTDF(int player,unique_ptr<BaseEvaluator> E):current_player(player){
        r=std::move(E);
    }
    Move iterativeDeepening(Reversi& initialState, int maxDepth) {
        int guess = 0;
        for (int depth = 1; depth <= maxDepth; ++depth) {
            currentDepth = depth;
            guess = mtdf(initialState, guess, depth);
            // bestMove is updated within alphaBetaWithMemory
        }
        return bestMove;
    }
};


#endif //CSCI_561_HW2_NMTDF_H
