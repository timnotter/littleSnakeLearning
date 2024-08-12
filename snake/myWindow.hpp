#include "../../../../../../usr/include/x86_64-linux-gnu/sys/types.h"
#include <stdio.h>
#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#define KEY_ESCAPE 9
#define KEY_SPACE 65
#define KEY_A 38
#define KEY_D 40
#define KEY_J 44
#define KEY_L 46


class MyWindow{
private:
    // Used for X11 on Linux. They are all void*, such that we don't have to forward declare types. These void* can possibly reused by the windows graphics API (To be done)
    void *display;
    void *window;
    int screen;
    void *event;
    void *gc;
    void *backBuffer;
    // Used to store information about the window
    void *rootWindow;
    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int borderWidth;
    unsigned int depth;
    int tempX;
    int tempY;

public:
    // Should not be accessed
    MyWindow();
    MyWindow(int width, int height);
    
    void drawBackground(int colour);
    // SHOULD be called after a function finished drawing. It swaps currently displayed and newly drawn picture
    void endDrawing();

    // Draw point
    int drawPoint(unsigned int col, int x, int y);
    // Draws line
    int drawLine(unsigned int col, int x1, int y1, int x2, int y2);
    // Draws rect
    int drawRect(unsigned int col, int x, int y, int width, int height);
    // Draws circle
    int drawCircle(unsigned int col, int x, int y, int diam);
    // Draws string
    int drawString(unsigned int col, int x, int y, const char *stringToBe);
    // Draws triangle
    int drawTriangle(unsigned int col, int x1, int y1, int x2, int y2, int x3, int y3);
    // Draws polygon
    // int drawPolygon(unsigned int col, short count, Point2d *points);

    // SHOULD be called after finishing execution. Cleans up object
    void closeWindow();

    int getNumberOfPendingEvents();
    // Takes as input two arraypointers and their size. Writes type of event into eventTypes array, and the keycodes into parameters array, if event was a keypress. 
    // If total number of pending events is smaller than numberOfEvents, it writes -1 into the first entry of the evenTypes array
    void getPendingEvents(int *eventTypes, u_int *parameters, int numberOfEvents);

    int handleEvents(int *var);

    // These functions should be platform independent

    int getWindowWidth();
    int getWindowHeight();
};

#endif