#ifndef Matrix3x3_CPP
#define Matrix3x3_CPP

#include "Matrix3x3.h"
#include <toString.h>

Matrix3x3::Matrix3x3(long double initializeValue)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
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
            if (std::abs(m1.at(i,j) - m2.at(i,j)) > zeroL) return false;
    return true;
}

bool operator!=(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
    return !(m1==m2);
}

Matrix3x3 operator*(const Matrix3x3 &m1, const Matrix3x3 &m2)
{
/*    if (m1.isSquare() && m1.getN() == 3)
    {
        QVector<double> r1, r2, r3;
        Matrix3x3 newMat;
//        for (int i = 0; i < 3; i++)
//            for (int j = 0; j < 3; j++)
//                newMat.setValue(i,j, m1.getColumn(i)*m2.getRow(j));
        //        a11xb11 + a12xb21 + a13xb31
        r1.append(m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + m1(0,2)*m2(2,0));
        //        a11xb12 + a12xb22 + a13xb32
        r1.append(m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) + m1(0,2)*m2(2,1));
        //        a11xb13 + a12xb23 + a13xb33
        r1.append(m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) + m1(0,2)*m2(2,2));

        //        a21xb11 + a22xb21 + a23xb31
        r2.append(m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) + m1(1,2)*m2(2,0));
        //        a21xb12 + a22xb22 + a23xb32
        r2.append(m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + m1(1,2)*m2(2,1));
        //        a21xb13 + a22xb23 + a23xb33
        r2.append(m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + m1(1,2)*m2(2,2));

        //        a31xb11 + a32xb21 + a33xb31
        r3.append(m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0));
        //        a31xb12 + a32xb22 + a33xb32
        r3.append(m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1));
        //        a31xb13 + a32xb23 + a33xb33
        r3.append(m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2));
        for (int i = 0; i < 3; i++) newMat.setValue(0, i, r1.at(i));
        for (int i = 0; i < 3; i++) newMat.setValue(1, i, r2.at(i));
        for (int i = 0; i < 3; i++) newMat.setValue(2, i, r3.at(i));
    }
    else*/
    {
        Matrix3x3 newM1 = m1.transpose();
        Matrix3x3 newMatrix3x3(0);
        for (int i = 0; i < newMatrix3x3.getM(); i++)
            for (int j = 0; j < newMatrix3x3.getN(); j++)
                newMatrix3x3.setValue(i, j, dot(newM1.getRow(j),m2.getColumn(i)));
        return newMatrix3x3;
    }
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
