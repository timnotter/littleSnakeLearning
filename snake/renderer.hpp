#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "myWindow.hpp"
#include "snakeGame.hpp"

class Renderer{
private:
    MyWindow *myWindow;

public:
    Renderer(MyWindow *myWindow);

    int draw(SnakeGame *game);
};

#endif