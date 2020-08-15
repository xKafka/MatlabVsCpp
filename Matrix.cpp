#include "Matrix.h"
#include "stl_matrix.h"

#include <stdexcept>
#include <ostream>

#include <iostream>

template<typename D>
Matrix<D>::Matrix(size_t rows, size_t columns)
        : m_data(rows * columns)
        , m_rows(rows)
        , m_columns(columns)
{
}
template<typename D>
Matrix<D>::Matrix(size_t rows, size_t columns, const D *data)
        : m_data(data, data + rows * columns)
        , m_rows(rows)
        , m_columns(columns)
{}
template<typename D>
Matrix<D>::Matrix(const Matrix<D> &other)
        : m_data(other.m_data)
        , m_rows(other.m_rows)
        , m_columns(other.m_columns)
{
     std::cout << "Matrix copied" << '\n';
}
template<typename D>
Matrix<D>::Matrix(Matrix<D> &&other) noexcept
{
    *this = std::move(other);
}
template<typename D>
Matrix<D> &Matrix<D>::operator=(const Matrix<D> &other)
{
    m_data = other.m_data;
    m_rows = other.m_rows;
    m_columns = other.m_columns;

    return *this;
}
template<typename D>
Matrix<D> &Matrix<D>::operator=(Matrix<D> &&other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_rows, other.m_rows);
    std::swap(m_columns, other.m_columns);

    return *this;
}
template<typename D> size_t Matrix<D>::size() const { return m_data.size(); }
template<typename D> size_t Matrix<D>::rows() const { return m_rows; }
template<typename D> size_t Matrix<D>::columns() const { return m_columns; }

template<typename D> D &Matrix<D>::operator() (size_t row, size_t column) { return m_data[row * columns() + column]; }
template<typename D> const D &Matrix<D>::operator() (size_t row, size_t column) const { return m_data[row * columns() + column]; }
template<typename D>
Matrix<D> Matrix<D>::operator-() const
{
    Matrix result = *this;

    stl_matrix::negate(result);
    return result;
}
template<typename D>
Matrix<D> &Matrix<D>::operator+=(const Matrix<D> &other)
{
    if(!hasSameSize(other))
        throw new std::runtime_error("Matrix::operator+=: Matrix size mismatch");

    const D *otherValue = other.m_data.data();

    iterate([&otherValue](Matrix &, size_t, size_t, D &value) {
        value += *otherValue++;
    });

    return *this;
}
template<typename D>
Matrix<D> &Matrix<D>::operator-=(const Matrix<D> &other)
{
    if(!hasSameSize(other))
        throw new std::runtime_error("Matrix::operator+=: Matrix size mismatch");

    const D *otherValue = other.m_data.data();

    iterate([&otherValue](Matrix &, size_t, size_t, D &value) {
        value -= *otherValue++;
    });

    return *this;
}
template<typename D>
Matrix<D> &Matrix<D>::operator+=(const D &otherValue)
{
    iterate([&otherValue](Matrix &, size_t, size_t, D &value) {
        value += otherValue;
    });

    return *this;
}
template<typename D>
Matrix<D> &Matrix<D>::operator-=(const D &otherValue)
{
    iterate([&otherValue](Matrix &, size_t, size_t, D &value) {
        value -= otherValue;
    });

    return *this;
}
template<typename D>
Matrix<D> &Matrix<D>::operator*=(const Matrix &other)
{
    if(!canBeMultiplied(other))
        throw new std::runtime_error("Matrix::operator*=: Matrix size mismatch");

    const Matrix<D> &left = *this, &right = other;

    Matrix<D> result(left.rows(), right.columns());

    result.iterate([&left, &right](Matrix &, size_t row, size_t column, D &value) {
        D result = {};
        for(size_t index = 0; index < left.columns(); ++index)
            result += left(row, index) * right(index, column);
        value = result;
    });

    return (*this = std::move(result));
}
template<typename D>
Matrix<D> &Matrix<D>::operator*=(const D &otherValue)
{
    iterate([&otherValue](Matrix &, size_t, size_t, D &value){
        value *= otherValue;
    });

    return *this;
}
template<typename D>
bool Matrix<D>::operator==(const Matrix &other) {

    if(m_rows not_eq other.rows() && m_columns not_eq other.columns())
        return false;

    for(size_t row = 0; row < m_rows; ++row)
    {
        for(size_t column = 0; column < m_columns; ++column)
        {
            if(this->operator()(row, column) not_eq other(row, column))
                return false;
        }
    }

    return true;
}
template<typename D>
bool Matrix<D>::operator!=(const Matrix &other) {
    return !(*this == other);
}
template<typename D>
void Matrix<D>::push_back_row(Matrix<D> &from) {
/*
    if(m_columns not_eq from.columns())
        throw new std::runtime_error("Matrix::push_back_row column size mismatch");
*/
    if(!(m_columns))
        m_columns = from.columns();

    ++m_rows;

    for(size_t column = 0; column < from.columns(); ++column) {
        m_data.push_back(from(0, column));
    }
}

template<typename D>
void Matrix<D>::push_back_column(Matrix<D> &from) {

    if(m_rows not_eq from.columns())
        throw new std::runtime_error("Matrix::push_back_column row size mismatch");

    ++m_columns;

    for(size_t column = 0, itr = m_columns - 1; column < from.columns(); ++column, itr += m_columns)
        m_data.insert(m_data.begin() + itr, from(0,column));
}

template<typename D> Matrix<D> operator*(const Matrix<D> &left, const Matrix<D> &right)
{
    if(!left.canBeMultiplied(right))
        throw new std::runtime_error("Matrix::operator*=: Matrix size mismatch");

    Matrix<D> result(left.rows(), right.columns());

    result.iterate([&left, &right](Matrix<D> &, size_t row, size_t column, D &value) {
        D result = {};
        for(size_t index = 0; index < left.columns(); ++index)
            result += left(row, index) * right(index, column);
        value = result;
    });

    return result;
}

template Matrix<double> operator*(const Matrix<double> &, const Matrix<double> &);

template<typename D> std::ostream &operator<<(std::ostream &out, const Matrix<D> &matrix)
{
    out << "Matrix[" << matrix.rows() << "][" << matrix.columns() << "] {\n";

    matrix.iterate([&out](const Matrix<D> &matrix, size_t, size_t column, const D &value) mutable {
        out << value;
        if(column < matrix.columns() - 1)
            out << ", ";
    }, [&out](const Matrix<D> &, size_t) mutable {
        out << "    {";
    }, [&out](const Matrix<D> &matrix, size_t row) mutable {
        out << (row < matrix.rows() - 1 ?
                "},\n" :
                "}\n"
        );
    });

    return out << "}";
}

bool operator==(const Matrix<double> &left, const Matrix<double> &right) {

    if(left.rows() not_eq right.rows() && left.columns() not_eq right.columns())
        return false;

    for(size_t row = 0; row < left.rows(); ++row)
    {
        for(size_t column = 0; column < left.columns(); ++column)
        {
            if(left(row, column) not_eq right(row, column))
                return false;
        }
    }
    return true;
}
bool operator!=(const Matrix<double> &left, const Matrix<double> &right) {
    return !(left == right);
}

template std::ostream &operator<<(std::ostream &out, const Matrix<double> &matrix);
template class Matrix<double>;

