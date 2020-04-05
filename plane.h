#ifndef PLANE_H
#define PLANE_H
#include <Point3D.h>

class Plane
{
    Point3D pt1, pt2, pt3;
    Point3D basis1, basis2;
    Point3D addPoint;
    long double basisLength;
    bool pointsAreNonLinear() const;
    void updateBases();
public:
    Plane(Point3D p1 = Point3D(0,0,0), Point3D p2 = Point3D(1,0,0), Point3D p3 = Point3D(0,1,0),
          Point3D pointOnPlane = Point3D(0,0,0));

    void setAddPoint(Point3D newAddPoint);
    void setPoints(Point3D p1, Point3D p2, Point3D p3);
    void setPoint(Point3D newPoint, int replacePoint);
    void setBasisLength(long double newLength) {basisLength = newLength; updateBases();}

    void getPoints(Point3D & p1, Point3D & p2, Point3D & p3) const;
    Point3D getAddPoint() const {return addPoint;}
    Point3D getPoint(int pointNumber) const;
    Point3D getBasis1() const {return basis1;}
    Point3D getBasis2() const {return basis2;}
    Point3D getNormal() const {return cross(basis1, basis2);}
    Point3D operator() (long double x, long double y) const {return getPointOnPlane(x,y);}

    Point3D at(long double x, long double y) const {return getPointOnPlane(x,y);}
    Point3D at(QPointF point) const {return at(point.x(), point.y());}
    Point3D at(QPoint point) const {return at(point.x(), point.y());}

    // (coord1, coord2) returns basis1*coord1 + basis2*coord2
    Point3D getPointOnPlane(long double coord1, long double coord2) const;
    QString standardEquation() const;
};

#endif // PLANE_H
