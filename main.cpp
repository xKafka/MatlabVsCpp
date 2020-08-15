#include <iostream>
#include <chrono>
#include <random>

#include <fstream>

#include "Matrix.h"
#include "stl_matrix.h"
#include "LUP.h"

typedef Matrix<double> Mat;

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 5
#endif

//matlab same code speed comparison
///! using O3 gcc


void makeCMatrix(Mat &matrix)
{
    size_t i = 0;

    const size_t x = 4;
    const size_t y = 9;

    for(size_t p = 0; p < x; ++p)
    {
        for(size_t o = 0; o < y; ++o)
        {
            if(p == x - 1)
            {
                if(o == y - 1)
                {
                    matrix(i - 1, i) = -1.0;
                    matrix(i, i - 1) = -1.0;
                }
                else
                {
                    matrix(i + 1, i) = -1.0;
                    matrix(i, i + 1) = -1.0;
                }
            } else
            {
                if(o == y - 1)
                {
                    matrix(i + 1, i) = 0.0;
                    matrix(i, i + 1) = 0.0;
                }
                else
                {
                    matrix(i + 1, i) = -1.0;
                    matrix(i, i + 1) = -1.0;
                }
            }
            ++i;
        }
    }
}

int main() {

    const double E0 = 8.86e-12;
    const double a = 10;
    const double b = a / 2;
    const double h = 1;
    const size_t Fi_0 = 0;
    const size_t Fi_10 = 10;

    Mat P(36, 1);

    for(size_t from = 27; from < P.rows(); ++from)
    {
        P(from, 0) = 10;
    }

    Mat A(36, 36);

    stl_matrix::eye(A, 4.0, 0);

    Mat C(36, 36);

    makeCMatrix(C);

    size_t k = 36;

    Mat e(k, 1);

    Mat B(36, 36);
    Mat D(36, 36);

    stl_matrix::eye(B, -1.0, -9);
    stl_matrix::eye(D, -1.0, 9);

    C = C + A + B + D;

    LUP lup(C);
    Mat R(k ,k);
    R = lup.LUPInverse()*P;

    Mat F(6, 11);

    size_t t = 0;

    for(size_t rowF = 1; rowF < F.rows() - 1; ++rowF)
    {
        for(size_t colF = 1; colF < F.columns() - 1; ++colF)
        {
            F(rowF, colF) = R(0, t++);
        }
    }

    for(size_t colF = 1; colF < F.columns() - 1; ++colF)
    {
        F(F.rows() - 1, colF) = 10.0;
    }

    //print F

    std::ofstream F_data;

    F_data.open("F_data.txt");

    for(size_t _row = 0; _row < F.rows(); ++_row)
    {
        for(size_t column = 0; column < F.columns(); ++column)
        {
            F_data << F(_row, column) << ' ';
        }

        F_data << '\n';
    }

    Mat Ex(5, 9);


    for(size_t row = 0; row < Ex.rows(); ++row)
    {
        for(size_t column = 0; column < Ex.columns(); ++column)
        {
            Ex(row, column) = F(row + 1, column + 1) - F(row + 1, column + 2);
        }
    }

    Mat Ey(5, 9);

    for(size_t column = 0; column < Ey.columns(); ++column)
    {
        for(size_t row = 0; row < Ey.rows() - 1; ++row)
        {
            Ey(row, column) = F(row + 1, column + 1) - F(row + 2, column + 1);
        }
    }

    stl_matrix::for_each(Ex.begin(), Ex.end(), [](double &value){value *= value ;});
    stl_matrix::for_each(Ey.begin(), Ey.end(), [](double &value){value *= value ;});

    Mat E = Ex + Ey;

    stl_matrix::for_each(E.begin(), E.end(), [](double &value){value = sqrt(value);});

    E(E.rows() - 1, E.columns() - 1) = 0.0;

    std::ofstream E_data;

    E_data.open("E_data.txt");

    for(size_t _row = 0; _row < E.rows(); ++_row)
    {
        for(size_t column = 0; column < E.columns(); ++column)
        {
            E_data << E(_row, column) << ' ';
        }

        E_data << '\n';
    }

    return 0;
}