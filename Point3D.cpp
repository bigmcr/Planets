#include "Point3D.h"
#include <QString>
#include <toString.h>

const Point3D operator*(const Point3D & p, long double factor)
{
    return Point3D(p.x()*factor, p.y()*factor, p.z()*factor);
}

const Point3D operator*(long double factor, const Point3D & p)
{
    return Point3D(p.x()*factor, p.y()*factor, p.z()*factor);
}

const Point3D operator+(const Point3D & p1, const Point3D & p2)
{
    return Point3D(p1.x()+p2.x(), p1.y()+p2.y(), p1.z()+p2.z());
}

const Point3D operator-(const Point3D & p1, const Point3D & p2)
{
    return Point3D(p1.x()-p2.x(), p1.y()-p2.y(), p1.z()-p2.z());
}

const Point3D operator/(const Point3D & p, long double divisor)
{
    return Point3D(p.x()/divisor, p.y()/divisor, p.z()/divisor);
}

const Point3D operator-(Point3D & p)
{
    return Point3D(-p.x(), -p.y(), -p.z());
}

const Point3D operator%(const Point3D b, const Point3D c)
{
    return Point3D(b.y()*c.z() - b.z()*c.y(),
                   b.z()*c.x() - b.x()*c.z(),
                   b.x()*c.y() - b.y()*c.x());
}

const Point3D cross(const Point3D p1, const Point3D p2)
{
    return Point3D(p1.y()*p2.z() - p1.z()*p2.y(),
                   p1.z()*p2.x() - p1.x()*p2.z(),
                   p1.x()*p2.y() - p1.y()*p2.x());
}

long double dot(const Point3D p1, const Point3D p2)
{
    return p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z();
}

QString Point3D::toString(bool isHorizontal) const
{
    QString middle;
    if (isHorizontal) middle = ", ";
    else middle = "\n";
    return toStringL(xCoord) + middle + toStringL(yCoord) + middle + toStringL(zCoord);
}

Point3D normalize(Point3D point, long double length)
{
    return point/point.length() * length;
}

Point3D Point3D::round(int digits) const
{
    if (digits > 10 || digits < 0) return *this;
    long double multiple = 1;
    for (int i = 0; i < digits; i++) multiple *= 10;
    return Point3D(int(x()*multiple)/multiple,
                   int(y()*multiple)/multiple,
                   int(z()*multiple)/multiple);
}

long double distance(Point3D point)
{
    return point.length();
}
