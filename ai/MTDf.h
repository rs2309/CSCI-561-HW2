////
//// Created by Rutvik Shah on 2/16/24.
////
//
//#ifndef CSCI_561_HW2_MTDF_H
//#define CSCI_561_HW2_MTDF_H
//
//#include "BoardHelper.h" // Assumed to exist from your snippet
////#include "Evaluator.h"   // Assumed to exist from your snippet
//#include "RTEvaluator.h"
//#include <random>
//
//
//struct TTEntry {
//    int lowerbound;
//    int upperbound;
//};
//
//typedef uint64_t BoardHash; // Simplified; adjust according to your hashing function
//using ZobristTableType = std::array<std::array<std::array<BoardHash, 3>, N>, N>;
//ZobristTableType generateZobristTable() {
//    std::random_device rd;
//    std::mt19937_64 eng(rd());
//    std::uniform_int_distribution<BoardHash> distr;
//
//    ZobristTableType zobristTable;
//    for (int row = 0; row < N; ++row) {
//        for (int col = 0; col < N; ++col) {
//            for (int piece = 0; piece < 3; ++piece) {
//                zobristTable[row][col][piece] = distr(eng);
//            }
//        }
//    }
//
//    return zobristTable;
//}
//BoardHash computeHash(const Reversi board, const ZobristTableType& zobristTable) {
//    BoardHash hash = 0;
//    for (int row = 0; row < N; ++row) {
//        for (int col = 0; col < N; ++col) {
//            int piece = board[row][col]+1; // Assuming 0 for empty, 2 for black, 1 for white
//            hash ^= zobristTable[row][col][piece];
//        }
//    }
//    return hash;
//}
//
//
//
//
//
//class MTDf {
//private:
//    unordered_map<BoardHash, TTEntry> transpositionTable;
//    RTEvaluator evaluator;
//    int current_player;
//    ZobristTableType zobristTable;
//
//    void store(BoardHash hash, const TTEntry& data) {
//        transpositionTable[hash] = data;
//    }
//
//    bool retrieve(BoardHash hash, TTEntry& data) {
//        auto it = transpositionTable.find(hash);
//        if (it != transpositionTable.end()) {
//            data = it->second;
//            return true;
//        }
//        return false;
//    }
//
//    int AlphaBetaWithMemory(Reversi& board,int alpha, int beta, int d, bool maximizingPlayer, int player) {
//        BoardHash hash= computeHash(board,zobristTable);
//        TTEntry ttEntry;
//        if (retrieve(hash, ttEntry)) { // Transposition table lookup
//            if (ttEntry.lowerbound >= beta) return ttEntry.lowerbound;
//            if (ttEntry.upperbound <= alpha) return ttEntry.upperbound;
//            alpha = std::max(alpha, ttEntry.lowerbound);
//            beta = std::min(beta, ttEntry.upperbound);
//        }
//
//        if (d == 0 || BoardHelper::isGameFinished(board)) { // Leaf node
//            return evaluator.eval(board,current_player);
//        }
//
//        int g;
//        if (maximizingPlayer) {
//            g = std::numeric_limits<int>::min();
//            int a = alpha; // Save original alpha value
//            for (auto move : BoardHelper::findNextMoves(board,player,true) ) {
//                vector<Move> flippedDisks = BoardHelper::performMove(board, move, player);
//                g = std::max(g, AlphaBetaWithMemory(board, a, beta, d - 1, false, 1-player));
//                BoardHelper::undoMove(board, flippedDisks, move, player);
//                a = std::max(a, g);
//                if (g >= beta) break; // Beta cutoff
//            }
//        } else { // n is a MINNODE
//            g = std::numeric_limits<int>::max();
//            int b = beta; // Save original beta value
//            for (auto move : BoardHelper::findNextMoves(board,player,true)) {
//                vector<Move> flippedDisks = BoardHelper::performMove(board, move, player);
//                g = std::min(g, AlphaBetaWithMemory(board, alpha, b, d - 1,true,1-player));
//                BoardHelper::undoMove(board, flippedDisks, move, player);
//                b = std::min(b, g);
//                if (g <= alpha) break; // Alpha cutoff
//            }
//        }
//
//        // Update transposition table
//        if (g <= alpha) {
//            // Fail low result implies an upper bound
//            transpositionTable[hash].upperbound = g;
//        } else if (g >= beta) {
//            // Fail high result implies a lower bound
//            transpositionTable[hash].lowerbound = g;
//        } else {
//            // Exact value
//            transpositionTable[hash].lowerbound = g;
//            transpositionTable[hash].upperbound = g;
//        }
//        store(hash, transpositionTable[hash]);
//
//        return g;
//    }
//
//    int mtdf(Reversi board, int guess, int depth) {
//        int g = guess;
//        int beta;
//        int upperBound = std::numeric_limits<int>::max();
//        int lowerBound = std::numeric_limits<int>::min();
//
//        while (lowerBound < upperBound) {
//            if (g == lowerBound) {
//                beta = g + 1;
//            } else {
//                beta = g;
//            }
//
//            g = AlphaBetaWithMemory(board, beta - 1, beta, depth,false,1-current_player);
//
//            if (g < beta) {
//                upperBound = g;
//            } else {
//                lowerBound = g;
//            }
//        }
//        return g;
//    }
//
//public:
//    MTDf(int player) : current_player(player) {
//        zobristTable = generateZobristTable();
//    }
//
//    Move Solve(Reversi& board, int depth) {
//        int guess = 0;
//        Move bestMove = {-1, -1}; // You need to define how to choose bestMove based on your game's logic
//
//        std::vector<Move> possibleMoves = BoardHelper::findNextMoves(board,current_player,true);
//        int bestScore = std::numeric_limits<int>::min();
//
//        for (const auto& move : possibleMoves) {
//            // Apply move to current_state to get a new state
//            vector<Move> flippedDisks = BoardHelper::performMove(board, move, current_player);
//            // Use MTD(f) to evaluate newState
//            int initialGuess = 0; // This could be refined based on context
//            int score = mtdf(board, initialGuess, depth);
//            BoardHelper::undoMove(board, flippedDisks, move, current_player);
//            // Update bestScore and bestMove if this move is better
//            if (score > bestScore) {
//                bestScore = score;
//                bestMove = move;
//            }
//
//        }
//
//        return bestMove;
//
//        cout << "Best Move: (" << bestMove.x << "," << bestMove.y << ") with score " << guess << endl;
//        return bestMove;
//    }
//};
//
//
//#endif //CSCI_561_HW2_MTDF_H
