//
// Created by Dell on 29. 4. 2020.
//
#include "LUP.h"

LUP::LUP(const Matrix<double>& from)
        : result(from),
          length(from.rows()),
          pivot(from.rows() + 1),
          index(asIndexVector(result))
{
    LUPDecompose::solveLinearLupDecompose(index, pivot, length);
}
std::vector<double *> LUP::asIndexVector(Matrix<double> &from) {

    std::vector<double *> rowPointers(length);

        for(size_t row = 0; row < from.rows(); ++row)
            rowPointers[row] = &from(row, 0);

    return rowPointers;
}
Matrix<double> LUP::LUPInverse() {

    Matrix<double> to(result.rows(), result.columns());
    std::vector<double*> toIndex = asIndexVector(to);

    LUPDecompose::LUPInvert(index, pivot, toIndex.size(), toIndex);

    return to;
}
double LUP::LUPDeterminant() {

    return LUPDecompose::LUPDeterminant(index, pivot, index.size());
}



