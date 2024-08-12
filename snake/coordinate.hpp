#ifndef COORDINATE_HPP
#define COORDINATE_HPP

class Coordinate{
    private:
        int x;
        int y;
    
    public:
        Coordinate();
        Coordinate(int x, int y);

        int getX();
        int getY();

        bool setX(int x);
        bool setY(int y);
};

#endif