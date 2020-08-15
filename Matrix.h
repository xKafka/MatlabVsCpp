#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>
#include <iostream>

template<typename D>
class Matrix {
public:
    using DataType = D;

private:
    std::vector<DataType> m_data = {};
    size_t m_rows = 0, m_columns = 0;

    friend class stl_matrix;

public:
    Matrix() = default;
    Matrix(size_t rows, size_t columns);
    Matrix(size_t rows, size_t columns, const D *data);

    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;
    Matrix &operator=(const Matrix &other);
    Matrix &operator=(Matrix &&other) noexcept;

    template<size_t Rows, size_t Columns>
    explicit Matrix(const DataType (&from)[Rows][Columns]) :
            Matrix(Rows, Columns, &from[0][0])
    {}

    D *begin() {return m_data.data();}
    D *end() {return m_data.data() + m_rows * m_columns;}

    template<typename DO> bool hasSameSize(const Matrix<DO> &other) const
    { return m_rows == other.m_rows && m_columns == other.m_columns; }
    template<typename DO> bool canBeMultiplied(const Matrix<DO> &other) const
    { return m_columns == other.m_rows; }

    size_t size() const;
    size_t rows() const;
    size_t columns() const;

    void push_back_row(Matrix<D> &from);
    void push_back_column(Matrix<D> &from);

    DataType &operator() (size_t row, size_t column);
    const DataType &operator() (size_t row, size_t column) const;

    template <typename F>
    void iterateInRow(F iterator, size_t inRow)
    {
        if(inRow > m_rows || inRow < 0)
            throw new std::runtime_error("Matrix::iterateInRow out of size");

        D *value = m_data.data();
        value += inRow * m_columns;
        for(size_t column = 0; column < m_columns; ++column)
            iterator(*this, inRow, column, *value++);
    }
    template <typename F>
    void iterateInColumn(F iterator, size_t inColumn)
    {
        if(inColumn > m_columns || inColumn < 0)
            throw new std::runtime_error("Matrix::iterateInColumn out of size");

        D *value = m_data.data();
        value += inColumn;
        for(size_t row = 0; row < m_rows; ++row)
            iterator(*this, row, inColumn, *(value + m_columns * row));
    }
    template<typename F>
    void iterateInDiag(F iterator, int inDiag)
    {
        D *value = m_data.data();
        int columns = abs(inDiag);

        for(size_t row = 0; row < m_rows - columns; ++row)
        {
            if(inDiag <= 0)
                iterator(*this, this->operator()(row + columns, row));
            else
                iterator(*this, this->operator()(row, row + columns));
        }
    }
    template<typename F>
    void iterate(F iterator)
    {
        D *value = m_data.data();
        for(size_t row = 0; row < m_rows; ++row) {
            for(size_t column = 0; column < m_columns; ++column)
                iterator(*this, row, column, *value++);
        }
    }
    template<typename F>
    void iterate(F iterator) const
    {
        const D *value = m_data.data();
        for(size_t row = 0; row < m_rows; ++row) {
            for(size_t column = 0; column < m_columns; ++column)
                iterator(*this, row, column, *value++);
        }
    }
    template<typename F, typename F2, typename F3>
    void iterate(F iterator, F2 onRowStart, F3 onRowEnd) const
    {
        const D *value = m_data.data();
        for(size_t row = 0; row < m_rows; ++row) {
            onRowStart(*this, row);
            for(size_t column = 0; column < m_columns; ++column)
                iterator(*this, row, column, *value++);
            onRowEnd(*this, row);
        }
    }
    template<typename F, typename F2, typename F3>
    void iterate(F iterator, F2 onRowStart, F3 onRowEnd)
    {
        D *value = m_data.data();
        for(size_t row = 0; row < m_rows; ++row) {
            onRowStart(*this, row);
            for(size_t column = 0; column < m_columns; ++column)
                iterator(*this, row, column, *value++);
            onRowEnd(*this, row);
        }
    }

    Matrix operator-() const;

    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator+=(const DataType &value);
    Matrix &operator-=(const DataType &value);
    bool operator==(const Matrix &other);
    bool operator!=(const Matrix &other);

    Matrix &operator*=(const Matrix &other);
    Matrix &operator*=(const DataType &other);
};
template<typename D> Matrix<D> operator+(Matrix<D> l, const Matrix<D> &r) { return l += r; }
template<typename D> Matrix<D> operator-(Matrix<D> l, const Matrix<D> &r) { return l -= r; }
template<typename D> Matrix<D> operator+(Matrix<D> l, const D &r) { return l += r; }
template<typename D> Matrix<D> operator-(Matrix<D> l, const D &r) { return l -= r; }
template<typename D> Matrix<D> operator+(const D &l, Matrix<D> r) { return r += l; }
template<typename D> Matrix<D> operator-(const D &l, Matrix<D> r) { return r.negate() += l; }
template<typename D> Matrix<D> operator*(Matrix<D> l, const D &r) { return l *= r; }
template<typename D> Matrix<D> operator*(const D &l, Matrix<D> r) { return r *= l; }

template<typename D> Matrix<D> operator*(const Matrix<D> &l, const Matrix<D> &r);
template<typename D> std::ostream &operator<<(std::ostream &out, const Matrix<D> &matrix);

extern template Matrix<double> operator*(const Matrix<double> &l, const Matrix<double> &r);
extern template std::ostream &operator<<(std::ostream &out, const Matrix<double> &matrix);

extern template class Matrix<double>;

template<typename D> Matrix<D> RowVector(size_t columns) { return Matrix<D>(1, columns); }
template<typename D, size_t C> Matrix<D> RowVector(const D (&data)[C]) { return Matrix<D>(1, C, data); }
template<typename D> Matrix<D> ColVector(size_t rows) { return Matrix<D>(rows, 1); }
template<typename D, size_t R> Matrix<D> ColVector(const D (&data)[R]) { return Matrix<D>(R, 1, data); }

#endif // MATRIX_H
