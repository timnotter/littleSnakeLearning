#include <stdio.h>
#include <cassert>
#include "matrix.hpp"

Matrix::Matrix() : Matrix(1,1){}

Matrix::Matrix(int numberOfRows, int numberOfCols){
    if(numberOfRows <= 0 || numberOfCols <= 0){
        printf("Invalid matrix dimension for initialisation\n");
        assert(numberOfRows > 0 && numberOfCols > 0);
        return;
    }

    this->numberOfRows = numberOfRows;
    this->numberOfCols = numberOfCols;

    // The whole data is initialised to 0
    data = std::vector<double>(numberOfRows * numberOfCols);
    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, 0);
        }
    }
    // printf("Finished initialisation of this matrix\n");
}

Matrix::~Matrix(){

}

int Matrix::set(int row, int col, double val){
    return data[row * numberOfCols + col] = val;
}

double Matrix::get(int row, int col) const{
    return data[row * numberOfCols + col];
}

int Matrix::getNumberOfRows() const{
    return numberOfRows;
}

int Matrix::getNumberOfCols() const{
    return numberOfCols;
}

// All operations check if dimensions are correct


Matrix& Matrix::operator=(const Matrix& m){
    data.clear();
    numberOfRows = m.getNumberOfRows();
    numberOfCols = m.getNumberOfCols();
    std::vector<std::vector<double>> data(numberOfRows, std::vector<double>(numberOfCols));
    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, m.get(i, j));
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& m){
    if(numberOfCols != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for multiplication\n");
        assert(!(numberOfCols != m.getNumberOfRows()));
    }

    int newNumberOfRows = numberOfRows;
    int newNumberOfCols = m.getNumberOfCols();
    std::vector<double> newData(newNumberOfRows * newNumberOfCols);


    for(int i=0;i<newNumberOfRows;i++){
        for(int j=0;j<newNumberOfCols;j++){
            
            for(int k=0;k<numberOfCols;k++){
                newData[i * newNumberOfCols + j] += get(i, k) * m.get(k, j);
            }
        }
    }

    data = newData;

    return *this;
}

Matrix& Matrix::operator+=(const Matrix& m){
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for addition\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()));
    }

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, get(i, j) + m.get(i, j));
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& m){
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for subtraction\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()));
    }

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, get(i, j) - m.get(i, j));
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(double val){
    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, get(i, j) * val);
        }
    }

    return *this;
}

Matrix& Matrix::operator/=(double val){
    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            set(i, j, get(i, j) / val);
        }
    }

    return *this;
}

Matrix& Matrix::transposeThis(){
    std::vector<double> newData(numberOfRows * numberOfCols);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newData[i * numberOfCols + j] = get(j, i);
        }
    }

    data = newData;

    return *this;
}

Matrix Matrix::operator*(const Matrix& m) const{
    if(numberOfCols != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for multiplication. Trying (%dx%d) * (%dx%d)\n", numberOfRows, numberOfCols, m.getNumberOfRows(), m.getNumberOfCols());
        assert(!(numberOfCols != m.getNumberOfRows()));
    }

    int newNumberOfRows = numberOfRows;
    int newNumberOfCols = m.getNumberOfCols();
    Matrix newMatrix = Matrix(newNumberOfRows, newNumberOfCols);

    for(int i=0;i<newNumberOfRows;i++){
        for(int j=0;j<newNumberOfCols;j++){
            double value = 0;
            for(int k=0;k<numberOfCols;k++){
                value += get(i, k) * m.get(k, j);
            }
            newMatrix.set(i, j, value);
        }
    }

    return newMatrix;
}

Matrix Matrix::operator+(const Matrix& m) const{
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for addition\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()));
    }
    Matrix newMatrix = Matrix(numberOfRows, numberOfCols);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newMatrix.set(i, j, get(i, j) + m.get(i, j));
        }
    }

    return newMatrix;
}

Matrix Matrix::operator-(const Matrix& m) const{
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for subtraction\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()));
    }
    Matrix newMatrix = Matrix(numberOfRows, numberOfCols);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newMatrix.set(i, j, get(i, j) - m.get(i, j));
        }
    }

    return newMatrix;
}

Matrix Matrix::operator*(double val) const{
    Matrix newMatrix = Matrix(numberOfRows, numberOfCols);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newMatrix.set(i, j, get(i, j) * val);
        }
    }

    return newMatrix;
}

Matrix Matrix::operator/(double val) const{
    Matrix newMatrix = Matrix(numberOfRows, numberOfCols);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newMatrix.set(i, j, get(i, j) / val);
        }
    }

    return newMatrix;
}

Matrix Matrix::transpose() const{
    Matrix newMatrix = Matrix(numberOfCols, numberOfRows);

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            newMatrix.set(i, j, get(j, i));
        }
    }

    return newMatrix;
}

double Matrix::dotProduct(Matrix& m) const{
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != 1 || m.getNumberOfRows() != 1){
        printf("Invalid matrix dimensions for dotProduct\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != 1 || m.getNumberOfRows() != 1));
        return 0;
    }

    double value = 0;

    for(int i=0;i<numberOfCols;i++){
        value += get(i, 0) * m.get(i, 0);
    }

    return value;
}

bool Matrix::operator==(const Matrix& m) const{
    if(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()){
        printf("Invalid matrix dimensions for dotProduct\n");
        assert(!(numberOfCols != m.getNumberOfCols() || numberOfRows != m.getNumberOfRows()));
        return false;
    }

    for(int i=0;i<numberOfRows;i++){
        for(int j=0;j<numberOfCols;j++){
            if(get(i, j) != m.get(i, j)){
                return false;
            }
        }
    }

    return true;
}

int Matrix::print(){
    // printf("Printing matrix with dimension %dx%d\n", numberOfRows, numberOfCols);
    // printf("(%d, %d)\n", 0, 0);
    // printf("[[%f\n", data[0][0]);
    // for(int j=1;j<numberOfCols;j++){
    //     printf("(%d, %d)\n", 0, j);
    //     printf(", %f\n", data[0][j]);
    // }
    // printf("]");

    // for(int i=1;i<numberOfRows;i++){
    //     printf("(%d, %d)\n", i, 0);
    //     printf("\n[%f\n", data[i][0]);
    //     for(int j=1;j<numberOfCols;j++){
    //         printf("(%d, %d)\n", 0, j);
    //         printf(", %f\n", get(i, j));
    //     }
    //     printf("]");
    // }

    // printf("]\n");

    printf("Printing matrix with dimension %dx%d\n", numberOfRows, numberOfCols);
    printf("[[%f", get(0, 0));
    for(int j=1;j<numberOfCols;j++){
        printf(", %f", get(0, j));
    }
    printf("]");

    for(int i=1;i<numberOfRows;i++){
        printf("\n[%f", get(i, 0));
        for(int j=1;j<numberOfCols;j++){
            printf(", %f", get(i, j));
        }
        printf("]");
    }

    printf("]\n");
    return 0;
}