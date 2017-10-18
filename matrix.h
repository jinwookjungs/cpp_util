/**
 * @file    matrix.h
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2017-10-18 17:05:12
 *
 * Created on Wed 18 Oct 2017 05:05:12 PM KST.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <exception>
#include <memory>
#include <iostream>

/**
 * A simple matrix class.
 */
template <typename T>
class Matrix
{
public:
    Matrix (size_t num_rows, size_t num_cols, const T value=0);
    Matrix (const Matrix& m) = default;
    Matrix& operator= (const Matrix& rhs) = default;
    Matrix (Matrix&& m);
    Matrix& operator= (Matrix&& rhs);

    size_t get_num_rows () const;
    size_t get_num_cols () const;
    const T operator () (size_t row, size_t col) const;
    T& operator() (size_t row, size_t col);

    Matrix& operator+= (const T& rhs);
    Matrix& operator-= (const T& rhs);
    Matrix& operator*= (const T& rhs);

    Matrix& operator+= (const Matrix& rhs);
    Matrix& operator-= (const Matrix& rhs);
    Matrix& operator*= (const Matrix& rhs);

private:
    size_t num_rows_;           ///< Number of rows.
    size_t num_cols_;           ///< Number of columns.
    std::vector<T> values_;     ///< Element values.
};

template <typename T> bool operator== (const Matrix<T>& lhs, 
                                       const Matrix<T>& rhs);
template <typename T> bool operator!= (const Matrix<T>& lhs, 
                                       const Matrix<T>& rhs);

template <typename T> Matrix<T> operator+ (const T& lhs, const Matrix<T>& rhs);
template <typename T> Matrix<T> operator- (const T& lhs, const Matrix<T>& rhs);
template <typename T> Matrix<T> operator* (const T& lhs, const Matrix<T>& rhs);
template <typename T> Matrix<T> operator+ (const Matrix<T>& lhs, const T& rhs);
template <typename T> Matrix<T> operator- (const Matrix<T>& lhs, const T& rhs);
template <typename T> Matrix<T> operator* (const Matrix<T>& lhs, const T& rhs);

template <typename T> Matrix<T> operator+ (const Matrix<T>& lhs, 
                                           const Matrix<T>& rhs);
template <typename T> Matrix<T> operator- (const Matrix<T>& lhs, 
                                           const Matrix<T>& rhs);
template <typename T> Matrix<T> operator* (const Matrix<T>& lhs, 
                                           const Matrix<T>& rhs);

template <typename T> std::ostream& operator<< (std::ostream& os, 
                                                const Matrix<T>& matrix);

//-----------------------------------------------------------------------------
// Implementation of Matrix
//-----------------------------------------------------------------------------
template <typename T>
Matrix<T>::Matrix (size_t num_rows, size_t num_cols, const T value)
    : num_rows_(num_rows), num_cols_(num_cols), 
      values_(num_rows*num_cols, value)
{
    //
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& m)
    : num_rows_(m.num_rows_), num_cols_(m.num_cols_),
      values_(std::move(m.values_))
{
    // Leave m in a state in which it is safe to run the destructor.
    m.values_.clear();
    m.num_rows_ = 0;
    m.num_cols_ = 0;
}

template <typename T>
Matrix<T>& Matrix<T>::operator= (Matrix<T>&& rhs)
{
    if (this != &rhs) {
        num_rows_ = rhs.num_rows_;
        num_cols_ = rhs.num_cols_;
        values_ = std::move(rhs.values_);

        rhs.num_rows_ = rhs.num_cols_ = 0;
        rhs.values_.clear();
    }
    return *this;
}

template <typename T>
size_t Matrix<T>::get_num_rows () const
{
    return num_rows_;
}


template <typename T>
size_t Matrix<T>::get_num_cols () const
{
    return num_cols_;
}


template <typename T>
T& Matrix<T>::operator() (size_t row, size_t col)
{
    return values_[row*num_cols_ + col];
}


template <typename T>
const T Matrix<T>::operator() (size_t row, size_t col) const
{
    return values_[row*num_cols_ + col];
}


template <typename T>
Matrix<T>& Matrix<T>::operator+= (const T& rhs)
{
    for (auto& val : values_) {
        val += rhs;
    }
}


template <typename T>
Matrix<T>& Matrix<T>::operator-= (const T& rhs)
{
    for (auto& val : values_) {
        val -= rhs;
    }
}


