#include <time.h>
#include <string> 
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "Board.h"

// Function to get a random valid move from the available moves
std::pair<int, int> getRandomMove(const Board& board) {
    auto moves = board.validMoves();
    if (moves.empty()) {
        return {-1, -1}; // No valid moves
    }
    
    // Generate a random index
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, moves.size() - 1);
    int randomIndex = distrib(gen);
    
    return moves[randomIndex];
}

int main(int argc, char **argv) {
    // Set the number of iterations to test
    const int NUM_ITERATIONS = 100000;
    
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create initial board
    Board board;
    
    // Vector to store individual move times
    std::vector<double> moveTimes;
    moveTimes.reserve(NUM_ITERATIONS);
    
    int totalMoves = 0;
    
    while (!board.isGameOver() && totalMoves < NUM_ITERATIONS) {
        // Start timing for this individual move
        auto moveStartTime = std::chrono::high_resolution_clock::now();
        
        auto move = getRandomMove(board);
        board.step(move.first, move.second);
        
        // Stop timing for this individual move
        auto moveEndTime = std::chrono::high_resolution_clock::now();
        auto moveDuration = std::chrono::duration_cast<std::chrono::microseconds>(moveEndTime - moveStartTime);
        
        // Store the time in microseconds
        moveTimes.push_back(moveDuration.count());
        totalMoves++;
    }
    
    // Calculate statistics
    double totalTime = 0;
    double minTime = moveTimes[0];
    double maxTime = moveTimes[0];
    
    for (double time : moveTimes) {
        totalTime += time;
        minTime = std::min(minTime, time);
        maxTime = std::max(maxTime, time);
    }
    
    double averageTime = totalTime / totalMoves;
    
    // Convert to different units for readability
    double averageTimeMs = averageTime / 1000.0;
    double averageTimeNs = averageTime * 1000.0;
    
    // Print results
    std::cout << "=== Move Time Analysis ===" << std::endl;
    std::cout << "Total moves processed: " << totalMoves << std::endl;
    std::cout << "Game completed: " << (board.isGameOver() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Timing Results ===" << std::endl;
    std::cout << "Average time per move: " << averageTime << " microseconds" << std::endl;
    std::cout << "Average time per move: " << averageTimeMs << " milliseconds" << std::endl;
    std::cout << "Average time per move: " << averageTimeNs << " nanoseconds" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Min time per move: " << minTime << " microseconds" << std::endl;
    std::cout << "Max time per move: " << maxTime << " microseconds" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Moves per second: " << (1000000.0 / averageTime) << std::endl;
    
    return 0;
}