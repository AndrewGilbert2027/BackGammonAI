#include "Board.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <inttypes.h>
#include <vector>

Board::Board() {
    reset();
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
}

Board::Board(const int boardState[31]) {
    // Initialize player positions from the provided board state
    for (int i = 0; i < 24; ++i) {
        if (boardState[i] < 0) {
            player1[i] = 0; 
            player2[i] = (uint8_t)(-boardState[i]); 
        } else {
            player1[i] = (uint8_t)(boardState[i]); // Player 1's pieces
            player2[i] = 0; // Player 2 has no pieces on this position
        }
    }
    bar1 = (uint8_t)boardState[24]; // Number of pieces on the bar for player 1
    bar2 = (uint8_t)boardState[25]; // Number of pieces on the bar for player 2
    // Initialize dice and current player
    std::fill(std::begin(dice), std::end(dice), 0);
    for (int i = 26; i < 30; ++i) {
        if (boardState[i] != -1) { // Check if the die is valid
            dice[boardState[i] - 1] += 1; // Increment the count for the rolled die
        }
    }
    currentPlayer = boardState[30]; // Set the current player
}

Board::~Board() {
    // Destructor does not need to do anything special for this implementation
}

Board::Board(const Board& other) {
    // Deep copy of the board state
    std::memcpy(this->player1, other.player1, sizeof(this->player1));
    std::memcpy(this->player2, other.player2, sizeof(this->player2));
    std::memcpy(this->dice, other.dice, sizeof(this->dice));
    this->bar1 = other.bar1;
    this->bar2 = other.bar2;
    this->currentPlayer = other.currentPlayer;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) { // Check for self-assignment
        // Deep copy of the board state
        std::memcpy(this->player1, other.player1, sizeof(this->player1));
        std::memcpy(this->player2, other.player2, sizeof(this->player2));
        std::memcpy(this->dice, other.dice, sizeof(this->dice));
        this->bar1 = other.bar1;
        this->bar2 = other.bar2;
        this->currentPlayer = other.currentPlayer;
    }
    return *this;
}

void Board::reset() {
    // Initialize player positions
    std::fill(std::begin(player1), std::end(player1), 0);
    std::fill(std::begin(player2), std::end(player2), 0);

    player1[0] = 2; // Player 1 starts with 2 pieces on position 0
    player2[23] = 2; // Player 2 starts with 2 pieces on position 23

    player1[11] = 5; // Player 1 has 5 pieces on position 11
    player2[12] = 5; // Player 2 has 5 pieces on position 12

    player1[16] = 3; // Player 1 has 3 pieces on position 16
    player2[7] = 3; // Player 2 has 3 pieces on position 7

    player1[18] = 5; // Player 1 has 5 pieces on position 18
    player2[5] = 5; // Player 2 has 5 pieces on position 5

    // Reset dice values
    std::fill(std::begin(dice), std::end(dice), 0); // Set all dice to 0 (not rolled)
    
    // Simulate the initial dice roll to determine the starting player
    int player1_dice = -1;
    int player2_dice = -1;
    do {
        player1_dice = std::rand() % 6 + 1; // Random number between 1 and 6
        player2_dice =  std::rand() % 6 + 1; // Random number between 1 and 6
    } while (player1_dice == player2_dice); // Ensure the dice are not equal
    
    currentPlayer = (player1_dice > player2_dice) ? 1 : -1; // Determine the starting player based on dice values
    dice[player1_dice - 1] = 1;                             // Mark the rolled die for player 1
    dice[player2_dice - 1] = 1;                             // Mark the rolled die for player 2
    bar1 = 0; // Player 1 has no pieces on the bar
    bar2 = 0; // Player 2 has no pieces on the bar
}

