#include <gtest/gtest.h>
#include "../logic/Board.h"

// Test fixture class with privileged access to Board
class BoardFixture : public ::testing::Test {
protected:
    Board board;
    
    void SetUp() override {
        // Default setup is handled by Board constructor
        board = Board(); // Reset the board to its initial state
    }
    
    // Helper methods to manipulate internal board state
    void setDice(int face1, int face2) {
        // Clear existing dice
        std::fill(std::begin(board.dice), std::end(board.dice), 0);
        
        // Set the specific dice values
        if (face1 > 0 && face1 <= 6) board.dice[face1-1]++;
        if (face2 > 0 && face2 <= 6) board.dice[face2-1]++;
    }
    
    void setCurrentPlayer(int player) {
        board.currentPlayer = player;
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
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }