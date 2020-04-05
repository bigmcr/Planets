#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>
#include <QPoint>
#include <QPointF>
#include <Point3D.h>
#include <QString>

class Matrix
{
    QVector<QVector<double> > array;
    // for a 3x3 matrix, it should be set out like the following
    //    <--------N------->
    // ^  M1N1   M1N2   M1N3
    // |
    // M  M2N1   M2N2   M2N3
    // |
    // |  M3N1   M2N3   M3N3
    static const int Error = 12345;
public:
    Matrix(int m = 1, int n = 1, double initializeValue = 0);
    int getN() const {if (array.size() == 0) return 0; else return array.at(0).size();}
    int getM() const {return array.size();}  // the main array is the horizontal direction.
    double at(int i, int j) const {if (isInM(i) && isInN(j)) return array.at(i).at(j); else return 0;}
    double setValue(int i, int j, double value)
    {
        if (isInM(i) && isInN(j)) return array[i][j] = value;
        else return Error;
    }
    bool isInM(int i) const {return (i >=0 && i <getN());}
    bool isInN(int j) const {return (j >=0 && j <getM());}
    bool detIsLong() const {if ((!isSquare()) || getN() > 5) return true; else return false;}
    double det() const;
    bool isNull() const;
    bool isSquare() const {return getN() == getM();}
    Matrix subMatrix(int column, int row = 0);
    Matrix subMatrix(int startRow, int startCol, int endRow, int endCol);
    QString toString(bool formatNice = false) const;
    QVector<double> getRow(int row) const
    {
        if (isInM(row))
            return array.at(row);
        else
        {
            QVector<double> vector;
            return vector;
        }
    }
    QVector<double> getColumn(int col) const
    {
        if (!isInN(col))
        {
            QVector<double> vector;
            return vector;
        }
        else
        {
            QVector<double> column;
            for (int i = 0; i < getM(); i++)
                column.append(at(col, i));
            return column;
        }
    }
    Matrix transpose() const
    {
        Matrix newMat(getM(), getN());
        for (int i = 0; i < getM(); i++)
            for (int j = 0; j < getN(); j++)
                newMat.setValue(i,j,at(j,i));
        return newMat;
    }

    static int getError() {return Error;}
    static Matrix getIdentity(int size)
    {
        Matrix newMat(size, size, 0);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (i == j) newMat.setValue(i,j,1);
        return newMat;
    }
/*    Matrix operator=(Matrix mat1)
    {
        try
        {
            for (int i = 0; i < array.at(0).size(); i++)
                array[i].clear();
            array.clear();
            QVector<double> vector;
            for (int i = 0; i < mat1.getM(); i++)
            {
                array.append(vector);
                for (int j = 0; j < mat1.getN(); j++)
                    array[j].append(mat1.at(i,j));
            }
            return *this;
        }
        catch (...)
        {
            return Matrix(3,3,0);
        }
    }*/
};

bool operator==(const Matrix &m1, const Matrix &m2);
bool operator!=(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m1, double factor);
Matrix operator*(double factor, const Matrix &m1);
Matrix operator-(const Matrix &m1, const Matrix &m2);
Matrix operator+(const Matrix &m1, const Matrix &m2);

#ifndef MATRIX_CPP
#define MATRIX_CPP
#include "matrix.cpp"
#endif // MATRIX_CPP


#endif // MATRIX_H
