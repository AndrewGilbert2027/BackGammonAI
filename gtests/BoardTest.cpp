#include <gtest/gtest.h>
#include "../logic/Board.h"

// Test fixture class with privileged access to Board
class BoardFixture : public ::testing::Test {
protected:
    Board board;
    Board barBoard; // Board with pieces on the bar for testing
    Board barBoardComplex; // Board with complex bar state for testing
    Board bearingOffBoard; // Board with pieces ready for bearing off
    Board bearingOffComplex; // Board with complex bearing off state for testing
    
    void SetUp() override {
        // Default setup is handled by Board constructor
        board = Board(); // Reset the board to its initial state
        int barState[31] = {0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 5, 
                            0, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, 1, 
                            1, 0, -1, -1, -1, -1, 1}; // Player 1 has piece on the bar

        barBoard = Board(barState); // Initialize the board with pieces on the bar
        int barStateComplex[31] = {-2, 0, -1, 2, -4, 1, 0, 0, 0, 0, 0, 5, 
                                   0, 0, 0, 0, 3, 0, 5, -1, 4, 3, -2, 1, 
                                   1, 0, -1, -1, -1, -1, 1}; 
        barBoardComplex = Board(barStateComplex); 

        int bearingOffState[31] = {-1, 0, -2, -4, 0, 0, 0, 0, 0, 0, 0, 0, 
                                   0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 1, 
                                   0, 0, -1, -1, -1, -1, 1}; // Player ready to bear off
        bearingOffBoard = Board(bearingOffState); // Initialize the board with pieces ready for bearing off

        int bearOffComplex[31] = {-2, 0, 2, 2, -4, 1, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 3, 0, 5, 0, 4, 3, 0, 1, 
                                   0, 0, -1, -1, -1, -1, 1}; // Complex bearing off state
        bearingOffComplex = Board(bearOffComplex); // Initialize the board with complex bearing off state
    
    }
    
