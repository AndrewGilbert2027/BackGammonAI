#include <gtest/gtest.h>
#include "../logic/Board.h"

// Test fixture class with privileged access to Board
class BoardFixture : public ::testing::Test {
protected:
    Board board;
    Board barBoard; // Board with pieces on the bar for testing
    Board barBoardComplex; // Board with complex bar state for testing
    Board bearingOffBoard; // Board with pieces ready for bearing off
    
    void SetUp() override {
        // Default setup is handled by Board constructor
        board = Board(); // Reset the board to its initial state
        int barState[31] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 
                            0, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, 1, 
                            1, 0, -1, -1, -1, -1, 1}; // Player 1 has piece on the bar

        barBoard = Board(barState); // Initialize the board with pieces on the bar
        int barStateComplex[31] = {-2, 0, -1, 2, -4, 1, 0, 0, 0, 0, 0, 5, 
                                   0, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, 1, 
                                   1, 0, -1, -1, -1, -1, 1}; 
        barBoardComplex = Board(barStateComplex); 
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
    
    void setCurrentPlayer(int player) {
        board.currentPlayer = player;
    }

    void setCurrentPlayerBar(int player) {
        barBoard.currentPlayer = player; // Set the current player for the bar board
    }

    void setCurrentPlayerBarComplex(int player) {
        barBoardComplex.currentPlayer = player; // Set the current player for the complex bar board
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




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }