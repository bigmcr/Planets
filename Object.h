#ifndef OBJECT_H
#define OBJECT_H
#include <QString>
#include <QColor>
#include <QList>
#include "Point3D.h"
#include "cmath"

class Object
{
public:
    Object(Point3D newPosition = Point3D(),
           Point3D newVel = Point3D(), long double newMass = 0.0L,
           long double newRadius = 0.0L, QString newName = "",
           QColor newColor = QColor())
    {
        setVelocity(newVel);
        setPosition(newPosition);
        setMass(newMass);
        setRadius(newRadius);
        setName(newName);
        setColor(newColor);
        setAccel(Point3D(0, 0, 0));
        setAccel(Point3D(0, 0, 0));
    }
    Point3D setPosition(Point3D newPos) {return position = newPos;}
    Point3D setVelocity(Point3D newVel) {return velocity = newVel;}
    Point3D setAccel(Point3D newAccel) {oldAccel = accel; return accel = newAccel;}
    long double setMass(long double newMass) {return mass = newMass;}
    long double setRadius(long double newRadius) {return radius = newRadius;}
    QString setName(QString newName) {return name = newName;}
    QColor setColor(QColor newColor) {return color = newColor;}
    void addToTrace(Point3D newPoint){trace.append(newPoint);}
    void eraseFirstTrace(){trace.erase(trace.begin());}

    Point3D getPosition() const {return position;}
    Point3D getVelocity() const {return velocity;}
    Point3D getAccel() const {return accel;}
    long double getMass() const {return mass;}
    long double getRadius() const {return radius;}
    QString getName() const {return name;}
    QColor getColor() const {return color;}
    long double x() const {return position.x();}
    long double y() const {return position.y();}
    long double dx() const {return velocity.x();}
    long double dy() const {return velocity.y();}
    QList<Point3D> getTrace() const {return trace;}
    void clearTrace() {trace.clear();}

    long double getSpeed() const {return sqrtl(velocity.x()*velocity.x() + velocity.y()*velocity.y());}
    Point3D getMomentum() const {return velocity*mass;}
    long double distance(Point3D point) const {return sqrtl(point.x() * point.x() + point.y()*point.y());}
    void shiftTrace(Point3D center) {for (int i = 0; i < trace.size(); i++) trace[i] -= center;}
private:
    Point3D position;
    Point3D velocity;
    Point3D accel;
    Point3D oldAccel;
    long double mass;
    long double radius;
    QString name;
    QColor color;
    QList<Point3D> trace;
};

#endif // OBJECT_H
