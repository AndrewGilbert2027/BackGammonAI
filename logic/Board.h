#ifndef BOARD_H
#define BOARD_H
#include <inttypes.h>
#include <vector>
#include <numeric> // For std::accumulate



/**
 * @file Board.h
 * @brief Header file for the Board class, representing a backgammon board.
 * 
 * This class manages the state of the backgammon board, including player positions,
 * dice rolls, and player turns.
 * 
 * It provides methods to initialize the board, roll dice, move pieces,
 * and switch players.
 * 
 * Player1 moves from positions 0 to 23, while Player2 moves from positions 23 to 0.
 * 
 */
class Board {
    friend class BoardFixture; // Allow BoardFixture to access private members for testing
    
    public: 

        /**
         * @brief Constructs a new Board object.
         * This constructor initializes the backgammon board with two players,
         * each having 15 pieces on their respective sides. 
         */
        Board();

        /**
         * @brief Create a new Board object from a character array.
         * This constructor initializes the board from a character array representing the board state.
         * @param boardState An integer array representing the board state. 
         * Size of the array is 31 (24 positions + 2 bars + 4 dice + current player).
         * The first 24 elements represent the positions on the board,
         * the 25th element represents the bar for player 1,
         * and the 26th element represents the bar for player 2.
         * 27th - 30th elements are for dice.
         * 31st element is the current player. 
         */
        Board(const int boardState[31]);
        

        /**
         * @brief Destroys the Board object.
         * This destructor cleans up any resources used by the Board.
         */
        ~Board();

        /**
         * @brief Copy constructor for the Board class.
         * This constructor creates a new Board object as a copy of another Board object.
         * @param other The Board object to copy from.
         * This method performs a deep copy of the board state, including player positions and dice values.
         * It ensures that the new Board object has its own independent state.
         */
        Board(const Board& other);

        /**
         * @brief Assignment operator for the Board class.
         * This operator assigns the state of one Board object to another.
         * @param other The Board object to copy from.
         * @return A reference to the current Board object.
         * This method performs a deep copy of the board state, including player positions and dice values.
         */
        Board& operator=(const Board& other);

        /**
         * @brief Resets the board to its initial state.
         * This method clears the player positions and resets the dice.
         */
        void reset();

        /**
         * @brief Rolls the dice for the current player and stores the results in the dice array.
         * This method generates two random numbers between 1 and 6,
         * simulating a dice roll.
         */
        void rollDice();

        /**
         * @brief Moves a player's piece from a specified position by a given distance.
         * @param from The starting position of the piece to be moved.
         * @param distance The number of spaces to move the piece.
         * This method updates the player's position on the board.
         * It assumes that the move is valid and does not check for game rules. 
         * @throws std::out_of_range if the move goes out of bounds.
         */
        void move(int from, int distance);

        /**
         * @brief Moves a player's piece from a specified position by a given distance. 
         * Switches to the next player if all dice are used and rolls again. 
         * @param from The starting position of the piece to be moved.
         * @param distance The number of spaces to move the piece.
         * This method updates the player's position on the board.
         */
        void step(int from, int distance);

        /**
         * @brief Changes the current player.
         * This method switches the turn to the other player.
         * It toggles the currentPlayer variable between 1 and -1.
         */
        void changePlayer();

        /**
         * @brief Gets the position of a player's piece on the board.
         */
        int getCurrentPlayer() const;


        /**
         * @brief Checks if a specific die is available for the current player.
         * @param face The face value of the die to check (1 to 6).
         * @return true if the die is available, false otherwise.
         * This method checks if the specified die is available for the current player.
         * It returns true if the die is available (i.e., the count is greater than 0),
         * and false if it has already been used or is not available.
         * @throws std::out_of_range if the face value is not between 1 and 6.
         */
        bool diceAvailable(int face) const;

        uint8_t diceLeft() const {
            return std::accumulate(std::begin(dice), std::end(dice), 0); // Returns the total number of available dice
        }

        /**
         * @brief Gets the number of pieces on the bar for player 1.
         * @return The number of pieces on the bar for player 1.
         */
        uint8_t getBar1() const { return bar1; }