void Board::rollDice() {
    // Roll the dice for the current player
    std::fill(std::begin(dice), std::end(dice), 0); // Reset dice to 0 (not available)
    int dice1 = std::rand() % 6 + 1; // Random number between 1 and 6
    int dice2 = std::rand() % 6 + 1; // Random number between 1 and 6
    if (dice1 == dice2) {
        // If the dice are equal, it's a double roll
        dice[dice1 - 1] = 4; // 4 available moves for doubles
    } else {
        // Store the rolled values in the first two slots
        dice[dice1 - 1] = 1; 
        dice[dice2 - 1] = 1;
    }
}

void Board::changePlayer() {
    // Change the current player
    currentPlayer = (currentPlayer == 1) ? -1 : 1; // Toggle between player 1 and player 2 (ternary operator faster than multiplication)
    rollDice(); // Roll the dice for the new player
}

int Board::getCurrentPlayer() const {
    return currentPlayer; // Return the current player
}

void Board::step(int from, int distance) {
    move(from, distance); // Move the piece from the specified position
    if (std::all_of(dice, dice + 6, [](uint8_t d) { return d == 0; })) {
        // If all dice have been used, change the player and roll new dice
        changePlayer();
        rollDice();
    }
    else {
        // If there are still dice available, check to see if the player can make any valid moves
        auto moves = validMoves();
        if (moves.empty()) {
            // If no valid moves are available, change the player and roll new dice
            changePlayer();
            rollDice();
        }
    }
}

void Board::move(int from, int distance) {
    (currentPlayer == 1) ? handlePlayer1Move(from, distance) : handlePlayer2Move(from, distance);
}


// std::vector<std::pair<int, int>> Board::validMoves() const {
//     return (currentPlayer == 1) ? validMovesPlayer1() : validMovesPlayer2();
// }

std::vector<std::pair<int, int>> Board::validMoves() const {
    return (currentPlayer == 1) ? validMovesDFS1() : validMovesDFS2();
}



//////////////////////////////////////////////////
//// HELPER FUNCTIONS FOR VALID MOVES ////
//////////////////////////////////////////////////

/**
 * @brief Returns the minimum amount of die left that can be possible for player 1 given a board. 
 * This function recursively checks all valid moves for player 1 and returns the minimum die left after making those moves.
 * @param b The current board state.
 * @return The minimum die left for player 1 after making valid moves.
 */
int getMinimumDieLeft1(Board b) {
    auto moves = b.validMovesPlayer1(); 
    if (moves.empty()) {
        return b.diceLeft(); 
    }
    int minDie = 7; // Initialize to a value greater than the amount of dice available
    for (const auto& move : moves) {
        int c = getMinimumDieLeft1(b.stepReturn(move.first, move.second)); // Recursively check the next state
        minDie = std::min(minDie, c); // Update the minimum die found
    }
    return minDie; // Return the minimum die found
}


std::vector<std::pair<int, int>> Board::validMovesDFS1() const {
    auto moves = validMovesPlayer1(); // Get all valid moves for player 1
    if (moves.empty()) {
        return moves; // Return empty if no valid moves
    }

    std::vector<int> diceLeftPossible(moves.size(), 8); // Set dice left to a value greater than possible
    int dieLeftMin = 7; // Initialize minimum die left greater than the maximum possible (4) 
    for (size_t i = 0; i < moves.size(); ++i) {
        Board b = stepReturn(moves[i].first, moves[i].second); // Get the board state after the move
        diceLeftPossible[i] = getMinimumDieLeft1(b); // Get the minimum die used in the next state
        dieLeftMin = std::min(dieLeftMin, diceLeftPossible[i]); // Update the minimum die found
    }
    std::vector<std::pair<int, int>> validMoves;
    if (dieLeftMin == 0) { // Return any move that can use all possible die
        for (size_t i = 0; i < moves.size(); ++i) {
            if (diceLeftPossible[i] == 0) { // If the move uses the minimum die found
                validMoves.push_back(moves[i]); 
            }
        }
        return validMoves; // Return valid moves if minimum die is 0
    }

    // If there are no moves that use all possible dice, we can only choose moves that use the highest possible die. 
    int maxDice = 0; // Variable to track the maximum die used (set to 0 (impossible))
    for (size_t i = 0; i < moves.size(); ++i) {
        if (diceLeftPossible[i] == dieLeftMin) { // If the move uses the minimum die found
            maxDice = std::max(maxDice, moves[i].second); // Update the maximum die used
        }
    }

    for (size_t i = 0; i < moves.size(); ++i) {
        if (diceLeftPossible[i] == dieLeftMin && moves[i].second == maxDice) { // If the move uses the most amount of die and has highest dice face. 
            validMoves.push_back(moves[i]); 
        }
    }
    return validMoves; // Return valid moves
}