    // Helper methods to manipulate internal board state
    void setDice(int face1, int face2) {
        // Clear existing dice
        std::fill(std::begin(board.dice), std::end(board.dice), 0);
        
        // Set the specific dice values
        if (face1 > 0 && face1 <= 6) board.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) board.dice[face2-1]++;
    }

    void setDiceBar(int face1, int face2) {
        // Clear existing dice for the bar board
        std::fill(std::begin(barBoard.dice), std::end(barBoard.dice), 0);
        
        // Set the specific dice values for the bar board
        if (face1 > 0 && face1 <= 6) barBoard.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) barBoard.dice[face2-1]++;
    }

    void setDiceBarComplex(int face1, int face2) {
        // Clear existing dice for the complex bar board
        std::fill(std::begin(barBoardComplex.dice), std::end(barBoardComplex.dice), 0);
        
        // Set the specific dice values for the complex bar board
        if (face1 > 0 && face1 <= 6) barBoardComplex.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) barBoardComplex.dice[face2-1]++;
    }

    void setDiceBearOff(int face1, int face2) {
        // Clear existing dice for the bearing off board
        std::fill(std::begin(bearingOffBoard.dice), std::end(bearingOffBoard.dice), 0);
        
        // Set the specific dice values for the bearing off board
        if (face1 > 0 && face1 <= 6) bearingOffBoard.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) bearingOffBoard.dice[face2-1]++;
    }

    void setDiceBearOffComplex(int face1, int face2) {
        // Clear existing dice for the complex bearing off board
        std::fill(std::begin(bearingOffComplex.dice), std::end(bearingOffComplex.dice), 0);
        
        // Set the specific dice values for the complex bearing off board
        if (face1 > 0 && face1 <= 6) bearingOffComplex.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) bearingOffComplex.dice[face2-1]++;
    }
    
    void setCurrentPlayer(int player) {
        board.currentPlayer = player;
    }

    void setCurrentPlayerBar(int player) {
        barBoard.currentPlayer = player; // Set the current player for the bar board
    }

    void setCurrentPlayerBarComplex(int player) {
        barBoardComplex.currentPlayer = player; // Set the current player for the complex bar board
    }

    void setCurrentPlayerBearOff(int player) {
        bearingOffBoard.currentPlayer = player; // Set the current player for the bearing off board
    }
    
    void setCurrentPlayerBearOffComplex(int player) {
        bearingOffComplex.currentPlayer = player; // Set the current player for the complex bearing off board
    }

    void placePiece(int player, int position, int count) {
        if (player == 1) {
            board.player1[position] = count;
        } else if (player == -1 || player == 2) {
            board.player2[position] = count;
        }
    }
    
    void setBar(int player, int count) {
        if (player == 1) {
            board.bar1 = count;
        } else if (player == -1 || player == 2) {
            board.bar2 = count;
        }
    }

    void setBarBar(int player, int count) {
        if (player == 1) {
            barBoard.bar1 = count; // Set the number of pieces on the bar for player 1 in barBoard
        } else if (player == -1 || player == 2) {
            barBoard.bar2 = count; // Set the number of pieces on the bar for player 2 in barBoard
        }
    }

    void setBarComplex(int player, int count) {
        if (player == 1) {
            barBoardComplex.bar1 = count; // Set the number of pieces on the bar for player 1 in complex barBoard
        } else if (player == -1 || player == 2) {
            barBoardComplex.bar2 = count; // Set the number of pieces on the bar for player 2 in complex barBoard
        }
    }

    void setBarBearOffComplex(int player, int count) {
        if (player == 1) {
            bearingOffComplex.bar1 = count; // Set the number of pieces on the bar for player 1 in complex bearing off board
        } else if (player == -1 || player == 2) {
            bearingOffComplex.bar2 = count; // Set the number of pieces on the bar for player 2 in complex bearing off board
        }
    }

    void resetBoard() {
        board.reset(); // Reset the board to its initial state
    }

    void rollDice() {
        board.rollDice(); // Roll the dice for the current player
    }

    void changePlayer() {
        board.changePlayer(); // Change the current player
    }

    void move(int from, int distance) {
        board.move(from, distance); // Move a piece from the specified position
    }
    void step(int from, int distance) {
        board.step(from, distance); // Step a piece from the specified position
    }
    bool diceAvailable(int face) const {
        return board.diceAvailable(face); // Check if a specific die is available
    }
    int getCurrentPlayer() const {
        return board.getCurrentPlayer(); // Get the current player
    }
    uint8_t getBar1() const {
        return board.getBar1(); // Get the number of pieces on the bar for player 1
    }

    uint8_t getBar2() const {
        return board.getBar2(); // Get the number of pieces on the bar for player 2
    }

    uint8_t getBarBar1() const {
        return barBoard.getBar1(); // Get the number of pieces on the bar for player 1 in barBoard
    }

    uint8_t getBarBar2() const {
        return barBoard.getBar2(); // Get the number of pieces on the bar for player 2 in barBoard
    }

    uint8_t getBarComplex1() const {
        return barBoardComplex.getBar1(); // Get the number of pieces on the bar for player 1 in complex barBoard
    }

    uint8_t getBarComplex2() const {
        return barBoardComplex.getBar2(); // Get the number of pieces on the bar for player 2 in complex barBoard
    }

    uint8_t* getPlayer1Board() {
        return board.player1; // Get the board state for player 1
    }

    uint8_t* getPlayer2Board() {
        return board.player2; // Get the board state for player 2
    }

    uint8_t* getDice() {
        return board.dice; // Get the dice state
    }

};


TEST_F(BoardFixture, InitialSetup) {
    EXPECT_EQ(board.getBar1(), 0); // Player 1 should have no pieces on the bar
    EXPECT_EQ(board.getBar2(), 0); // Player 2 should have no pieces on the bar
    int count = 0;
    for (int i = 0; i < 6; ++i) {
        count += (board.diceAvailable(i + 1) ? 1 : 0);
    }
    EXPECT_EQ(count, 2); // 2 dice should be available 
}