        /**
         * @brief Gets the number of pieces on the bar for player 2.
         * @return The number of pieces on the bar for player 2.
         */
        uint8_t getBar2() const { return bar2; }


        /**
         * @brief Returns all valid moves for the current player. 
         * This method checks the current player's pieces and the rolled dice to determine all possible moves.
         * @return A vector of pairs, where each pair contains the starting position and the distance to move.
         * Each pair represents a valid move that the current player can make.
         * The first element of the pair is the starting position of the piece,
         * and the second element is the distance to move the piece.
         */
        std::vector<std::pair<int, int>> validMoves() const;

         
        /**
         * @brief Returns all valid moves for player 1.
         * This method checks player 1's pieces and the rolled dice to determine all possible moves.
         * @return A vector of pairs, where each pair contains the starting position and the distance to move.
         */
        std::vector<std::pair<int, int>> validMovesPlayer1() const;

        /**
         * @brief Returns all valid moves for player 2.
         * This method checks player 2's pieces and the rolled dice to determine all possible moves.
         * @return A vector of pairs, where each pair contains the starting position and the distance to move.
         */
        std::vector<std::pair<int, int>> validMovesPlayer2() const;

        Board stepReturn(int from, int distance) const {
            Board newBoard(*this); // Create a copy of the current board
            newBoard.move(from, distance); // Move the piece on the copied board
            return newBoard; // Return the new board state
        }


        /**
         * @brief Checks if the game is over.
         * @return true if the game is over, false otherwise.
         * This method checks if either player has won the game by bearing off all their pieces.
         */
        bool isGameOver() const; 

        /**
         * @brief Gets the outcome of the game. 
         * Returns 0 if game is ongoing, 1 if player 1 wins, -1 if player 2 wins,
         * 2 if player 1 gets a gammon, -2 if player 2 gets a gammon,
         * 3 if player 1 gets a backgammon, -3 if player 2 gets a backgammon.
         * @return The outcome of the game as an integer.
         */
        int getOutcome() const;





    private: 
        uint8_t player1[24]; // Array representing player 1's pieces on the board
        uint8_t player2[24]; // Array representing player 2's pieces on the board
        uint8_t bar1;        // Number of pieces on the bar for player 1
        uint8_t bar2;        // Number of pieces on the bar for player 2
        uint8_t dice[6];     // Array representing how many dice rolls for each number (used to avoid duplicate moves) 
                             // the player has rolled doubles, otherwise they are -1.
        int currentPlayer;   // Variable representing the current player (1 or -1).


    
        ///////////////////////////////////////////////////
        ///// PRIVATE METHODS FOR VALID MOVES /////////////
        ///////////////////////////////////////////////////

        /**
         * @brief Returns all valid moves for player 1 using a depth-first search approach.
         * This method checks player 1's pieces and the rolled dice to determine all possible moves.
         * This method is used to explore all possible moves for player 1 and make sure it only returns moves
         * that are valid according to the game rules (using moves that allow the maximum amount of dice to be used).
         * @return A vector of pairs, where each pair contains the starting position and the distance to move. 
         */
        std::vector<std::pair<int, int>> validMovesDFS1() const;

        /**
         * @brief Returns all valid moves for player 2 using a depth-first search approach.
         * This method checks player 2's pieces and the rolled dice to determine all possible moves.
         * This method is used to explore all possible moves for player 2 and make sure it only returns moves
         * that are valid according to the game rules (using moves that allow the maximum amount of dice to be used).
         * @return A vector of pairs, where each pair contains the starting position and the distance to move.
         */
        std::vector<std::pair<int, int>> validMovesDFS2() const;



        /**
         * @brief Handles the movement of player 1's pieces.
         * This method updates player 1's position on the board based on the move.
         * @param from The starting position of the piece to be moved.
         * @param distance The number of spaces to move the piece.
         */
        void handlePlayer1Move(int from, int distance);


        /**
         * @brief Handles the movement of player 2's pieces.
         * This method updates player 2's position on the board based on the move.
         * @param from The starting position of the piece to be moved.
         * @param distance The number of spaces to move the piece.
         */
        void handlePlayer2Move(int from, int distance);
};


#endif // BOARD_H