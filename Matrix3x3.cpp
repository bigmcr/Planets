#ifndef Matrix3x3_CPP
#define Matrix3x3_CPP

#include "Matrix3x3.h"
#include <toString.h>

Matrix3x3::Matrix3x3(long double initializeValue)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            array[i][j] = initializeValue;
}

long double Matrix3x3::det() const
{
    return    at(0, 0)*(at(1, 1)*at(2, 2) - at(2, 1)*at(1, 2))
            - at(0, 1)*(at(0, 1)*at(2, 2) - at(2, 1)*at(0, 2))
            + at(0, 2)*(at(0, 1)*at(1, 2) - at(1, 1)*at(0, 3));
}

bool Matrix3x3::isNull() const
{
    for (int i = 0; i < getN(); i++)
        for (int j = 0; j < getM(); j++)
            if (at(i,j) != 0.0L) return false;
    return true;
}

bool operator==(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    if (m1.getN() != m2.getN()) return false;
    if (m1.getM() != m2.getM()) return false;
    for (int i = 0; i < m1.getM(); i++)
        for (int j = 0; j < m1.getN(); j++)
            if (m1.at(i,j) == m2.at(i,j)) return false;
    return true;
}

bool operator!=(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    return !(m1==m2);
}

Matrix3x3 operator*(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    Matrix3x3 newM1 = m1.transpose();
    Matrix3x3 newMatrix3x3(0);
    for (int i = 0; i < newMatrix3x3.getM(); i++)
        for (int j = 0; j < newMatrix3x3.getN(); j++)
            newMatrix3x3.setValue(i, j, dot(newM1.getRow(j),m2.getColumn(i)));
    return newMatrix3x3;
}

Matrix3x3 operator*(const Matrix3x3 &m1, long double factor)
{
    Matrix3x3 newMat;
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j)*factor);
    return newMat;
}

Matrix3x3 operator*(long double factor, const Matrix3x3 &m1)
{
    return m1*factor;
}

Matrix3x3 operator-(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    if (m1.getN() != m2.getN() || m1.getM() != m2.getM())
        return Matrix3x3(m1.getError());
    Matrix3x3 newMat;
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j) - m1.at(i, j));
    return newMat;
}

Matrix3x3 operator+(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    if (m1.getN() != m2.getN() || m1.getM() != m2.getM())
        return Matrix3x3(m1.getError());
    Matrix3x3 newMat;
    for (int i = 0; i < newMat.getN(); i++)
        for (int j = 0; j < newMat.getM(); j++)
            newMat.setValue(i, j, m1.at(i, j) + m1.at(i, j));
    return newMat;
}

QString Matrix3x3::toString(bool formatNice) const
{
    QString message;
    if (!formatNice)
    {
        for (int i = 0; i < getN(); i++)
        {
            for (int j = 0; j < getM(); j++)
                message += toStringL(at(i, j)) + " ";
            message += "\r\n";
        }
        return message;
    }

    QString digit;
    int chars = 0;
    for (int i = 0; i < getN(); i++)
        for (int j = 0; j < getM(); j++)
            if (chars < toStringL(at(i, j)).size())
                chars = toStringL(at(i, j)).size();
    chars += 2;

    for (int i = 0; i < getN(); i++)
    {
        for (int j = 0; j < getM(); j++)
        {
            digit = toStringL(at(i, j)) + " ";
            while (digit.size() < chars)
                digit.prepend("_");
            message += digit;
        }
        message += "\r\n";
    }
    message += "Nice";
    return message;
}

#endif // Matrix3x3_CPP
