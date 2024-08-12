#ifndef SNAKEGAME_HPP
#define SNAKEGAME_HPP
#include <vector>
#include <stdlib.h>
#include "coordinate.hpp"

#define EMPTY 0
#define SNAKE_HEAD 1
#define SNAKE_BODY 2
#define COIN 3

#define RAND_MAX 2147483647

class SnakeGame{
    private:
        // This is the board. Every entry can contain 0, 1, 2 or 3
        // 0: empty
        // 1: snake head
        // 2: snake body
        // 3: snake
        // Stores the board state
        int board[15][15];
        // Stores pointers to corresponding pre allocated Coordinate objects
        Coordinate *coordinates[15][15];
        int score;
        // Direction can only have values (0, 1), (0, -1), (1, 0), (-1, 0), which are stored int directions
        // We store it as an index to the directions array
        int directionIndex;
        int newDirectionIndex;
        Coordinate *directions[4];
        // Should always be the length of body
        int snakeSize;
        // A coordinate object cannot ever be the coin and in body at the same time
        std::vector<Coordinate*> body;
        Coordinate *coin;
        // Is one if snake has died
        bool dead;
        // Is one if snake has the same size as the whole grid
        bool won;

    public:
        SnakeGame();
        // Return -1 if dead, 0 if nominal, 1 if won and anything else if error occured
        int update();
        // This assumes it is only called if the next field is a coin
        int eat();
        int placeCoin();
        int updateBoard();
        int checkWin();
        bool hasWon();
        bool hasDied();

        int turnLeft();
        int turnRight();
        // Can be call with parameter = -1 (turn left), = 0 (do nothing), = 1 (turn right)
        int turn(int parameter);

        int (*getBoard())[15];
        int getSnakeSize();
        int getScore();
        int getDirectionIndex();
        Coordinate *getCoin();

        // Should be called before exiting program: deletes every datastructure
        void cleanClass();
};

#endif