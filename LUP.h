//
// Created by Dell on 29. 4. 2020.
//
#ifndef ZADANIE_8_LUP_H
#define ZADANIE_8_LUP_H

#include <vector>
#include <cmath>
#include <iostream>

#include "stl_matrix.h"
#include "Matrix.h"
#include "LUPDecompose.h"

#define swap(type, a, b) {type _tmp = a; a = b; b = _tmp;}

class LUP {

    std::vector<double *> asIndexVector(Matrix<double > &from);

public:
    explicit LUP(const Matrix<double>& from);

    Matrix<double> LUPInverse();
    double LUPDeterminant();

private:
    Matrix<double> result;
    size_t length;
    std::vector<double *> index;
    std::vector<size_t > pivot;
};

#endif //ZADANIE_8_LUP_H