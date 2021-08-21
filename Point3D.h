#ifndef Point3D_H
#define Point3D_H

#include <QPoint>
#include <cmath>

class Point3D
{
public:
    typedef long double dataType ;
private:
    dataType xCoord, yCoord, zCoord;
public:
    Point3D(dataType x = 0, dataType y = 0, dataType z = 0) {xCoord = x; yCoord = y; zCoord = z;}
    bool isNull() const {return ((xCoord == 0) && (yCoord == 0) && (zCoord == 0));}
    dataType manhattanLength() const {return fabsl(xCoord) + fabsl(yCoord) + fabsl(zCoord);}
    dataType length() const {return sqrtl(magSqr()); }
    dataType mag() const {return length();}
    dataType magSqr() const
    {// if one of the coordinates is 0, don't calculate that distance
        if (zCoord == 0) return fabsl(xCoord*xCoord + yCoord*yCoord);
        if (yCoord == 0) return fabsl(xCoord*xCoord + zCoord*zCoord);
        if (xCoord == 0) return fabsl(yCoord*yCoord + zCoord*zCoord);
        return  fabsl(xCoord*xCoord + yCoord*yCoord + zCoord*zCoord);
    }
    void setX(dataType x) {xCoord = x;}
    void setY(dataType y) {yCoord = y;}
    void setZ(dataType z) {zCoord = z;}
    void setXY(dataType x, dataType y) {xCoord = x; yCoord = y;}
    void setXZ(dataType x, dataType z) {xCoord = x; zCoord = z;}
    void setYZ(dataType y, dataType z) {yCoord = y; zCoord = z;}
    void setXYZ(dataType x, dataType y, dataType z) {xCoord = x; yCoord = y; zCoord = z;}
    void setVar(int var, dataType value) {if (var == 0) xCoord = value; else if (var == 1) yCoord = value; else zCoord = value;}
    QPoint toPoint() const {return QPoint(xCoord, yCoord);}
    QPointF toPointF() const {return QPointF(xCoord, yCoord);}
    dataType x() const {return xCoord;}
    dataType y() const {return yCoord;}
    dataType z() const {return zCoord;}
    dataType var(int index) const
    {
        if (index == 0) return xCoord;
        if (index == 1) return yCoord;
        if (index == 2) return zCoord;
        return -0x123456;
    }
    QString toString(bool isHorizontal = true) const;
    Point3D round(int digits) const;

    Point3D operator+= (const Point3D & point)
    {
        xCoord += point.x();
        yCoord += point.y();
        zCoord += point.z();
       return *this ;
    }
    Point3D operator-= (const Point3D & point)
    {
        xCoord -= point.x();
        yCoord -= point.y();
        zCoord -= point.z();
        return *this;
    }
    Point3D operator/= (const dataType divisor)
    {
        xCoord /= divisor;
        yCoord /= divisor;
        zCoord /= divisor;
        return *this;
    }
    Point3D operator*= (const dataType factor)
    {
        xCoord *= factor;
        yCoord *= factor;
        zCoord *= factor;
        return *this;
    }
    bool operator!=(const Point3D p2) const {return ((xCoord != p2.x()) || (yCoord != p2.y()) || (zCoord != p2.z()));}
    bool operator==(const Point3D p2) const {return ((xCoord == p2.x()) && (yCoord == p2.y()) && (zCoord == p2.z()));}
};

const Point3D operator*(const Point3D & p, Point3D::dataType factor);
const Point3D operator*(Point3D::dataType factor, const Point3D & p);
const Point3D operator+(const Point3D & p1, const Point3D & p2);
const Point3D operator-(const Point3D & p1, const Point3D & p2);
const Point3D operator/(const Point3D & p, Point3D::dataType divisor);
const Point3D operator-(Point3D & p);
const Point3D operator%(const Point3D p1, const Point3D p2);
const Point3D cross(const Point3D p1, const Point3D p2);
Point3D::dataType dot(const Point3D p1, const Point3D p2);
Point3D normalize(const Point3D point, Point3D::dataType length = 1);
Point3D::dataType distance(const Point3D point1, const Point3D point2 = Point3D(0,0,0));

#endif // Point3D_H
