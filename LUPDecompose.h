//
// Created by Dell on 29. 4. 2020.
//

#ifndef ZADANIE_8_LUPDECOMPOSE_H
#define ZADANIE_8_LUPDECOMPOSE_H

#include <iostream>

#include <vector>
#include <cmath>
#include <stdexcept>

#include "Matrix.h"

#define swap(type, a, b) {type _tmp = a; a = b; b = _tmp;}
#define TOLERANCE 1e-20

class LUPDecompose {

    static size_t findMaximumPivotRow(size_t diagonal, std::vector<double*> &index, size_t length);
    inline static void matrixPivotSwap(std::vector<double*> &index, std::vector<size_t> &pivot, size_t rowA, size_t rowB, size_t length);

public:
    static void solveLinearLupDecompose(std::vector<double*> &index, std::vector<size_t> &pivot, size_t length);
    static void LUPInvert(std::vector<double*> &from, std::vector<size_t> &pivot, size_t N, std::vector<double*> &to);
    static double LUPDeterminant(std::vector<double*> &from, std::vector<size_t> &pivot, size_t N);
};


#endif //ZADANIE_8_LUP_H