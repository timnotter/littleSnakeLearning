#ifndef AI_HPP
#define AI_HPP

#include <vector>
#include "matrix.hpp"

#define RELU 0
#define SIGMOID 1
#define TANH 2
#define LEAKY_RELU 3
#define LEAKY_RELU_PARAM 0.01
#define LINEAR 4

// This can be used to transform outputs into probabilities.
#define SOFTMAX 5

class AI{
private:
int inputSize;
// Specifies the number of layers, the last one being
int layerCount;
std::vector<int> layerSize;
std::vector<int> activationFunction;
std::vector<Matrix> weights;
// The intermediate values are stored here. IN the last matrix, we have the output
std::vector<Matrix> intermediateValues;



public:
    AI(int inputSize, int layerCount, int *layerSize, int *activationFunction);
    ~AI();

    // Here we assume that input is an array with size inputSize. We do not enforce this
    int forward(double *input);
    int forward(Matrix input);
    // Takes as input an index, meaning which index of intermediateValue we are dealing with
    // According to this the specified activation function is applied
    int applyActivationFunction(int index);
    // Return the value after applying the function to the input
    double getActivationFunctionValue(double value, int function);

    // This function sets the initial weights for the matrix to random numbers.
    // It is currently always using xavier
    int initialiseWeightsMatrix();
    // The mean should be 0 and the variance is 2/layerSize
    int heInitialisation(Matrix *m);
    // This is a uniform distribution between [-6/(layerInput + layerOutput), 6/(layerInput + layerOutput)]
    int xavierInitialisation(Matrix *m);
    // We go through every weight and adjust it with a gaussian with mean 0
    int adjustWeights();

    Matrix getOutput();

    int print();
};

#endif