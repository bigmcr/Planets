#include <Point3D.h>
#include <QString>
#include <QVector>

const Point3D operator*(const Point3D & p, Point3D::dataType factor)
{
    return Point3D(p.x()*factor, p.y()*factor, p.z()*factor);
}

const Point3D operator*(Point3D::dataType factor, const Point3D & p)
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

const Point3D operator/(const Point3D & p, Point3D::dataType divisor)
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

Point3D::dataType dot(const Point3D p1, const Point3D p2)
{
    return p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z();
}

QString Point3D::toString(bool isHorizontal) const
{
    QString middle;
    if (isHorizontal) middle = ", ";
    else middle = "\n";
    return QString::number(double(xCoord)) + middle + QString::number(double(yCoord)) + middle + QString::number(double(zCoord));
}

Point3D normalize(Point3D point, Point3D::dataType length)
{
    return point/Point3D::dataType(point.length()) * length;
}

Point3D Point3D::round(int digits) const
{
    if (digits > 10 || digits < 0) return *this;
    dataType multiple = 1;
    for (int i = 0; i < digits; i++) multiple *= 10;
    return Point3D(int(x()*multiple)/multiple,
                   int(y()*multiple)/multiple,
                   int(z()*multiple)/multiple);
}

Point3D::dataType distance(const Point3D point1, const Point3D point2)
{
    return (point1 - point2).length();
}

Point3D::dataType distance(const Point3D point1)
{
    return point1.length();
}

QVector<Point3D> add(QVector<Point3D> vect1, QVector<Point3D> vect2) {
    if (vect1.length() != vect2.length()) throw std::length_error("QVectors must have the same length to be added");
    QVector<Point3D> returnMe;
    for (int i = 0; i < vect1.length(); i++) {
        returnMe.append(vect1[i] + vect2[i]);
    }
    return returnMe;
}

QVector<Point3D> sub(QVector<Point3D> vect1, QVector<Point3D> vect2) {
    if (vect1.length() != vect2.length()) throw std::length_error("QVectors must have the same length to be subtracted");
    QVector<Point3D> returnMe;
    for (int i = 0; i < vect1.length(); i++) {
        returnMe.append(vect1[i] - vect2[i]);
    }
    return returnMe;
}

QVector<Point3D> operator*(QVector<Point3D> vect, Point3D::dataType factor) {
    QVector<Point3D> returnMe;
    for (int i = 0; i < vect.length(); i++) {
        returnMe.append(vect[i] * factor);
    }
    return returnMe;
}

QVector<Point3D> operator*(Point3D::dataType factor, QVector<Point3D> vect) {
    return vect * factor;
}

QVector<Point3D> operator/(QVector<Point3D> vect, Point3D::dataType factor) {
    QVector<Point3D> returnMe;
    for (int i = 0; i < vect.length(); i++) {
        returnMe.append(vect[i] / factor);
    }
    return returnMe;
}
