#ifndef PICTUREFRAME_H
#define PICTUREFRAME_H

#include <QFrame>
#include <QPainter>
#include <Object.h>
#include "tracePoints.h"
#include <QTimer>
#include <cmath>
#include <Options.h>
#include <Matrix3x3.h>
#include <plane.h>
#include <toString.h>

class pictureFrame : public QFrame
{
    Q_OBJECT
public:
    explicit pictureFrame(QWidget *parent = nullptr);
//    void paintEvent(QPainter * event, QList<tracePoints> * newTraces,
//                    QList<Object> * newObjects, long double systemTime, Object removedObjects);
    void paintEvent(QPaintEvent * event);

public slots:
    void setZoomPoint(QPointF newZoomPoint) {zoomPoint = newZoomPoint;}
    void setOptionsPointer(Options * newOptionsPtr) {options = newOptionsPtr;}
    void setViewerAngle(Point3D newAngle) {viewerAngle = newAngle;}
    void setViewerPosition(Point3D newPosition) {viewerPosition = newPosition; updateMatrix();}
    void setDistanceScalar(double newScalar) {distanceScalar = newScalar;}
    void setDrawTrace(bool newDrawTrace) {drawTrace = newDrawTrace;}
    void setCenterOfMass(Point3D newCenterOfMass) {centerOfMass = newCenterOfMass;}

    void setGravLengths(QList <Point3D> newGravLengths) {gravLengths = newGravLengths;}
    void setGravPoints(QList <Point3D> newGravPoints) {gravPoints = newGravPoints;}
    void setGravColors(QList <QColor> newGravColors) {gravColors = newGravColors;}

    void setTraces(QList<tracePoints> * newTraces) {traces = newTraces;}
    void setObjects(QList<Object> * newObjects) {objects = newObjects;}
    void setTime(long double systemTime) {time = systemTime;}
    void setRemovedObjects(Object * newRemovedObjects) {removedObjects = newRemovedObjects;}

private:
    QList <Point3D> gravPoints;         // the list of points that gravity has been calculated at
    QList <Point3D> gravLengths;        // the list of lengths of the gravity vectors at the gravity points
    QList <QColor> gravColors;          // the list of colors corresponding to the gravPoints
    Options * options;                  // a pointer to the options class. Easier than passing all of the options individually
    double distanceScalar;
    bool drawTrace;
    Point3D centerOfMass;
    Point3D viewerAngle;
    Point3D viewerPosition;
    Matrix3x3 transformMatrix;
    QPointF zoomPoint;

    QList<tracePoints> * traces;
    QList<Object> * objects;
    long double time;
    Object * removedObjects;

    Point3D transform(Point3D point);

    void updateMatrix();
    QPointF to2D(Point3D point);
    void paintGravField(QPainter * painter, QList<Object> * objects);
    void drawEllipse(QPainter * painter, Point3D point, double width);
    void drawLine(QPainter * painter, Point3D point, Point3D fromPoint = Point3D(0, 0, 0));
    void drawText(QPainter * painter, Point3D point, QString text);
    long double distance(Point3D point) const {return sqrtl(point.x()*point.x() + point.y()*point.y() + point.z()*point.z());}
    QString toString(Point3D point)
    {
        if (options->usePolar())
        {
            double r = static_cast<double>(distance(point));
            return "r = " + toStringD(r) + ", " +
                    QChar(0x03C1) + " = " + toStringD(atan2(static_cast<double>(point.z()), static_cast<double>(r)) * 180.0/3.14159) + ", " +
                    QChar(952) + " = " + toStringD(atan2(static_cast<double>(point.y()), static_cast<double>(point.x())));
        }
        return "x = " + toStringL(point.x()) + "\ty = " + toStringL(point.y()) + "\tz = " + toStringL(point.z());
    }

    Point3D normalize(Point3D point, long double length = 1.0L);
    void paintPlanet(QPainter * painter);
    void paintTrace(QPainter * painter);
    void paintAxes(QPainter * painter);
};

#endif // PICTUREFRAME_H
