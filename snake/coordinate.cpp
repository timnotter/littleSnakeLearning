#include "coordinate.hpp"

Coordinate::Coordinate(){
    int x = 0;
    int y = 0;
}

Coordinate::Coordinate(int x, int y){
    this->x = x;
    this->y = y;
}

int Coordinate::getX(){
    return x;
}

int Coordinate::getY(){
    return y;
}

bool Coordinate::setX(int x){
    this->x = x;
    return 0;
}

bool Coordinate::setY(int y){
    this->y = y;
    return 0;
}