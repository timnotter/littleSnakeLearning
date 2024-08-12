#include "renderer.hpp"
#include <string>

Renderer::Renderer(MyWindow *myWindow){
    this->myWindow = myWindow;
}

int Renderer::draw(SnakeGame *game){
    int width = myWindow->getWindowWidth();
    int height = myWindow->getWindowHeight();
    // printf("Width: %d, height: %d\n", width, height);
    int topBarHeight = 50;
    int gridSize = std::min(width, height-topBarHeight);
    int fieldSize = gridSize / 15;
    gridSize = fieldSize * 15;
    // printf("GridSize: %d, fieldSize: %d\n", gridSize, fieldSize);

    // Draw background
    myWindow->drawBackground(0x555555);

    // Draw top bar
    myWindow->drawRect(0x999999, 0, 0, width, topBarHeight);
    std::string toDisplay = std::to_string(game->getScore());
    toDisplay.insert(0, "Score: ");
    myWindow->drawString(0, 10, 30, toDisplay.c_str());

    // Draw grid
    int (*board)[15] = game->getBoard();
    
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            switch(board[i][j]){
                case EMPTY:
                    break;
                case SNAKE_HEAD:
                    myWindow->drawRect(0x00AA00, i * fieldSize, topBarHeight + j * fieldSize, fieldSize, fieldSize);
                    // printf("Drawing snakeHead at (%d, %d)\n", i, j);
                    break;
                case SNAKE_BODY:
                    myWindow->drawRect(0x00FF00, i * fieldSize, topBarHeight + j * fieldSize, fieldSize, fieldSize);
                    // printf("Drawing snakeBody at (%d, %d)\n", i, j);
                    break;
                case COIN:
                    myWindow->drawRect(0xFFD700, i * fieldSize, topBarHeight + j * fieldSize, fieldSize, fieldSize);
                    // printf("Drawing Coin at (%d, %d)\n", i, j);
                    break;
            }
        }
    }

    myWindow->endDrawing();

    return 0;
}