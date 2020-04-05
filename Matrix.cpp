#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "matrix.h"

Matrix::Matrix(int m, int n, double initializeValue)
{
    for (int i = 0; i < m; i++)
    {
        array.append(*new QVector<double>);
        for (int j = 0; j < n; j++)
            array[i].append(initializeValue);
    }
}

double findSubDet(Matrix mat)
{
    if (mat.getN() == 2)
        return (mat.at(0,0)*mat.at(1,1) - mat.at(1,0)*mat.at(0,1));
    else
    {
        double determinant = 0;
        int sign = 1;
        for (int i = 0; i < mat.getN(); i++)
        {
            determinant += sign*findSubDet(mat.subMatrix(i));
            sign *= -1;
        }
        return determinant;
    }
}

double Matrix::det() const
{
    if (!isSquare())
        return 0;
    return findSubDet(*this);
}

Matrix Matrix::subMatrix(int column, int row)
{
    Matrix newMatrix(getM() - 1, getN() - 1);
    int currentRow = -1, currentColumn = -1;
    for (int i = 0; i < newMatrix.getM(); i++)
    {
        currentRow++;
        if (currentRow == row)
        {
            currentRow--;
            continue;
        }
        currentColumn = -1;
        for (int j = 0; j < newMatrix.getN(); j++)
        {
            currentColumn++;
            if (currentColumn == column)
            {
                currentColumn--;
                continue;
            }
            newMatrix.setValue(currentRow, currentColumn, at(i,j));
        }
    }
    return newMatrix;
}

Matrix Matrix::subMatrix(int startRow, int startCol, int endRow, int endCol)
{
    Matrix newMatrix(endCol - startCol, endRow - startRow);
    int currentRow = startRow - 1, currentColumn = startCol - 1;
    for (int i = startCol; i < endCol - startCol; i++)
    {
        currentRow++;
        currentColumn = -1;
        for (int j = startRow; j < endRow - startRow; j++)
        {
            currentColumn++;
            newMatrix.setValue(currentRow, currentColumn, at(i,j));
        }
    }
    return newMatrix;
}

bool Matrix::isNull() const
{
    for (int i = 0; i < getN(); i++)
        for (int j = 0; j < getM(); j++)
            if (at(i,j) != 0) return false;
    return true;
}

double operator*(QVector<double> v1, QVector<double> v2)
{
    if (v1.size() != v2.size())
        return 0;
    double sum = 0;
    for (int i = 0; i < v1.size(); i++)
        sum += v1.at(i)*v2.at(i);
    return sum;
}

bool operator==(const Matrix &m1, const Matrix &m2)
{
    if (m1.getN() != m2.getN()) return false;
    if (m1.getM() != m2.getM()) return false;
    int isNotEqual = 0;
    for (int i = 0; i < m1.getM(); i++)
        for (int j = 0; j < m1.getN(); j++)
            if (m1.at(i,j) != m2.at(i,j)) isNotEqual++;
    return isNotEqual == 0;
}

bool operator!=(const Matrix &m1, const Matrix &m2)
{
    if (m1.getN() != m2.getN()) return true;
    if (m1.getM() != m2.getM()) return true;
    int isNotEqual = 0;
    for (int i = 0; i < m1.getM(); i++)
        for (int j = 0; j < m1.getN(); j++)
            if (m1.at(i,j) != m2.at(i,j)) isNotEqual++;
    return isNotEqual != 0;
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    if (m1.getN() != m2.getM())
        return Matrix(1,1, Matrix::getError());
    Matrix newM1 = m1.transpose();
    Matrix newMatrix(newM1.getM(), m2.getN(), 0);
    for (int i = 0; i < newMatrix.getM(); i++)
        for (int j = 0; j < newMatrix.getN(); j++)
            newMatrix.setValue(i, j, newM1.getRow(j)*m2.getColumn(i));
    return newMatrix;
}

Matrix operator*(const Matrix &m1, double factor)
{
    Matrix newMat(m1.getN(), m1.getM());
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j)*factor);
    return newMat;
}

Matrix operator*(double factor, const Matrix &m1)
{
    return m1*factor;
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
    if (m1.getN() != m2.getN() || m1.getM() != m2.getM())
        return Matrix(1,1, m1.getError());
    Matrix newMat(m1.getN(), m1.getM());
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j) - m1.at(i, j));
    return newMat;
}

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
    if (m1.getN() != m2.getN() || m1.getM() != m2.getM())
        return Matrix(1,1, m1.getError());
    Matrix newMat(m1.getN(), m1.getM());
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j) + m1.at(i, j));
    return newMat;
}

QString Matrix ::toString(bool formatNice) const
{
    QString message;
    if (!formatNice)
    {
        for (int i = 0; i < getN(); i++)
        {
            for (int j = 0; j < getM(); j++)
                message += QString::number(at(i, j)) + " ";
            message += "\n";
        }
        return message;
    }

    QString digit;
    int chars = 0;
    for (int i = 0; i < getN(); i++)
        for (int j = 0; j < getM(); j++)
            if (chars < QString::number(at(i, j)).size())
                chars = QString::number(at(i, j)).size();
    chars += 2;

    for (int i = 0; i < getN(); i++)
    {
        for (int j = 0; j < getM(); j++)
        {
            digit = QString::number(at(i, j)) + " ";
            while (digit.size() < chars)
                digit.prepend(" ");
            message += digit;
        }
        message += "\n";
    }
    message += "Nice";
    return message;
}

#endif // MATRIX_CPP