TEST_F(BoardFixture, ValidMovesBasic) {
    setCurrentPlayer(1); // Set current player to Player 1
    auto moves = board.validMoves();
    
    // Check if the initial valid moves are correct
    EXPECT_FALSE(moves.empty()); // There should be valid moves available
    for (const auto& move : moves) {
        EXPECT_GE(move.first, 0); // Starting position should be valid
        EXPECT_LE(move.first, 23); // Starting position should be within the board
        EXPECT_GE(move.second, 1); // Distance should be at least 1
        EXPECT_LE(move.second, 6); // Distance should not exceed 6
    }

    setCurrentPlayer(-1); // Set current player to Player 2
    moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    for (const auto& move : moves) {
        EXPECT_GE(move.first, 0); // Starting position should be valid
        EXPECT_LE(move.first, 23); // Starting position should be within the board
        EXPECT_GE(move.second, -6); // Distance between -6 and -1 
        EXPECT_LE(move.second, -1); 
    }
}

TEST_F(BoardFixture, ValidMovesPlayer1) {
    setDice(1, 2); // Set dice to 1 and 2
    setCurrentPlayer(1); // Set current player to Player 1
    auto moves = board.validMoves();

    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 7); // There should be 7 valid moves for Player 1 with dice 1 and 2

    std::vector<std::pair<int, int>> expected_moves = {
        {0, 1}, {0, 2}, {11, 2}, {16, 1}, {16, 2}, {18, 1}, {18, 2}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }

    setDice(3, 4);
    setCurrentPlayer(1); // Set current player to Player 1
    moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 8); // There should be 8 valid moves for Player 1 with dice 3 and 4
    expected_moves = {
        {0, 3}, {0, 4}, {11, 3}, {11, 4}, {16, 3}, {16, 4}, {18, 3}, {18, 4}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }

    setDice(5, 6);
    setCurrentPlayer(1); // Set current player to Player 1
    moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 5); // There should be 8 valid moves for Player 1 with dice 5 and 6
    expected_moves = {
        {0, 6}, {11, 5}, {11, 6}, {16, 5}, {16, 6}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, ValidMovesPlayer2) {
    setDice(1, 2); // Set dice to 1 and 2
    setCurrentPlayer(-1); // Set current player to Player 2
    auto moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 7); // There should be 7 valid moves for Player 2 with dice 1 and 2
    std::vector<std::pair<int, int>> expected_moves = {
        {23, -1}, {23, -2}, {12, -2}, {7, -1}, {7, -2}, {5, -1}, {5, -2}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDice(3, 4);
    setCurrentPlayer(-1); // Set current player to Player 2
    moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 8); // There should be 8 valid moves for Player 2 with dice 3 and 4
    expected_moves = {
        {23, -3}, {23, -4}, {12, -3}, {12, -4}, {7, -3}, {7, -4}, {5, -3}, {5, -4}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDice(5, 6);
    setCurrentPlayer(-1); // Set current player to Player 2
    moves = board.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 5); // There should be 5 valid moves for Player 2 with dice 5 and 6
    expected_moves = {
        {23, -6}, {12, -5}, {12, -6}, {7, -5}, {7, -6}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestMovePlayer1) {
    setCurrentPlayer(1); 
    setDice(1, 2);  
    board.move(0, 1);  
    EXPECT_EQ(getPlayer1Board()[0], 1);  
    EXPECT_EQ(getPlayer1Board()[1], 1);  
    EXPECT_EQ(getPlayer2Board()[0], 0);  
    EXPECT_EQ(getPlayer2Board()[1], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_TRUE(diceAvailable(2));  
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_EQ(getCurrentPlayer(), 1);  
    board.move(0, 2);  
    EXPECT_EQ(getPlayer1Board()[0], 0);  
    EXPECT_EQ(getPlayer1Board()[2], 1);  
    EXPECT_EQ(getPlayer2Board()[0], 0);  
    EXPECT_EQ(getPlayer2Board()[2], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_FALSE(diceAvailable(2));  
}

TEST_F(BoardFixture, TestMovePlayer2) {
    setCurrentPlayer(-1);   
    setDice(1, 2);  
    board.move(23, -1);  
    EXPECT_EQ(getPlayer2Board()[23], 1);  
    EXPECT_EQ(getPlayer2Board()[22], 1);  
    EXPECT_EQ(getPlayer1Board()[23], 0);  
    EXPECT_EQ(getPlayer1Board()[22], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_TRUE(diceAvailable(2));  
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_EQ(getCurrentPlayer(), -1);  
    board.move(23, -2);  
    EXPECT_EQ(getPlayer2Board()[23], 0);  
    EXPECT_EQ(getPlayer2Board()[21], 1);  
    EXPECT_EQ(getPlayer1Board()[23], 0);  
    EXPECT_EQ(getPlayer1Board()[21], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_FALSE(diceAvailable(2));  
}

TEST_F(BoardFixture, TestStepPlayer1) {
    setCurrentPlayer(1);  
    setDice(1, 2);  
    board.step(0, 1);  
    EXPECT_EQ(getPlayer1Board()[0], 1);  
    EXPECT_EQ(getPlayer1Board()[1], 1);  
    EXPECT_EQ(getPlayer2Board()[0], 0);  
    EXPECT_EQ(getPlayer2Board()[1], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_TRUE(diceAvailable(2));  
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_EQ(getCurrentPlayer(), 1);  
    board.step(0, 2);  
    EXPECT_EQ(getPlayer1Board()[0], 0);  
    EXPECT_EQ(getPlayer1Board()[2], 1);  
    EXPECT_EQ(getPlayer2Board()[0], 0);  
    EXPECT_EQ(getPlayer2Board()[2], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_EQ(getCurrentPlayer(), -1);  
}

TEST_F(BoardFixture, TestStepPlayer2) {
    setCurrentPlayer(-1);  
    setDice(1, 2);  
    board.step(23, -1);  
    EXPECT_EQ(getPlayer2Board()[23], 1); 
    EXPECT_EQ(getPlayer2Board()[22], 1); 
    EXPECT_EQ(getPlayer1Board()[23], 0); 
    EXPECT_EQ(getPlayer1Board()[22], 0);
    EXPECT_EQ(getBar1(), 0); 
    EXPECT_EQ(getBar2(), 0);
    EXPECT_TRUE(diceAvailable(2)); 
    EXPECT_FALSE(diceAvailable(1));  
    EXPECT_EQ(getCurrentPlayer(), -1); 
    board.step(23, -2);  
    EXPECT_EQ(getPlayer2Board()[23], 0);  
    EXPECT_EQ(getPlayer2Board()[21], 1);  
    EXPECT_EQ(getPlayer1Board()[23], 0);  
    EXPECT_EQ(getPlayer1Board()[21], 0);  
    EXPECT_EQ(getBar1(), 0);  
    EXPECT_EQ(getBar2(), 0);  
    EXPECT_EQ(getCurrentPlayer(), 1);  
}

TEST_F(BoardFixture, TestBarPlayer1) {
    setCurrentPlayerBar(1);
    setDiceBar(1, 2); 
    EXPECT_EQ(getBarBar1(), 1) << "Player 1 should have 1 piece on the bar.";
    EXPECT_EQ(getBarBar2(), 0) << "Player 2 should have no pieces on the bar.";
    auto moves = barBoard.validMoves();
    std::cout << std::endl;
    EXPECT_FALSE(moves.empty()); 
    EXPECT_EQ(moves.size(), 2);
    std::vector<std::pair<int, int>> expected_moves = {
        {0, 7}, {1, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBar(3, 4);
    setCurrentPlayerBar(1); 
    moves = barBoard.validMoves();
    EXPECT_FALSE(moves.empty());
    EXPECT_EQ(moves.size(), 2); 
    expected_moves = {
        {2, 7}, {3, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBar(5, 6);
    setCurrentPlayerBar(1);
    moves = barBoard.validMoves();
    EXPECT_FALSE(moves.empty());
    EXPECT_EQ(moves.size(), 2); 
    expected_moves = {
        {4, 7}, {5, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestBarPlayer2) {
    setDiceBar(1, 2); // Set dice to 1 and 2
    setCurrentPlayerBar(-1); // Set current player to Player 2
    setBarBar(-1, 1); // Player 2 has 1 piece on the bar (otherwise no valid moves)
    auto moves = barBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2); // There should be 2 valid moves for Player 2 with dice 1 and 2
    std::vector<std::pair<int, int>> expected_moves = {
        {23, 7}, {22, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBar(3, 4); // Set dice to 3 and 4
    setCurrentPlayerBar(-1); // Set current player to Player 2
    moves = barBoard.validMoves();
    EXPECT_FALSE(moves.empty()); 
    EXPECT_EQ(moves.size(), 2); 
    expected_moves = {
        {21, 7}, {20, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBar(5, 6); // Set dice to 5 and 6
    setCurrentPlayerBar(-1); // Set current player to Player 2
    moves = barBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 1) << "Moves should be 1"; // Player1 blocking 6th position
    expected_moves = {
        {19, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestBarComplexPlayer1) {
    setCurrentPlayerBarComplex(1);
    setDiceBarComplex(1, 2); // Set dice to 1 and 2
    EXPECT_EQ(getBarComplex1(), 1) << "Player 1 should have 1 piece on the bar.";
    EXPECT_EQ(getBarComplex2(), 0) << "Player 2 should have no pieces on the bar.";
    auto moves = barBoardComplex.validMoves();
    EXPECT_FALSE(moves.empty()); 
    EXPECT_EQ(moves.size(), 1);
    std::vector<std::pair<int, int>> expected_moves = {
        {1, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBarComplex(3, 4);
    setCurrentPlayerBarComplex(1); // Set current player to Player 1
    moves = barBoardComplex.validMoves();
    EXPECT_FALSE(moves.empty());
    EXPECT_EQ(moves.size(), 2);
    expected_moves = {
        {2, 7}, {3, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBarComplex(5, 6);
    setCurrentPlayerBarComplex(1); // Set current player to Player 1
    moves = barBoardComplex.validMoves();
    EXPECT_FALSE(moves.empty());
    EXPECT_EQ(moves.size(), 1);
    expected_moves = {
        {5, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestBarComplexPlayer2) {
    setDiceBarComplex(1, 2); // Set dice to 1 and 2
    setCurrentPlayerBarComplex(-1); // Set current player to Player 2
    setBarComplex(-1, 1); // Player 2 has 1 piece on the bar (otherwise no valid moves)
    auto moves = barBoardComplex.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2); 
    std::vector<std::pair<int, int>> expected_moves = {
        {23, 7}, {22, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBarComplex(3, 4); // Set dice to 3 and 4
    setCurrentPlayerBarComplex(-1); // Set current player to Player 2
    moves = barBoardComplex.validMoves();
    EXPECT_TRUE(moves.empty()) << "There should be no valid moves for Player 2 with dice 3 and 4";
    setDiceBarComplex(5, 6); // Set dice to 5 and 6
    setCurrentPlayerBarComplex(-1); // Set current player to Player 2
    moves = barBoardComplex.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 1) << "There should be 1 valid move for Player 2 with dice 5 and 6";
    expected_moves = {
        {19, 7}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}


TEST_F(BoardFixture, TestBearingOffPlayer1) {
    setCurrentPlayerBearOff(1); // Set current player to Player 1
    setDiceBearOff(1, 2); // Set dice to 1 and 2
    EXPECT_EQ(bearingOffBoard.getBar1(), 0) << "Player 1 should have no pieces on the bar.";
    EXPECT_EQ(bearingOffBoard.getBar2(), 0) << "Player 2 should have no pieces on the bar.";
    auto moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 2); // There should be 2 valid moves for Player 1 with dice 1 and 2
    std::vector<std::pair<int, int>> expected_moves = {
        {23, 1}, {18, 2}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOff(3, 4); // Set dice to 3 and 4
    setCurrentPlayerBearOff(1); // Set current player to Player 1
    moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 2); // There should be 2 valid moves for Player 1 with dice 3 and 4
    expected_moves = {
        {18, 3}, {18, 4}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOff(5, 6); // Set dice to 5 and 6
    setCurrentPlayerBearOff(1); // Set current player to Player 1
    moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 1
    EXPECT_EQ(moves.size(), 2); // There should be 2 valid moves for Player 1 with dice 5 and 6
    expected_moves = {
        {18, 5}, {18, 6}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestBearingOffPlayer2) {
    setCurrentPlayerBearOff(-1); // Set current player to Player 2
    setDiceBearOff(1, 2); // Set dice to 1 and 2
    EXPECT_EQ(bearingOffBoard.getBar1(), 0) << "Player 1 should have no pieces on the bar.";
    EXPECT_EQ(bearingOffBoard.getBar2(), 0) << "Player 2 should have no pieces on the bar.";
    auto moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available for Player 2";
    EXPECT_EQ(moves.size(), 3) << "Number of valid moves should be 3"; 
    std::vector<std::pair<int, int>> expected_moves = {
        {0, -1}, {2, -2}, {3, -2}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOff(3, 4); // Set dice to 3 and 4
    setCurrentPlayerBearOff(-1); // Set current player to Player 2
    moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    expected_moves = {
        {2, -3}, {3, -4}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOff(5, 6); // Set dice to 5 and 6
    setCurrentPlayerBearOff(-1); // Set current player to Player 2
    moves = bearingOffBoard.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    expected_moves = {
        {3, -5}, {3, -6} // Must bear off from the highest position available
    }; 
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestBearingOffComplex2) {
    setCurrentPlayerBearOffComplex(-1); // Set current player to Player 2
    setDiceBearOffComplex(1, 2); // Set dice to 1 and 2
    EXPECT_EQ(bearingOffComplex.getBar1(), 0) << "Player 1 should have no pieces on the bar.";
    EXPECT_EQ(bearingOffComplex.getBar2(), 0) << "Player 2 should have no pieces on the bar.";
    auto moves = bearingOffComplex.validMoves(); 
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available for Player 2";
    EXPECT_EQ(moves.size(), 2) << "Number of valid moves should be 1";
    std::cout << "Valid moves for Player 2: ";
    for (const auto& move : moves) {
        std::cout << "{" << move.first << ", " << move.second << "} ";
    }
    std::cout << std::endl;
    std::vector<std::pair<int, int>> expected_moves = {
        {0, -1}, {0, -2} // The checker in position 5 cannot be used (blocked)
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOffComplex(3, 4); // Set dice to 3 and 4
    setCurrentPlayerBearOffComplex(-1); // Set current player to Player 2
    moves = bearingOffComplex.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    expected_moves = {
        {4, -3}, {4, -4} // Player 2 can bear off from position 0
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
    setDiceBearOffComplex(5, 6); // Set dice to 5 and 6
    setCurrentPlayerBearOffComplex(-1); // Set current player to Player 2
    moves = bearingOffComplex.validMoves();
    EXPECT_FALSE(moves.empty()); // There should be valid moves available for Player 2
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    expected_moves = {
        {4, -5}, {4, -6} // Player 2 can bear off from position 0
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST(DFS_Test, TestMostAmountOfDieUsed1) {
    int init_board[31] = {0, 0, 0, 0, 1, 0, 0, -2, 1, -2, 0, 0, 
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                          0, 0, 1, 2, -1, -1, 1};
    Board b(init_board);

    auto moves = b.validMoves();
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available";
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    std::vector<std::pair<int, int>> expected_moves = {
        {4, 1}, {8, 2}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST(DFS_Test, TestMostAmountOfDieUsed2) {
    int init_board[31] = {0, 0, 0, 0, 2, -1, 2, 0, 0, -1, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 1, 2, -1, -1, -1};
    Board b(init_board);
    auto moves = b.validMoves();
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available";
    EXPECT_EQ(moves.size(), 2) << "There should be 2 valid moves";
    std::vector<std::pair<int, int>> expected_moves = {
        {5, -2}, {9, -1}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST(DFS_Test, MaxDiceUsedIfTieBreak1) {
    int init_board[31] = {0, 0, 0, 0, 1, 0, 0, 0, 0, -2, 0, -2,
                          -2, -2, -2, -2, -2, -2, 0, 0, 0, 0, 0, 0,
                          0, 0, 4, 6, -1, -1, 1};

    // Even though the minimum amount of die left is 1 for both moves, we must choose the maximum die used. 
    Board b(init_board);
    auto moves = b.validMoves();
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available";
    EXPECT_EQ(moves.size(), 1) << "There should be 1 valid moves";
    std::vector<std::pair<int, int>> expected_moves = {
        {4, 6}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST(DFS_Test, MaxDiceUsedIfTieBreak2) {
    int init_board[31] = {0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0,
                         0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0,
                          0, 0, 4, 6, -1, -1, -1};
    Board b(init_board);
    auto moves = b.validMoves();
    EXPECT_FALSE(moves.empty()) << "There should be valid moves available";
    EXPECT_EQ(moves.size(), 1) << "There should be 1 valid moves";
    std::vector<std::pair<int, int>> expected_moves = {
        {17, -6}
    };
    for (const auto& move : expected_moves) {
        EXPECT_TRUE(std::find(moves.begin(), moves.end(), move) != moves.end())
            << "Expected move " << move.first << ", " << move.second << " not found in valid moves.";
    }
}

TEST_F(BoardFixture, TestGetOutcomeContinue) {
    int outcome = board.getOutcome();
    EXPECT_EQ(outcome, 0) << "Initial outcome should be 0 (no winner yet)";

    outcome = bearingOffBoard.getOutcome();
    EXPECT_EQ(outcome, 0) << "Initial outcome for bearing off board should be 0 (no winner yet)";

    outcome = barBoard.getOutcome();
    EXPECT_EQ(outcome, 0) << "Initial outcome for bar board should be 0 (no winner yet)";

    outcome = barBoardComplex.getOutcome();
    EXPECT_EQ(outcome, 0) << "Initial outcome for complex bar board should be 0 (no winner yet)";

    outcome = bearingOffComplex.getOutcome();
    EXPECT_EQ(outcome, 0) << "Initial outcome for complex bearing off board should be 0 (no winner yet)";
}

TEST(OutcomeTest, TestPlayer2Wins) {
    int init_board[31] = {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 4, 6, -1, -1, -1};
    Board b(init_board);
    int outcome = b.getOutcome();
    EXPECT_EQ(outcome, -1) << "Player 2 should win with the given board state.";

    int gammon_board[31] = {0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 4, 6, -1, -1, -1};
    Board gammon_b(gammon_board);
    int gammon_outcome = gammon_b.getOutcome();
    EXPECT_EQ(gammon_outcome, -2) << "Player 2 should win with a gammon with the given board state.";

    int backgammon_board[31] = {0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 4, 6, -1, -1, -1};
    Board backgammon_b(backgammon_board);
    int backgammon_outcome = backgammon_b.getOutcome();
    EXPECT_EQ(backgammon_outcome, -3) << "Player 2 should win with a backgammon with the given board state.";
}

TEST(OutcomeTest, TestPlayer1Wins) {
    int init_board[31] = {0, 0, 0, 0, 0, 0, -2, -2, -2, -2, -2, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 4, 6, -1, -1, -1};
    Board b(init_board);
    int outcome = b.getOutcome();
    EXPECT_EQ(outcome, 1) << "Player 1 should win with the given board state.";
    int gammon_board[31] = {0, 0, 0, 0, 0, 0, -15, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 4, 6, -1, -1, -1};
    Board gammon_b(gammon_board);
    int gammon_outcome = gammon_b.getOutcome();
    EXPECT_EQ(gammon_outcome, 2) << "Player 1 should win with a gammon with the given board state.";

    int backgammon_board[31] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, 0,
        0, 0, 4, 6, -1, -1, -1};

    Board backgammon_b(backgammon_board);
    int backgammon_outcome = backgammon_b.getOutcome();
    EXPECT_EQ(backgammon_outcome, 3) << "Player 1 should win with a backgammon with the given board state.";
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }