//
// Created by Dell on 29. 4. 2020.
//
#include "LUPDecompose.h"

void LUPDecompose::solveLinearLupDecompose(std::vector<double*> &index, std::vector<size_t> &pivot, size_t length) {

    for(size_t i = 0; i < pivot.size(); ++i)
        pivot[i] = i;

    for(size_t diagonal = 0; diagonal < length; ++diagonal) {
        size_t pivotRow = findMaximumPivotRow(diagonal, index, length);

        if (pivotRow != diagonal)
             matrixPivotSwap(index, pivot, diagonal, pivotRow, length);

        for (size_t row = diagonal + 1; row < length; ++row) {
               index.at(row)[diagonal] /= index.at(diagonal)[diagonal];

            for (size_t column = diagonal + 1; column < length; ++column)
                index.at(row)[column] -= index.at(row)[diagonal] * index.at(diagonal)[column];

        }
    }
}
size_t LUPDecompose::findMaximumPivotRow(size_t diagonal, std::vector<double *> &index, size_t length) {

    double maximum = 0.0;
    size_t maximumRow = diagonal;

    for(size_t row = diagonal; row < length; ++row) {
        const double absolute = fabs(index[row][diagonal]);

        if(absolute > maximum) {
            maximum = absolute;
            maximumRow = row;
        }
    }

    if(maximum < TOLERANCE)
        throw new std::runtime_error("LUPDecompose::solveLinearLupDecompose pivot out of tolerance");


    return maximumRow;
}
void LUPDecompose::matrixPivotSwap(std::vector<double *> &index, std::vector<size_t> &pivot, size_t rowA, size_t rowB,
        size_t length) {

    swap(int, pivot[rowA], pivot[rowB])
    swap(double *, index[rowA], index[rowB])

    // For determinant
    ++pivot[length];
}
void LUPDecompose::LUPInvert(
        std::vector<double *> &from,
        std::vector<size_t> &pivot,
        size_t N, std::vector<double *> &to) {


    ///Wikipedia

    for (size_t j = 0; j < N; j++) {
        for (size_t i = 0; i < N; i++) {
            if (pivot[i] == j)
                to[i][j] = 1.0;
            else
                to[i][j] = 0.0;

            for (size_t k = 0; k < i; k++)
                to[i][j] -= from[i][k] * to[k][j];
        }

        for (int i = N - 1; i >= 0; i--) {
            for (size_t k = i + 1; k < N; k++)
                to[i][j] -= from[i][k] * to[k][j];

            to[i][j] = to[i][j] / from[i][i];
        }
    }
}
double LUPDecompose::LUPDeterminant(std::vector<double *> &from, std::vector<size_t> &pivot, size_t N)
{
    double det = from[0][0];

    std::cout << N << '\n';

    for (int i = 1; i < N ; ++i)
        det *= from[i][i];

    if ((pivot[N] - N) % 2 == 0)
        return det;
    else
        return -det;
}