/**
 * @brief Returns the minimum amount of die left that can be possible for player 1 given a board. 
 * This function recursively checks all valid moves for player 1 and returns the minimum die left after making those moves.
 * @param b The current board state.
 * @return The minimum die left for player 1 after making valid moves.
 */
int getMinimumDieLeft2(Board b) {
    auto moves = b.validMovesPlayer2(); 
    if (moves.empty()) {
        return b.diceLeft(); 
    }
    int minDie = 7; // Initialize to a value greater than the amount of dice available
    for (const auto& move : moves) {
        int c = getMinimumDieLeft2(b.stepReturn(move.first, move.second)); // Recursively check the next state
        minDie = std::min(minDie, c); // Update the minimum die found
    }
    return minDie; // Return the minimum die found
}



/**
 * @brief Returns all valid moves for player 2 using a depth-first search approach.
 * This method checks player 2's pieces and the rolled dice to determine all possible moves.
 * This method is used to explore all possible moves for player 2 and make sure it only returns moves
 * that are valid according to the game rules (using moves that allow the maximum amount of dice to be used).
 * @return A vector of pairs, where each pair contains the starting position and the distance to move.
 */
std::vector<std::pair<int, int>> Board::validMovesDFS2() const {
    auto moves = validMovesPlayer2(); // Get all valid moves for player 1
    if (moves.empty()) {
        return moves; // Return empty if no valid moves
    }

    std::vector<int> diceLeftPossible(moves.size(), 8); // Set dice left to a value greater than possible
    int dieLeftMin = 7; // Initialize minimum die left greater than the maximum possible (4) 
    for (size_t i = 0; i < moves.size(); ++i) {
        Board b = stepReturn(moves[i].first, moves[i].second); // Get the board state after the move
        diceLeftPossible[i] = getMinimumDieLeft2(b); // Get the minimum amount of die left
        dieLeftMin = std::min(dieLeftMin, diceLeftPossible[i]); 
    }
    std::vector<std::pair<int, int>> validMoves;
    if (dieLeftMin == 0) { // Return any move that can use all possible die
        for (size_t i = 0; i < moves.size(); ++i) {
            if (diceLeftPossible[i] == 0) { // If the move uses the minimum die found
                validMoves.push_back(moves[i]); 
            }
        }
        return validMoves; // Return valid moves if minimum die is 0
    }

    // If there are no moves that use all possible dice, we can only choose moves that use the highest possible die. 
    int minDice = 0; // Variable to track the maximum die used (set to -7 for player2 (impossible))
    for (size_t i = 0; i < moves.size(); ++i) {
        if (diceLeftPossible[i] == dieLeftMin) { // If the move uses the minimum die found
            minDice = std::min(minDice, moves[i].second); // Using min here because player 2 has negative to 
        }
    }

    for (size_t i = 0; i < moves.size(); ++i) {
        if (diceLeftPossible[i] == dieLeftMin && moves[i].second == minDice) { // If the move uses the most amount of die and has highest dice face. 
            validMoves.push_back(moves[i]); 
        }
    }
    return validMoves; // Return valid moves
}

