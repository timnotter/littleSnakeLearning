#include <ctime>
#include <time.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <string>
#include "main.hpp"
#include "snakeGame.hpp"
#include "renderer.hpp"
#include "matrix.hpp"
#include "ai.hpp"

// TODO: Error in update loop. Not in forward function

int main(int argc, char** argv){
    int numberOfAI = 0;
    bool activateAI = false;
    if(argc == 2){
        // startAI(std::stoi(argv[1]));
        numberOfAI = std::stoi(argv[1]);
        printf("We start game with %d ais\n", numberOfAI);
        activateAI = true;
    }

    // Define characteristics of every ai that is used
    int size[3];
    size[0] = 800;
    size[1] = 400;
    size[2] = 3;
    int activationFunction[3];
    activationFunction[0] = RELU;
    activationFunction[1] = RELU;
    activationFunction[2] = SOFTMAX;
    // AI ai = AI((15 * 15 + 4), 3, size, activationFunction);

    // Initialise all ais with corresponding games
    std::vector<AI> ais = std::vector<AI>();
    std::vector<SnakeGame> games = std::vector<SnakeGame>();
    if(activateAI){
        for(int i=0;i<numberOfAI;i++){
            // printf("Adding ai\n");
            ais.push_back(AI((15 * 15 + 4), 3, size, activationFunction));
            // printf("Adding game\n");
            games.push_back(SnakeGame());
            // printf("Addes game and ai. New size of both is: %ld\n", games.size());
            if(++i%100 == 0){
                printf("Added games and ais. New size of both is: %ld\n", games.size());
            }
        }
    }

    // If no ai is used, we initialise one game for the player
    if(!activateAI){
        games.push_back(SnakeGame());
    }
    
    int scores[games.size()];
    for(int i=0;i<games.size();i++){
        scores[i] = 0;
    }

    // Initialise myWindow
    MyWindow myWindow = MyWindow(650, 600);

    // Initialise renderer
    Renderer renderer = Renderer(&myWindow);

    // We draw the first game, so that something showes up
    renderer.draw(&(games[0]));
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	renderer.draw(&(games[0]));

    // Gameloop
    bool running = true;
    bool paused = true;

    //Every tenth time we update, else we just draw. This is because keypresses are only recognized 2 steps after
    int counter = 0;
    // int maxCounter = 10;
    int maxCounter = 1;

    struct timespec prevTime;
	struct timespec currTime;
    // Times in nano seconds
	int updateTime;
    // int optimalUpdateTime = 500000 / maxCounter;
    int optimalUpdateTime = 100000 / maxCounter;
    int timeDifference;
    clock_gettime(0, &prevTime);

    // Here store the index of the game we draw
    int indexToDraw = 0;
    while(running){
        // Handle key events
        // printf("Events: %d\n", myWindow.getNumberOfPendingEvents());
        if(myWindow.getNumberOfPendingEvents() > 0){
            // First var: -1 means turn left, 1 means turn right, 0 means nothing
            // Second var: 1 means pause/unpause, 0 means nothing
            // Third var: 1 means escape, 0 means nothing
            int events[3];
            events[0] = 0;
            events[1] = 0;
            events[2] = 0;

            myWindow.handleEvents(events);
            if(!activateAI){
                // If we have no ai, we have one game
                if(events[0] == -1){
                    games[0].turnLeft();
                }
                if(events[0] == 1){
                    games[0].turnRight();
                }
            }
            if(events[1] == 1){
                paused = !paused;
            }
            if(events[2] == 1){
                running = false;
            }
        }

        if(counter == 0){
            if(!paused){
                // In every iteration we go over all games, make a step, update scores
                for(int i=0;i<games.size();i++){
                    printf("Updating %d\n", i);
                    if(games[i].hasWon() || games[i].hasDied()){
                        continue;
                    }

                    if(!(i==0 && !activateAI)){
                        // If we are not at the first game or the ai has been activated, we make a forward step for the ai. Else we just update
                        // First we build the input Matrix
                        printf("Assembling input\n");
                        Matrix input = Matrix(15 * 15 + 4, 1);
                        for(int i=0;i<15;i++){
                            for(int j=0;j<15;j++){
                                input.set(i * 15 + j, 0, games[i].getBoard()[i][j]);
                            }
                        }
                        printf("Adding directions to input\n");

                        switch(games[i].getDirectionIndex()){
                            case 0:
                                input.set(15 * 15, 0, 1);
                                break;
                            case 1:
                                input.set(15 * 15 + 1, 0, 1);
                                break;
                            case 2:
                                input.set(15 * 15 + 2, 0, 1);
                                break;
                            case 3:
                                input.set(15 * 15 + 3, 0, 1);
                                break;
                        }

                        // Then we make the forward pass
                        printf("In forward\n");
                        ais[i].forward(input);
                        printf("Out forward\n");

                        // And last, we look at the output. Largest one wins out
                        Matrix output = ais[i].getOutput();
                        
                        if(output.get(0, 0) >= output.get(1, 0) && output.get(0, 0) >= output.get(2, 0)){
                            games[i].turnLeft();
                            printf("Ai wants to turn Left\n");
                        }
                        else if(output.get(2, 0) >= output.get(0, 0) && output.get(2, 0) >= output.get(1, 0)){
                            games[i].turnRight();
                            printf("Ai wants to turn Right\n");
                        }
                        else{
                            printf("Ai wants to go straight\n");
                        }
                    }
                    // Update the game and the scoreboard
                    printf("Trying to update\n");
                    games[i].update();
                    printf("Updating score\n");
                    scores[i] = games[i].getScore();
                    // printf("Updated %d\n", i);
                }
                printf("Updated all\n");
            }
        }
        counter = (counter + 1) % maxCounter;
        
        // Here we draw the game with the largest score
        for(int i=0;i<games.size();i++){
            if(scores[i] > scores[indexToDraw]){
                indexToDraw = i;
            }
        }
        renderer.draw(&(games[indexToDraw]));

        // Wait for appropriate time
        clock_gettime(0, &currTime);
        // Update time in milli seconds
        updateTime = ((1000000000*(currTime.tv_sec-prevTime.tv_sec)+(currTime.tv_nsec-prevTime.tv_nsec))/1000);
        timeDifference = optimalUpdateTime-updateTime;
        if(timeDifference > 0){
            std::this_thread::sleep_for(std::chrono::microseconds(timeDifference));
        }
        clock_gettime(0, &prevTime);
    }

    // Game has ended

    //Close everything
    myWindow.closeWindow();
    
    return 0;
}

void startAI(int numberOfAIInstances){
    printf("Starting AI thingy with %d instances\n", numberOfAIInstances);

    int size[3];
    size[0] = 800;
    size[1] = 400;
    size[2] = 3;

    int activationFunction[3];
    activationFunction[0] = RELU;
    activationFunction[1] = RELU;
    activationFunction[2] = SOFTMAX; 

    AI ai = AI((15 * 15 + 4), 3, size, activationFunction);
    // ai.print();
}   