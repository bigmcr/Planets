#ifndef Matrix3x3_H
#define Matrix3x3_H

#include <QVector>
#include <QPoint>
#include <QPointF>
#include <Point3D.h>
#include <QString>
#include <Point3D.h>

class Matrix3x3
{
    long double array[3][3];
    static const int Error = 12345;
    static const int size = 3;
public:
    Matrix3x3(long double initializeValue = 0);
    int getN() const {return size;}
    int getM() const {return size;}
    long double at(int i, int j) const {if (isInM(i) && isInN(j)) return array[i][j]; else return 0;}
    long double operator()(int i, int j) const {return at(i,j);}
    long double setValue(int i, int j, long double value)
    {
        if (isInM(i) && isInN(j))
            return array[i][j] = value;
        else return Error;
    }
    bool isInM(int i) const {return (i >=0 && i <getN());}
    bool isInN(int j) const {return (j >=0 && j <getM());}
    long double det() const;
    bool isNull() const;
    bool isSquare() const {return true;}
    QString toString(bool formatNice = false) const;
    Point3D getRow(int row) const
    {
        if (isInM(row))
        {
            Point3D Row;
            for (int i = 0; i < 3; i++)
                Row.setVar(i, at(row, i));
            return Row;
        }
        else
            return Point3D(0,0,0);
    }
    Point3D getColumn(int col) const
    {
        if (isInN(col))
        {
            Point3D Col;
            for (int i = 0; i < 3; i++)
                Col.setVar(i, at(col, i));
            return Col;
        }
        else
            return Point3D(0,0,0);
    }
    Matrix3x3 transpose() const
    {
        Matrix3x3 newMat;
        for (int i = 0; i < getM(); i++)
            for (int j = 0; j < getN(); j++)
                newMat.setValue(i,j,at(j,i));
        return newMat;
    }

    static int getError() {return Error;}
    static Matrix3x3 getIdentity()
    {
        Matrix3x3 newMat(0);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (i == j) newMat.setValue(i,j,1);
        return newMat;
    }
};

bool operator==(const Matrix3x3 &m1, const Matrix3x3 &m2);
bool operator!=(const Matrix3x3 &m1, const Matrix3x3 &m2);
Matrix3x3 operator*(const Matrix3x3 &m1, const Matrix3x3 &m2);
Matrix3x3 operator*(const Matrix3x3 &m1, double factor);
Matrix3x3 operator*(double factor, const Matrix3x3 &m1);
Matrix3x3 operator-(const Matrix3x3 &m1, const Matrix3x3 &m2);
Matrix3x3 operator+(const Matrix3x3 &m1, const Matrix3x3 &m2);

#endif // Matrix3x3_H