template <typename T>
Matrix<T>& Matrix<T>::operator*= (const T& rhs)
{
    for (auto& val : values_) {
        val *= rhs;
    }
}


template <typename T>
Matrix<T>& Matrix<T>::operator+= (const Matrix<T>& rhs)
{
    if (num_rows_ != rhs.num_rows_ || num_cols_ != rhs.num_cols_) {
        throw std::logic_error ("different size");
    }

    for (size_t i = 0; i < num_rows_*num_cols_; i++) {
        values_[i] += rhs.values_[i];
    }

    return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::operator-= (const Matrix<T>& rhs)
{
    if (num_rows_ != rhs.num_rows_ || num_cols_ != rhs.num_cols_) {
        throw std::logic_error ("different size");
    }

    for (size_t i = 0; i < num_rows_*num_cols_; i++) {
        values_[i] -= rhs.values_[i];
    }

    return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::operator*= (const Matrix<T>& rhs)
{
    if (num_rows_ != rhs.num_cols_) {
        throw std::logic_error ("num_rows_ != rhs.num_cols");
    }

    Matrix<T> ret(num_rows_, rhs.num_cols_, 0);

    for (size_t i = 0; i < ret.num_rows_; i++) {
        for (size_t j = 0; j < ret.num_cols_; j++) {
            for (size_t k = 0; k < num_cols_; k++) {
                ret(i,j) += (*this)(i,k) * rhs(k,j);
            }
        }
    }

    std::swap(values_, ret.values_);
    num_rows_ = ret.num_rows_;
    num_cols_ = ret.num_cols_;

    return *this;
}


template <typename T> 
Matrix<T> operator+(const T& lhs, const Matrix<T>& rhs)
{
    Matrix<T> ret(rhs);
    ret += lhs;
    return ret;
}

template <typename T> 
Matrix<T> operator-(const T& lhs, const Matrix<T>& rhs)
{
    Matrix<T> ret(rhs);
    ret -= lhs;
    return ret;
}

template <typename T> 
Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs)
{
    Matrix<T> ret(rhs);
    ret *= lhs;
    return ret;
}

template <typename T> 
Matrix<T> operator+(const Matrix<T>& lhs, const T& rhs)
{
    Matrix<T> ret(lhs);
    ret += rhs;
    return ret;
}

template <typename T> 
Matrix<T> operator-(const Matrix<T>& lhs, const T& rhs)
{
    Matrix<T> ret(lhs);
    ret -= rhs;
    return ret;
}
template <typename T> 
Matrix<T> operator*(const Matrix<T>& lhs, const T& rhs)
{
    Matrix<T> ret(lhs);
    ret *= rhs;
    return ret;
}


template <typename T> 
Matrix<T> operator+ (const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.get_num_rows() != rhs.get_num_rows() 
        || lhs.get_num_cols() != rhs.get_num_cols()) {
        throw std::logic_error ("different size");
    }

    Matrix<T> ret(lhs);
    ret += rhs;
    return ret;
}


template <typename T> 
Matrix<T> operator- (const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.get_num_rows() != rhs.get_num_rows() 
        || lhs.get_num_cols() != rhs.get_num_cols()) {
        throw std::logic_error ("different size");
    }

    Matrix<T> ret(lhs);
    ret -= rhs;
    return ret;
}


template <typename T> 
Matrix<T> operator* (const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    auto lhs_num_rows = lhs.get_num_rows();
    auto lhs_num_cols = lhs.get_num_cols();
    auto rhs_num_cols = rhs.get_num_cols();

    if (lhs_num_rows != rhs_num_cols) {
        throw std::logic_error ("lhs.num_rows_ != rhs.num_cols");
    }

    Matrix<T> ret(lhs_num_rows, rhs_num_cols, 0);

    for (size_t i = 0; i < lhs_num_rows; i++) {
        for (size_t j = 0; j < rhs_num_cols; j++) {
            for (size_t k = 0; k < lhs_num_cols; k++) {
                ret(i,j) += lhs(i,k) * rhs(k,j);
            }
        }
    }

    return ret;
}


template <typename T>
std::ostream& operator<< (std::ostream& os, const Matrix<T>& matrix)
{
    for (size_t i = 0; i < matrix.get_num_rows(); i++) {
        for (size_t j = 0; j < matrix.get_num_cols(); j++) {
            os << matrix(i,j) << " ";
        }
        os << "\n";
    }
    return os;
}


#endif

