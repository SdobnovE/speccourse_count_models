#pragma once
#include <vector>
#include <iostream>

class Matrix {
private:
    size_t row_number_;
    size_t column_number_;
    std::vector<std::vector<double>> matrix_data_;

public:
    void Resize(size_t row_num, size_t col_num) {
        row_number_ = row_num;
        column_number_ = col_num;
        matrix_data_.resize(row_number_);
        for (auto& row : matrix_data_) {
            row.resize(column_number_);
        }
    }

    void FillZeroes() {
        for (auto& row : matrix_data_) {
            for (auto& elem : row) {
                elem = 0.;
            }
        }
    }

    Matrix(size_t row_num, size_t col_num) {
        Resize(row_num, col_num);
        FillZeroes();
    }

    Matrix(size_t row_num) : Matrix(row_num, row_num) {
    }

    Matrix(std::vector<std::vector<double>> matrix) : row_number_(matrix.size()) {
        if (row_number_ > 0) {
            column_number_ = matrix[0].size();
        }
        Resize(row_number_, column_number_);
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[0].size(); ++j) {
                matrix_data_[i][j] = matrix[i][j];
            }
        }
    }

    size_t Rows() const {
        return row_number_;
    }

    size_t Columns() const {
        return column_number_;
    }

    double& operator()(size_t row, size_t column) {
        return matrix_data_[row][column];
    }

    Matrix& operator+=(const Matrix& left) {
        for (size_t row = 0; row < left.Rows(); ++row) {
            for (size_t column = 0; column < left.Columns(); ++column) {
                (*this)(row, column) += left(row, column);
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& left) {
        Matrix result(*this);
        result += left;
        return result;
    }

    Matrix operator*(const Matrix& left) {
        Matrix result(this->Rows(), left.Columns());

        for (size_t row = 0; row < this->Rows(); ++row) {
            for (size_t column = 0; column < left.Columns(); ++column) {
                for (size_t k = 0; k < left.Rows(); ++k) {
                    result(row, column) += (*this)(row, k) * left(k, column);
                }
            }
        }
        return result;
    }

    Matrix& operator=(const Matrix& left) {
        for (size_t row = 0; row < this->Rows(); ++row) {
            for (size_t column = 0; column < left.Columns(); ++column) {
                matrix_data_[row][column] = left.matrix_data_[row][column];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& left) {
        for (size_t row = 0; row < left.Rows(); ++row) {
            for (size_t column = 0; column < left.Columns(); ++column) {
                (*this)(row, column) -= left(row, column);
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix& left) {
        Matrix result(*this);
        result -= left;
        return result;
    }

    Matrix operator-(const Matrix& left) const {
        Matrix result(*this);
        result -= left;
        return result;
    }

    Matrix operator+(const Matrix& left) const {
        Matrix result(*this);
        result += left;
        return result;
    }

    Matrix operator*(const Matrix& left) const {
        Matrix result(this->Rows(), left.Columns());

        for (size_t row = 0; row < this->Rows(); ++row) {
            for (size_t column = 0; column < left.Columns(); ++column) {
                for (size_t k = 0; k < left.Rows(); ++k) {
                    result(row, column) += (*this)(row, k) * left(k, column);
                }
            }
        }
        return result;
    }

    void Print() const {
        for (size_t row = 0; row < this->Rows(); ++row) {
            for (size_t column = 0; column < this->Columns(); ++column) {
                std::cout << (*this)(row, column) << " ";
            }
            std::cout << "\n";
        }
    }
};

Matrix Transpose(const Matrix& matr) {
    Matrix result(matr.Columns(), matr.Rows());
    for (size_t i = 0; i < matr.Rows(); ++i) {
        for (size_t j = 0; j < matr.Columns(); ++j) {
            result(j, i) = matr(i, j);
        }
    }
    return result;
}

Matrix Unit_Matrix(size_t size_matr) {
    Matrix result(size_matr);
    for (size_t i = 0; i < size_matr; ++i) {
        result(i, i) = 1.;
    }
    return result;
}
