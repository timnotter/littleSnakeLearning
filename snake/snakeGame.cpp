#include <ctime>
#include <time.h>
#include <stdio.h>
#include "snakeGame.hpp"

int **board;
int snakeSize;
int score;
bool dead;

SnakeGame::SnakeGame(){
    dead = false;
    won = false;
    score = 0;
    // Go thorugh array and initialise it with 0
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            board[i][j] = 0;
            coordinates[i][j] = new Coordinate(i, j);
        }
    }
    directionIndex = 0;
    newDirectionIndex = 0;

    directions[0] = new Coordinate(1, 0);
    directions[1] = new Coordinate(0, 1);
    directions[2] = new Coordinate(-1, 0);
    directions[3] = new Coordinate(0, -1);

    board[8][7] = SNAKE_HEAD;
    body = std::vector<Coordinate*>();
    body.push_back(coordinates[8][7]);

    placeCoin();

    updateBoard();
}

int SnakeGame::update(){
    if(hasWon()){
        printf("You won :)\n");
        return 1;
    }
    if(hasDied()){
        printf("You died :(\n");
        return 1;
    }
    // printf("Curr dir index: %d, new dir index: %d\n", directionIndex, newDirectionIndex);
    directionIndex = newDirectionIndex;

    // Move snake, die, update score, replace coin
    Coordinate *head = body.at(0);
    int newX = ((head->getX() + directions[directionIndex]->getX() + 15) % 15);
    int newY = ((head->getY() + directions[directionIndex]->getY() + 15) % 15);
    // printf("newX: %d, newY: %d\n", newX, newY);
    switch(board[newX][newY]){
        case EMPTY:
            // Add new coordinate to body and remove back
            body.insert(body.begin(), coordinates[newX][newY]);
            body.pop_back();
            break;
        case SNAKE_HEAD:
            // This is only possible if direction was (0,0), which should not be possible
            printf("Error: new position is snake head");
            break;
        case SNAKE_BODY:
            // Die
            body.insert(body.begin(), coordinates[newX][newY]);
            dead = true;
            break;
        case COIN:
            // Eat the coin
            eat();
            break;
        default:
            break;
    }

    // Update board with correct value
    updateBoard();

    if(hasWon()){
        printf("Won\n");
        return 1;
    }
    if(hasDied()){
        printf("Died\n");
        return -1;
    }
    return 0;
}

int SnakeGame::eat(){
    // Add coin field to body
    body.insert(body.begin(), getCoin());
    // Update size
    snakeSize = body.size();

    // Increase score
    score++;

    // Check if won
    hasWon();

    // Place new coin
    placeCoin();

    return 0;
}

int SnakeGame::placeCoin(){
    // Generate two random number between 0 and 15. Check with current grid if snake or coin is present. If yes, repeat. If no, place it

    struct timespec currTime;
    clock_gettime(0, &currTime);

    bool foundNewPosition = false;
    int newX;
    int newY;

    while(!foundNewPosition){
        srand(currTime.tv_nsec);
        newX = (int)rand() / (RAND_MAX / 15);
        srand(currTime.tv_nsec);
        newY = (int)rand() / (RAND_MAX / 15);

        if(board[newX][newY] == 0){
            foundNewPosition = true;
        }
    }

    coin = coordinates[newX][newY];

    return 0;
}

int SnakeGame::updateBoard(){
    // Reset board to 0
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            board[i][j] = 0;
        }
    }
    // Set coin position
    board[coin->getX()][coin->getY()] = COIN;

    // Go through body and set board accordingly
    auto it = body.begin();
    board[(*it)->getX()][(*it)->getY()] = SNAKE_HEAD;
    it++;
    for(;it!=body.end();it++){
        board[(*it)->getX()][(*it)->getY()] = SNAKE_BODY;
    }

    
    return 0;
}

int SnakeGame::checkWin(){
    return won = (snakeSize == 15 * 15);
}

bool SnakeGame::hasWon(){
    return won;
}

bool SnakeGame::hasDied(){
    return dead;
}

int SnakeGame::turnLeft(){
    // printf("Turn left from %d to %d\n", directionIndex, (directionIndex + 3) % 4);
    return newDirectionIndex = (directionIndex + 3) % 4;
}

int SnakeGame::turnRight(){
    // printf("Turn right from %d to %d\n", directionIndex, (directionIndex + 1) % 4);
    return newDirectionIndex = (directionIndex + 1) % 4;
}

int SnakeGame::turn(int parameter){
    if(parameter == -1){
        return turnLeft();
    }
    else if(parameter == 1){
        return turnRight();
    }
    return 0;
}

int (*SnakeGame::getBoard())[15]{
    return board;
}

int SnakeGame::getSnakeSize(){
    return snakeSize;
}

int SnakeGame::getScore(){
    return score;
}

int SnakeGame::getDirectionIndex(){
    return directionIndex;
}

void SnakeGame::cleanClass(){
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            delete coordinates[i][j];
        }
    }

    delete directions[0];
    delete directions[1];
    delete directions[2];
    delete directions[3];
}

Coordinate *SnakeGame::getCoin(){
    return coin;
}
