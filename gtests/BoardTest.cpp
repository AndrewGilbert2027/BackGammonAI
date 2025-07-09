#include <gtest/gtest.h>
#include "../logic/Board.h"


TEST(BoardTest, InitialSetup) {
    Board board;

    EXPECT_EQ(board.getBar1(), 0); // Player 1 should have no pieces on the bar
    EXPECT_EQ(board.getBar2(), 0); // Player 2 should have no pieces on the bar
    int count = 0;
    for (int i = 0; i < 6; ++i) {
        count += (board.diceAvailable(i + 1) ? 1 : 0);
    }
    EXPECT_EQ(count, 2); // 2 dice should be available 
}

TEST(BoardTest, ValidMovesPlayer1) {
    Board board;
    auto moves = board.validMoves();
    
    // Check if the initial valid moves are correct
    EXPECT_FALSE(moves.empty()); // There should be valid moves available
    for (const auto& move : moves) {
        EXPECT_GE(move.first, 0); // Starting position should be valid
        EXPECT_LE(move.first, 23); // Starting position should be within the board
        EXPECT_GE(move.second, 1); // Distance should be at least 1
        EXPECT_LE(move.second, 6); // Distance should not exceed 6
    }
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }