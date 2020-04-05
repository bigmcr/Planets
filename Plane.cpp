#include "plane.h"
#include <QString>
#include <toString.h>

bool Plane::pointsAreNonLinear() const
{
    // if any of the points are the same, things don't work well
    if ((pt1 == pt2) || (pt1 == pt3) || (pt2 == pt3))
        return false;
    Point3D vect1 = (pt1 - pt2);
    Point3D vect2 = (pt2 - pt3);
    Point3D crossProduct = cross(vect1, vect2).round(3);
    if (crossProduct.isNull()) return false;
    return true;
}

Point3D projection(Point3D a, Point3D b)
{
    return dot(b, normalize(a))*normalize(a);
}

void Plane::updateBases()
{
    // get the vectors between the points
    Point3D vect1 = normalize(pt1 - pt2);
    Point3D vect2 = normalize(pt2 - pt3);

    if (vect1.x() < 0) vect1 *= -1;
    else if (vect1.y() < 0) vect1 *= -1;
    else if (vect1.z() < 0) vect1 *= -1;

    if (vect2.x() < 0) vect2 *= -1;
    else if (vect2.y() < 0) vect2 *= -1;
    else if (vect2.z() < 0) vect2 *= -1;

    Point3D perpVect;
    // make perpVector parallel to the axis that is closest to the vector
    // this takes care of special cases like Point3D(0,1,0) and Point3D(0,0,1)
    if ((std::abs(vect1.x()) > std::abs(vect1.y())) && (std::abs(vect1.x()) > std::abs(vect1.z())))
        perpVect = Point3D(1, 0, 0);
    else if ((std::abs(vect1.y()) > std::abs(vect1.x())) && (std::abs(vect1.y()) > std::abs(vect1.z())))
        perpVect = Point3D(0, 1, 0);
    else
        perpVect = Point3D(0, 0, 1);

    basis1 = perpVect*basisLength;
    basis2 = normalize(vect2 - projection(basis1, vect2), basisLength);
//    basis1 = basis1.round(2);
//    basis2 = basis2.round(2);
    return;
}

Plane::Plane(Point3D p1, Point3D p2, Point3D p3, Point3D pointOnPlane)
{
    pt1 = p1;
    pt2 = p2;
    pt3 = p3;
    setAddPoint(pointOnPlane);
    updateBases();
}

void Plane::setAddPoint(Point3D newAddPoint)
{
    addPoint = newAddPoint;
}

void Plane::setPoints(Point3D p1, Point3D p2, Point3D p3)
{
    const Point3D oldPoints[3] = {pt1,pt2,pt3};
    pt1 = p1;
    pt2 = p2;
    pt3 = p3;
    if (!pointsAreNonLinear())
    {
        pt1 = oldPoints[0];
        pt2 = oldPoints[1];
        pt3 = oldPoints[2];
    }
    updateBases();
}

void Plane::setPoint(Point3D newPoint, int replacePoint)
{
    Point3D oldPoint;
    if (replacePoint == 0)
    {
        oldPoint = pt1;
        pt1 = newPoint;
        if (!pointsAreNonLinear())
            pt1 = oldPoint;
    }
    else if (replacePoint == 1)
    {
        oldPoint = pt2;
        pt2 = newPoint;
        if (!pointsAreNonLinear())
            pt2 = oldPoint;
    }
    else if (replacePoint == 2)
    {
        oldPoint = pt3;
        pt3 = newPoint;
        if (!pointsAreNonLinear())
            pt3 = oldPoint;
    }
    updateBases();
}

void Plane::getPoints(Point3D & p1, Point3D & p2, Point3D & p3) const
{
    p1 = pt1;
    p2 = pt2;
    p3 = pt3;
}

Point3D Plane::getPoint(int pointNumber) const
{
    if (pointNumber == 0)
        return pt1;
    else if (pointNumber == 1)
        return pt2;
    else if (pointNumber == 2)
        return pt3;
    return Point3D();
}

// (coord1, coord2) returns basis1*coord1 + basis2*coord2
Point3D Plane::getPointOnPlane(long double coord1, long double coord2) const
{
//    Point3D normal = cross(pt2 - pt1, pt3 - pt1)/cross(pt2 - pt1, pt3 - pt1).length();
//    Point3D averagePoint = projection(pt1, normal);
    return addPoint + (pt1 + pt2 + pt3)/3.0L + basis1*coord1 + basis2*coord2;
}

QString Plane::standardEquation() const
{
    Point3D normal = cross(basis1, basis2);
    return toStringL(normal.x() + addPoint.x()) + "x + " +
            toStringL(normal.y() + addPoint.y()) + "y + " +
            toStringL(normal.z() + addPoint.z()) + "z = 0";
}
