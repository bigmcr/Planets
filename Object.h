#ifndef OBJECT_H
#define OBJECT_H
#include <QString>
#include <QColor>
#include <QList>
#include "Point3D.h"
#include "cmath"
#include <QDebug>

class Object
{
public:
    Object(Point3D newPosition = Point3D(),
           Point3D newVel = Point3D(), Point3D::dataType newMass = 0.0L,
           Point3D::dataType newRadius = 0.0L, QString newName = "",
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
        while (x_i.length() < 5) x_i.append(Point3D(0,0,0));
        while (v_i.length() < 5) v_i.append(Point3D(0,0,0));
        while (a_i.length() < 5) a_i.append(Point3D(0,0,0));
    }
    Point3D setPosition(Point3D newPos) {oldPos = position; return position = newPos;}
    Point3D setVelocity(Point3D newVel) {oldVel = velocity; return velocity = newVel;}
    Point3D setAccel(Point3D newAccel) {oldAccel = accel; return accel = newAccel;}
    Point3D setJerk(Point3D newJerk) {oldJerk = jerk; return jerk = newJerk;}
    Point3D::dataType setMass(Point3D::dataType newMass) {return mass = newMass;}
    Point3D::dataType setEnergy(Point3D::dataType newEnergy) {return energy = newEnergy;}
    Point3D::dataType setRadius(Point3D::dataType newRadius) {return radius = newRadius;}
    QString setName(QString newName) {return name = newName;}
    QColor setColor(QColor newColor) {return color = newColor;}
    void addToTrace(Point3D newPoint){trace.append(newPoint);}
    void eraseFirstTrace(){trace.erase(trace.begin());}

    Point3D getPosition() const {return position;}
    Point3D getVelocity() const {return velocity;}
    Point3D getAccel() const {return accel;}
    Point3D getJerk() const {return jerk;}
    Point3D::dataType getMass() const {return mass;}
    Point3D::dataType getEnergy() const {return energy;}
    Point3D::dataType getRadius() const {return radius;}
    QString getName() const {return name;}
    QColor getColor() const {return color;}
    Point3D::dataType x() const {return position.x();}
    Point3D::dataType y() const {return position.y();}
    Point3D::dataType dx() const {return velocity.x();}
    Point3D::dataType dy() const {return velocity.y();}
    QList<Point3D> getTrace() const {return trace;}
    void clearTrace() {trace.clear();}

    Point3D::dataType getSpeed() const {return velocity.length();}
    Point3D getMomentum() const {return velocity*mass;}
    Point3D::dataType distance(Point3D point) const {return sqrtl(point.x() * point.x() + point.y()*point.y());}
    void shiftTrace(Point3D center) {for (int i = 0; i < trace.size(); i++) trace[i] -= center;}
    Point3D setVelocity_plus_half(Point3D newV1_2) {return velocity_plus_half = newV1_2;}
    Point3D setAccel_N_Minus_one(Point3D newAcc_minus_one) {return accel_n_minus_one = newAcc_minus_one;}
    Point3D setPosition_plus_half(Point3D newPosition_plus_half) {return position_plus_half = newPosition_plus_half;}
    Point3D setAccel_N_plus_Half(Point3D newAccel_n_plus_half) {return accel_n_plus_half = newAccel_n_plus_half;}
    Point3D setX_i(int index, Point3D newX_i) {if (index < x_i.length()) x_i.replace(index, newX_i); return x_i.at(index);}
    Point3D setV_i(int index, Point3D newV_i) {if (index < v_i.length()) v_i.replace(index, newV_i); return v_i.at(index);}
    Point3D setA_i(int index, Point3D newA_i) {if (index < a_i.length()) a_i.replace(index, newA_i); return a_i.at(index);}
    Point3D setR_P(Point3D newR_P) {return r_p = newR_P;}
    Point3D setV_P(Point3D newV_P) {return v_p = newV_P;}

    Point3D::dataType getKineticEnergy() const {return mass * velocity.magSqr() / 2.0L;}
    Point3D getVelocity_plus_half() const {return velocity_plus_half;}
    Point3D getAccel_N_Minus_one() const {return accel_n_minus_one;}
    Point3D getPosition_plus_half() const {return position_plus_half;}
    Point3D getAccel_N_plus_Half() const {return accel_n_plus_half;}
    Point3D getX_i(int index) const {if (index < x_i.length()) return x_i.at(index); return Point3D(0, 0, 0);}
    Point3D getV_i(int index) const {if (index < v_i.length()) return v_i.at(index); return Point3D(0, 0, 0);}
    Point3D getA_i(int index) const {if (index < a_i.length()) return a_i.at(index); return Point3D(0, 0, 0);}
    Point3D getR_P() const {return r_p;}
    Point3D getV_P() const {return v_p;}
    Point3D getOldPos() const {return oldPos;}
    Point3D getOldVel() const {return oldVel;}
    Point3D getOldAccel() const {return oldAccel;}
    Point3D getOldJerk() const {return oldJerk;}

private:
    Point3D position;
    Point3D velocity;
    Point3D accel;
    Point3D jerk;
    Point3D::dataType energy;
    Point3D::dataType mass;
    Point3D::dataType radius;
    QString name;
    QColor color;
    QList<Point3D> trace;

    // specialty values for Velocity Verlet
    Point3D velocity_plus_half, accel_n_minus_one;

    // specialty values for Position Verlet
    Point3D position_plus_half, accel_n_plus_half;

    // specialty values for Yoshida
    QVector<Point3D> x_i, v_i, a_i;

    // specialty values for Hermite
    Point3D oldPos, oldVel, oldAccel, oldJerk, r_p, v_p;

};

#endif // OBJECT_H
