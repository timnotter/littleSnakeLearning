#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix{
private:
    std::vector<double> data;
    int numberOfRows;
    int numberOfCols;

public:
    // Initialises the matrix with specified size with 0
    Matrix();
    Matrix(int numberOfRows, int numberOfCols);
    ~Matrix();

    int set(int row, int col, double val);
    double get(int row, int col) const;
    int getNumberOfRows() const;
    int getNumberOfCols() const;

    // All operations check if dimensions are correct


    Matrix& operator=(const Matrix& m);
    Matrix& operator*=(const Matrix& m);
    Matrix& operator+=(const Matrix& m);
    Matrix& operator-=(const Matrix& m);
    Matrix& operator*=(double val);
    Matrix& operator/=(double val);
    Matrix& transposeThis();

    Matrix operator*(const Matrix& m) const;
    Matrix operator+(const Matrix& m) const;
    Matrix operator-(const Matrix& m) const;
    Matrix operator*(double val) const;
    Matrix operator/(double val) const;
    Matrix transpose() const;
    double dotProduct(Matrix& m) const;

    bool operator==(const Matrix& m) const;

    int print();
};

#endif