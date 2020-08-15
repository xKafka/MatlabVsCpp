//
// Created by Dell on 27. 4. 2020.
//

#ifndef ZADANIE_8_STL_MATRIX_H
#define ZADANIE_8_STL_MATRIX_H

#include <algorithm>
#include <iostream>
#include <random>

#include "Matrix.h"

struct stl_matrix {

    template <typename D>
    static inline Matrix<D> &delete_column(Matrix<D> &from, size_t columnToDelete);

    template <typename D>
    static inline Matrix<D> &delete_row(Matrix<D> &from, size_t rowToDelete);

    template <typename  D>
    static Matrix<D> get_column(Matrix<D> &from, size_t fromColumn);

    template <typename D>
    static Matrix<D> get_row(Matrix<D> &from, size_t fromRow);

    template <typename  D>
    static Matrix<D> get_column(const Matrix<D> &from, size_t fromColumn);

    template <typename D>
    static Matrix<D> get_row(const Matrix<D> &from, size_t fromRow);

    template <typename D>
    static inline Matrix<D> &swap_rows(Matrix<D> &from, size_t fromRow, size_t toRow);

    template <typename D>
    static inline Matrix<D> &swap_columns(Matrix<D> &from, size_t fromColumn, size_t toColumn);

    template <typename D>
    static Matrix<D>  &insert_row(Matrix<D> &to, Matrix<D> &from, size_t toPosition);

    template <typename D>
    static Matrix<D>  &insert_row(Matrix<D> &to, std::vector<D> &from, size_t toPosition);

    template <typename D>
    static Matrix<D>  &insert_column(Matrix<D> &to, Matrix<D> &from, size_t toPosition);

    template <typename D>
    static Matrix<D>  &insert_column(Matrix<D> &to, std::vector<D> &from, size_t toPosition);

    template <typename D, typename F>
    static inline void for_each(D *begin, D *end, F function);

    template <typename D, typename F>
    static inline void for_each(const D *begin, const D *end, F function);

    template <typename D>
    static inline Matrix<D> &negate(Matrix<D> &what);

    template <typename D>
    static inline Matrix<D> &random(Matrix<D> &to, D min, D max);

    template <typename D>
    static inline Matrix<D> &transpose(Matrix<D> &from);

    template <typename D>
    static Matrix<D> &inverse(Matrix<D> &from);

    template <typename D>
    static inline Matrix<D> &move_row(Matrix<D> &to, size_t toRow, Matrix<D> &from, size_t fromRow);

    template<typename D>
    static inline Matrix<D> &eye(Matrix<D> &to, D toEye, int diagonal);