int Board::getOutcome() const {
    // Check if player 1 has won
    if (std::all_of(player1, player1 + 24, [](uint8_t p) { return p == 0; }) && bar1 == 0) {
        if (bar2 > 0 || std::any_of(player2 + 18, player2 + 24, [](uint8_t p) { return p > 0; })) {
            return 3; // Player 1 wins with a backgammon
        }
        else if (std::accumulate(player2, player2 + 24, 0) == 15) {
            return 2; // Player 1 wins with a gammon
        }
        return 1; // Player 1 wins normally
    }

    // Check if player 2 has won
    if (std::all_of(player2, player2 + 24, [](uint8_t p) { return p == 0; }) && bar2 == 0) {
        if (bar1 > 0 || std::any_of(player1, player1 + 6, [](uint8_t p) { return p > 0; })) {
            return -3; // Player 2 wins with a backgammon
        }
        else if (std::accumulate(player1, player1 + 24, 0) == 15) {
            return -2; // Player 2 wins with a gammon
        }
        return -1; // Player 2 wins normally
    }


    return 0; // Game is still ongoing
}

std::vector<std::pair<int, int>> Board::validMovesPlayer1() const {
    std::vector<std::pair<int, int>> moves;

    // Check if player 1 has pieces on the bar (must place a piece from the bar)
    if (bar1 > 0) {
        for (int i = 0; i < 6; ++i) {
            if (dice[i] > 0) { // Check if the die is valid
                int targetPosition = i; // Calculate target position based on the die rolled
                if (player2[targetPosition] <= 1) {
                    moves.emplace_back(targetPosition, 7); // Move from bar to target position (7 signifies emplacement move)
                }
            }
        }
        return moves; // Return moves if pieces are on the bar
    }

    // Check for bearing off 
    // If all pieces are in the home board, from position 0 to 17 inclusive are zero
    if (std::all_of(player1, player1 + 18, [](uint8_t p) { return p == 0; })) {
        for (int i = 0; i < 6; ++i) {
            if (dice[i] > 0) { // Check if the die is valid
                int targetPosition = 23 - i; // Calculate target position for bearing off
                if (player1[targetPosition] > 0) {
                    moves.emplace_back(targetPosition, i + 1); // Must bear off a piece
                } else {
                    // Check to see if there are any pieces higher than the target position that can be beared off
                    bool higherPieceFound = false;
                    for (int j = targetPosition - 1; j >= 18; --j) {
                        if (player1[j] > 0 && player2[j + i + 1] <= 1) {
                            moves.emplace_back(j, i + 1); // Move a piece from a higher position
                            higherPieceFound = true;
                        }
                    }
                    if (!higherPieceFound) {
                        // If no higher pieces found, bear off the next available lower piece 
                        for (int j = targetPosition + 1; j < 24; ++j) {
                            if (player1[j] > 0) {
                                moves.emplace_back(j, i + 1); // Bear off next available piece
                                break; // Exit loop after finding piece to bear off. 
                            }
                        }
                    }

                }
            }
        }
        return moves; // Return moves if bearing off is possible
    }

    // Check each piece on the board for valid moves
    for (int i = 0; i < 24; ++i) {
        if (player1[i] > 0) { // Player 1 has pieces on this position
            for (int j = 0; j < 6; ++j) {
                if (dice[j] > 0) { // Check if the die is available
                    int targetPosition = i + j + 1; // Calculate target position
                    if (targetPosition < 24 && player2[targetPosition] <= 1) { // Valid move to an open point (can't bear off)
                        moves.emplace_back(i, j + 1); // Add valid move
                    }
                }
            }
        }
    }

    return moves; // Return all valid moves for player 1
}

