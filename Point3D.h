#ifndef Point3D_H
#define Point3D_H

#include <QPoint>
#include <cmath>
#include <precision.h>

class Point3D
{
    long double xCoord, yCoord, zCoord;
public:
    Point3D(long double x = 0, long double y = 0, long double z = 0) {xCoord = x; yCoord = y; zCoord = z;}
    bool isNull() const {return ((std::abs(xCoord) < zeroL) && (std::abs(yCoord) < zeroL) && (std::abs(zCoord) < zeroL));}
    long double manhattanLength() const {return std::abs(xCoord) + std::abs(yCoord) + std::abs(zCoord);}
    long double length() const
    {// if one of the coordinates is 0, don't calculate that distance
        if (std::abs(zCoord) < zeroL) return sqrtl(xCoord*xCoord + yCoord*yCoord);
        if (std::abs(yCoord) < zeroL) return sqrtl(xCoord*xCoord + zCoord*zCoord);
        if (std::abs(xCoord) < zeroL) return sqrtl(yCoord*yCoord + zCoord*zCoord);
        return sqrtl(xCoord*xCoord + yCoord*yCoord + zCoord*zCoord);
    }
    void setX(long double x) {xCoord = x;}
    void setY(long double y) {yCoord = y;}
    void setZ(long double z) {zCoord = z;}
    void setXY(long double x, long double y) {xCoord = x; yCoord = y;}
    void setXZ(long double x, long double z) {xCoord = x; zCoord = z;}
    void setYZ(long double y, long double z) {yCoord = y; zCoord = z;}
    void setXYZ(long double x, long double y, long double z) {xCoord = x; yCoord = y; zCoord = z;}
    void setVar(int var, long double value) {if (var == 0) xCoord = value; else if (var == 1) yCoord = value; else zCoord = value;}
    QPoint toPoint() const {return QPoint(static_cast<int>(xCoord), static_cast<int>(yCoord));}
    QPointF toPointF() const {return QPointF(static_cast<double>(xCoord), static_cast<double>(yCoord));}
    long double x() const {return xCoord;}
    long double y() const {return yCoord;}
    long double z() const {return zCoord;}
    long double var(int index) const
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
    Point3D operator/= (long double divisor)
    {
        xCoord /= divisor;
        yCoord /= divisor;
        zCoord /= divisor;
        return *this;
    }
    Point3D operator*= (const long double factor)
    {
        xCoord *= factor;
        yCoord *= factor;
        zCoord *= factor;
        return *this;
    }
    bool operator!=(const Point3D p2) const {return ((std::abs(xCoord - p2.x()) > zeroL) || (std::abs(yCoord - p2.y()) > zeroL) || (std::abs(zCoord - p2.z()) > zeroL));}
    bool operator==(const Point3D p2) const {return ((std::abs(xCoord - p2.x()) < zeroL) && (std::abs(yCoord - p2.y()) < zeroL) && (std::abs(zCoord - p2.z()) < zeroL));}
};

const Point3D operator*(const Point3D & p, long double factor);
const Point3D operator*(long double factor, const Point3D & p);
const Point3D operator+(const Point3D & p1, const Point3D & p2);
const Point3D operator-(const Point3D & p1, const Point3D & p2);
const Point3D operator/(const Point3D & p, long double divisor);
const Point3D operator-(Point3D & p);
const Point3D operator%(const Point3D p1, const Point3D p2);
const Point3D cross(const Point3D p1, const Point3D p2);
long double dot(const Point3D p1, const Point3D p2);
Point3D normalize(const Point3D point, long double length = 1);
long double distance(Point3D point);

#endif // Point3D_H
