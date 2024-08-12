#include "ai.hpp"
#include <cmath>
#include <random>
#include <cassert>

AI::AI(int inputSize, int layerCount, int *layerSize, int *activationFunction){
    if(inputSize <= 0 || layerCount <= 0){
        printf("Invalid AI constructor");
        assert(inputSize <= 0 || layerCount <= 0);
    }

    this->inputSize = inputSize;
    this->layerCount = layerCount;

    this->layerSize = std::vector<int>(); 
    for(int i=0;i<layerCount;i++){
        this->layerSize.push_back(layerSize[i]); 
    }

    this->activationFunction = std::vector<int>();
    for(int i=0;i<layerCount;i++){
        this->activationFunction.push_back(activationFunction[i]); 
    }

    // Initialise all weights
    weights = std::vector<Matrix>();
    weights.push_back(Matrix(layerSize[0], inputSize));

    for(int i=1;i<layerCount;i++){
        weights.push_back(Matrix(layerSize[i], layerSize[i-1]));
    }

    std::vector<Matrix> intermediateValues;
    initialiseWeightsMatrix();
}

AI::~AI(){}

int AI::forward(double *input){
    Matrix inputMatrix(inputSize, 1);
    for(int i=0;i<inputSize;i++){
        inputMatrix.set(i, 0, input[i]);
    }
    return forward(inputMatrix);
}

int AI::forward(Matrix input){
    if(input.getNumberOfCols() != 1 || input.getNumberOfRows() != inputSize){
        printf("Invalid dimension of input in forward function\n");
        assert(input.getNumberOfCols() != 1 || input.getNumberOfRows() != inputSize);
    }

    intermediateValues.clear();
    intermediateValues.push_back(weights[0] * input);
    applyActivationFunction(0);
    
    for(int i=1;i<layerCount;i++){
        intermediateValues.push_back(weights[i] * intermediateValues[i-1]);
        applyActivationFunction(i);
    }
    
    return 0;
}

int AI::applyActivationFunction(int index){
    int currActivationFunction = activationFunction[index];
    int currLayerSize = layerSize[index];
    if(currActivationFunction == SOFTMAX){
        // This means we have reached the output. The Softmax has to be treated a bit differently, 
        // since it uses the whole layer as input
        double total = 0;
        for(int i=0;i<currLayerSize;i++){
            total += intermediateValues[index].get(i, 0);
        }
        for(int i=0;i<currLayerSize;i++){
            intermediateValues[index].set(i, 0, intermediateValues[index].get(i, 0) / total);
        }

        return 0;
    }

    // Else just apply the function
    for(int i=0;i<currLayerSize;i++){
        intermediateValues[index].set(i, 0, getActivationFunctionValue(intermediateValues[index].get(i, 0), currActivationFunction));
    }

    return 0;
}


double AI::getActivationFunctionValue(double value, int function){
    switch(function){
        case RELU:
            return std::max(0.0, value);
        case SIGMOID:
            return 1.0 / (1 + exp(-value));
        case TANH:
            return (exp(value) - exp(-value)) / (exp(value) + exp(-value));
        case LEAKY_RELU:
            return value>=0 ? value : value * LEAKY_RELU_PARAM;
        case LINEAR:
        default:
            return value;
    }
}

int AI::initialiseWeightsMatrix(){
    for(int i=0;i<layerCount;i++){
        // Here we could use different initialisations for different activation functions
        switch(activationFunction[i]){
            default:
                xavierInitialisation(&(weights[i]));
                break;
        }
    }

    return 0;
}

int AI::heInitialisation(Matrix *m){
    int rowCount = m->getNumberOfRows();
    int colCount = m->getNumberOfCols();
    // Standard deviation for He initialization
    double stddev = std::sqrt(2.0 / colCount);

    // Random number generator and normal distribution
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, stddev);

    for (int i = 0;i<rowCount;i++) {
        for (int j = 0;j<colCount;j++) {
            m->set(i, i, distribution(generator));
        }
    }

    return 0;
}

int AI::xavierInitialisation(Matrix *m){
    int rowCount = m->getNumberOfRows();
    int colCount = m->getNumberOfCols();
    // printf("Xavier with %dx%d\n", rowCount, colCount);
    double limit = std::sqrt(6.0 / (rowCount + colCount));
    
    // Create a random number generator for uniform distribution
    std::random_device rd;  // Seed for the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<> dis(-limit, limit); // Uniform distribution
    
    // Initialize weights
    for (int i = 0;i<rowCount;i++) {
        for (int j = 0;j<colCount;j++) {
            m->set(i, j, dis(gen));
        }
    }

    return 0;
}

int AI::adjustWeights(){
    for(int i=0;i<weights.size();i++){

        for(int j=0;j<weights[i].getNumberOfRows();j++){
            double stddev = std::sqrt(2.0 / weights[i].getNumberOfCols());
            for(int k=0;k<weights[i].getNumberOfCols();k++){
                // Random number generator and normal distribution
                std::default_random_engine generator(std::random_device{}());
                std::normal_distribution<double> distribution(weights[i].get(j, k), stddev);
                weights[i].set(j, k, distribution(generator));
            }
        }
    }
    return 0;
}

Matrix AI::getOutput(){
    // printf("Returning output of size (%dx%d)\n", weights.back().getNumberOfRows(), weights.back().getNumberOfCols());
    // weights.back().print();
    return weights.back();
}

int AI::print(){
    for(int i=0;i<layerCount;i++){
        printf("Weightmatrix %d: \n", i);
        weights[i].print();
    }

    return 0;
}