std::vector<std::pair<int, int>> Board::validMovesPlayer2() const {
    std::vector<std::pair<int, int>> moves;

    // Check if player 1 has pieces on the bar (must place a piece from the bar)
    if (bar2 > 0) {
        for (int i = 0; i < 6; ++i) {
            if (dice[i] > 0) { // Check if the die is valid
                int targetPosition = 23 - i; // Calculate target position based on the die rolled
                if (player1[targetPosition] <= 1) {
                    moves.emplace_back(targetPosition, 7); // Move from bar to target position (7 signifies emplacement move)
                }
            }
        }
        return moves; // Return moves if pieces are on the bar
    }

    
    // If all pieces are in the home board, from position 6 to 23 are zero
    if (std::all_of(player2 + 6, player2 + 24, [](uint8_t p) { return p == 0; })) {
        for (int i = 0; i < 6; ++i) {
            if (dice[i] > 0) { // Check if the die is valid
                int targetPosition = i; // Calculate target position for bearing off
                if (player2[targetPosition] > 0) {
                    moves.emplace_back(targetPosition, - (i + 1)); // Must bear off a piece
                } else {
                    // Check to see if there are any pieces higher than the target position that can be moved
                    bool higherPieceFound = false;
                    for (int j = i + 1; j <= 5; ++j) {
                        if (player2[j] > 0 && player1[j - (i + 1)] <= 1) {
                            moves.emplace_back(j, - (i + 1)); // Move a piece from a higher position
                            higherPieceFound = true;
                        }
                    }
                    if (!higherPieceFound) {
                        // If no higher pieces found, bear off the next available lower piece 
                        for (int j = targetPosition - 1; j >= 0; --j) {
                            if (player2[j] > 0) {
                                moves.emplace_back(j, - (i + 1)); // Bear off next available piece
                                break; // Exit the loop after finding a piece to bear off
                            }
                        }
                    }

                }
            }
        }
        return moves; // Return moves if bearing off is possible
    }

    // Check each piece on the board for valid moves
    for (int i = 0; i < 24; ++i) {
        if (player2[i] > 0) { // Player 2 has pieces on this position
            for (int j = 0; j < 6; ++j) {
                if (dice[j] > 0) { // Check if the die is available
                    int targetPosition = i - (j + 1); // Calculate target position
                    if (targetPosition >= 0 && player1[targetPosition] <= 1) { // Valid move to an open point (can't bear off)
                        moves.emplace_back(i, - (j + 1)); // Add valid move
                    }
                }
            }
        }
    }

    return moves; // Return all valid moves for player 1
}  


void Board::handlePlayer1Move(int from, int distance) {
    if (distance == 7) {
        player1[from]++; // If distance is 7, it means placing a piece from the bar
        bar1--; // Remove a piece from the bar
        return;
    }
    player1[from]--; // Remove a piece from the starting position
    int targetPosition = from + distance; // Calculate the target position
    player1[targetPosition]++; // Place a piece in the target position
    if (player2[targetPosition] == 1) {
        player2[targetPosition] = 0;
        bar2++; // If there was an opponent's piece, move it to the bar
    }
    dice[distance - 1]--; // Mark the die as used
}

void Board::handlePlayer2Move(int from, int distance) {
    if (distance == 7) {
        player2[from]++; // If distance is 7, it means placing a piece from the bar
        bar2--; // Remove a piece from the bar
        return;
    }
    player2[from]--; // Remove a piece from the starting position
    int targetPosition = from + distance; // Calculate the target position
    player2[targetPosition]++; // Place a piece in the target position
    if (player1[targetPosition] == 1) {
        player1[targetPosition] = 0;
        bar1++; // If there was an opponent's piece, move it to the bar
    }
    dice[(-distance) - 1]--; // Mark the die as used (negative index for player 2)
}

bool Board::isGameOver() const {
    // Check if either player has won the game by bearing off all their pieces
    bool player1Won = std::all_of(player1, player1 + 24, [](uint8_t p) { return p == 0; }) && bar1 == 0;
    bool player2Won = std::all_of(player2, player2 + 24, [](uint8_t p) { return p == 0; }) && bar2 == 0;
    
    return player1Won || player2Won;
}

bool Board::diceAvailable(int face) const {
    if (face < 1 || face > 6) {
        throw std::out_of_range("Dice value must be between 1 and 6.");
    }
    return this->dice[face - 1] > 0; // Check if the specified die is available
}