    template<typename D>
    static inline Matrix<D> &flip(Matrix<D> &toFlip);
};
template <typename D>
Matrix<D> &stl_matrix::delete_column(Matrix<D> &from, size_t columnToDelete)
{
    Matrix<D> result(from.rows(), from.columns() - 1);

    from.iterate([&columnToDelete, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(column < columnToDelete)
            result(row, column) = value;
        if(column > columnToDelete)
            result(row, column - 1) = value;
    });

    return (from = std::move(result));
}
template<typename D>
Matrix<D> &stl_matrix::delete_row(Matrix<D> &from, size_t rowToDelete) {

    Matrix<D> result(from.rows() - 1, from.columns());

    from.iterate([&rowToDelete, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(row < rowToDelete)
            result(row, column) = value;
        if(row > rowToDelete)
            result(row - 1, column) = value;
    });

    return (from = std::move(result));
}
template<typename D>
Matrix<D> stl_matrix::get_column(Matrix<D> &from, size_t fromColumn) {

    Matrix<D> result(1, from.rows());

    from.iterate([&fromColumn, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(column == fromColumn)
            result(0, row) = value;
    });

    return result;
}
template<typename D>
Matrix<D> stl_matrix::get_row(Matrix<D> &from, size_t fromRow) {

    Matrix<D> result(1, from.columns());

    from.iterate([&fromRow, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(row == fromRow)
            result(0, column) = value;
    });

    return result;
}
template<typename D>
Matrix<D> stl_matrix::get_column(const Matrix<D> &from, size_t fromColumn) {

    Matrix<D> result(1, from.rows());

    from.iterate([&fromColumn, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(column == fromColumn)
            result(0, row) = value;
    });

    return result;
}
template<typename D>
Matrix<D> stl_matrix::get_row(const Matrix<D> &from, size_t fromRow) {

    Matrix<D> result(1, from.columns());

    from.iterate([&fromRow, &result](Matrix<D> &, size_t row, size_t column, D &value) {
        if(row == fromRow)
            result(0, column) = value;
    });

    return result;
}
template<typename D>
Matrix<D> &stl_matrix::swap_rows(Matrix<D> &from, const size_t fromRow, const size_t toRow) {

    D *fromItr = from.begin() + from.columns() * fromRow;
    D *toItr = from.begin() + from.columns() * toRow;
    size_t column = from.columns();

    while(column--)
    {
        D value = *toItr;
        *toItr++ = *fromItr;
        *fromItr++ = value;
    }

    return from;
}
template<typename D>
Matrix<D> &stl_matrix::swap_columns(Matrix<D> &from, size_t fromColumn, size_t toColumn) {

    D *fromItr = from.begin() + fromColumn;
    D *toItr = from.begin() + toColumn;
    const size_t columns = from.columns();
    size_t rows = from.rows();

    while(rows--)
    {
        D value = *toItr;
        *toItr = *fromItr;
        *fromItr = value;
        toItr += columns;
        fromItr += columns;
    }

    return from;
}
template<typename D>
Matrix<D> &stl_matrix::insert_row(Matrix<D> &to, Matrix<D> &from, size_t toPosition) {

    if(to.rows() not_eq from.columns())
        throw new std::runtime_error("Matrix::insert_row column size mismatch");

    to.m_rows++;

    size_t itr = to.m_columns * toPosition;

    for(size_t column = 0; column < from.columns(); ++column)
        to.m_data.insert(to.m_data.begin() + itr++, from(0,column));

    return to;
}
template<typename D>
Matrix<D> &stl_matrix::insert_row(Matrix<D> &to, std::vector<D> &from, size_t toPosition) {

    if(to.rows() not_eq from.size())
        throw new std::runtime_error("Matrix::insert_row column size mismatch");

    to.m_rows++;

    size_t itr = to.m_columns * toPosition;

    for(size_t column = 0; column < from.size(); ++column)
        to.m_data.insert(to.m_data.begin() + itr++, from.at(column));

    return to;
}
template<typename D>
Matrix<D> &stl_matrix::insert_column(Matrix<D> &to, Matrix<D> &from, size_t toPosition) {

    ++to.m_columns;

    for(size_t column = 0, itr = toPosition; column < from.columns(); ++column, itr += to.m_columns)
        to.m_data.insert(to.m_data.begin() + itr, from(0,column));
}
template<typename D>
Matrix<D> &stl_matrix::insert_column(Matrix<D> &to, std::vector<D> &from, size_t toPosition) {

    ++to.m_columns;

    for(size_t column = 0, itr = toPosition; column < from.size(); ++column, itr += to.m_columns)
        to.m_data.insert(to.m_data.begin() + itr, from.at(column));
}
template<typename D, typename F>
void stl_matrix::for_each(const D *begin, const D *end, F function) {

    for(D *itr = begin; itr < end; ++itr)
        function(*itr);
}
template<typename D, typename F>
void stl_matrix::for_each(D *begin, D *end, F function) {

    for(D *itr = begin; itr < end; ++itr)
        function(*itr);
}
template<typename D>
Matrix<D> &stl_matrix::negate(Matrix<D> &what) {

    for_each(what.begin(), what.end(), [](D &value){value = -value; });

    return what;
}
template<typename D>
Matrix<D> &stl_matrix::random(Matrix<D> &to, D min, D max) {

    std::random_device rnd_device;

    std::mt19937 mrs_engine {rnd_device()};

    std::uniform_real_distribution<D> dist {min, max};

    //auto gen =[&dist, &mrs_engine](){return dist(mrs_engine);};

    for_each(to.begin(), to.end(), [&dist, &mrs_engine](double &value){value = dist(mrs_engine);});

    return to;
}
template<typename D>
Matrix<D> &stl_matrix::transpose(Matrix<D> &from) {

    Matrix<D> result(from.columns(), from.rows());

    from.iterate([&result](Matrix<D> &, size_t row, size_t column, D &value){ result( column, row ) = value;});

    return (from = std::move(result));
}
template<typename D>
Matrix<D> &stl_matrix::inverse(Matrix<D> &from) {

    const D det = determinant(from);

    Matrix<D> result = adjugate(from);

    result *= 1 / det;

    return result;
}
template<typename D>
Matrix<D> &stl_matrix::move_row(Matrix<D> &to, size_t toRow, Matrix<D> &from, size_t fromRow) {

    from.iterateInRow([&to, &toRow](Matrix<D> &, size_t , size_t column, D value)
    {
        to(toRow, column) = value;
    }, fromRow);

    return to;
}

template<typename D>
Matrix<D> &stl_matrix::eye(Matrix<D> &to, D toEye, int diagonal) {

    to.iterateInDiag([&toEye](Matrix<D> &, D &value){ value = toEye; }, diagonal);

    return to;
}

template<typename D>
Matrix<D> &stl_matrix::flip(Matrix<D> &from) {

    std::reverse(from.m_data.begin(), from.m_data.end());

    return from;
}

#endif //ZADANIE_8_STL_MATRIX_H
