#include "pictureFrame.h"
#include <algorithm>
#include <QTime>
#include <Options.h>
#include <Matrix3x3.h>
#include <plane.h>
#include <qmath.h>
#include <precision.h>
#include <tgmath.h>
#include <physicsconsts.h>

Point3D operator*(Matrix3x3 mat, Point3D v)
{
    return Point3D(v.x()*mat.at(0,0) + v.y()*mat.at(0,1) + v.z()*mat.at(0,2),
                   v.x()*mat.at(1,0) + v.y()*mat.at(1,1) + v.z()*mat.at(1,2),
                   v.x()*mat.at(2,0) + v.y()*mat.at(2,1) + v.z()*mat.at(2,2));
}

pictureFrame::pictureFrame(QWidget *parent) :
        QFrame(parent)
{
	distanceScalar = 1;
	drawTrace = true;
    centerOfMass = Point3D(0, 0, 0);
    viewerAngle = Point3D(0, 0, 0);
    viewerPosition = Point3D(0, 0, 0);
    updateMatrix();
}

Point3D pictureFrame::normalize(Point3D point, long double length) {return point*length/distance(point);}

int round(int numToRound, int multiple)
{
	int number = numToRound/multiple;
	return number*multiple;
}

void pictureFrame::paintTrace(QPainter * painter)
{
    if (drawTrace)
    {
        for (int i = 0; i < objects->size(); i++)
        {
            painter->setBrush(objects->at(i).getColor());
            painter->setPen(objects->at(i).getColor());
            for (int j = 0; j < objects->at(i).getTrace().size() - 1; j++)
                drawLine(painter, transform(objects->at(i).getTrace().at(j)),
                         transform(objects->at(i).getTrace().at(j + 1)));
            if (! objects->at(i).getTrace().isEmpty())
                drawLine(painter, transform(objects->at(i).getTrace().last()),
                         transform(objects->at(i).getPosition()));
        }
        for (int i = 0; i < traces->size(); i++)
        {
            painter->setPen(traces->at(i).getColor());
            painter->setBrush(traces->at(i).getColor());
            for (int j = 0; j < traces->at(i).size() - 1; j++)
                drawLine(painter, transform(traces->at(i).getPoint(j)),
                                  transform(traces->at(i).getPoint(j + 1)));
        }
    }
}

bool operator<(const Object obj1, const Object obj2)
{
    return obj1.getPosition().z() <= obj2.getPosition().z();
}

bool operator==(Object obj1, Object obj2)
{
    return obj1.getName() == obj2.getName();
}

void pictureFrame::paintPlanet(QPainter * painter)
{
    Object object(Point3D(), Point3D(), 0, 0, "", QColor());
    QList<Object> transformedObjects;
    for (int i = 0; i < objects->size(); i++)
        transformedObjects.append(Object(transform(objects->at(i).getPosition()),
                                         transform(objects->at(i).getVelocity()),
                                         objects->at(i).getMass(), objects->at(i).getRadius()/static_cast<long double>(distanceScalar),
                                         objects->at(i).getName(), objects->at(i).getColor()));
    qSort(transformedObjects);
    for (int planet = 0; planet < objects->size(); planet++)
    {
        object = transformedObjects.at(planet);
        int newPlanet = objects->lastIndexOf(object);
        if (newPlanet == -1)
            newPlanet = planet;
        double diameter = static_cast<double>(std::min(std::max(std::max(object.getRadius(),2.0L)/static_cast<long double>(distanceScalar), 2.0L), 30.0L));

        painter->setPen(object.getColor());
        painter->setBrush(object.getColor());
        // draw the object itself
        if (options->perspectiveView())
            drawEllipse(painter, object.getPosition(), static_cast<double>(diameter));
        else
            drawEllipse(painter, object.getPosition(), static_cast<double>(diameter));
        // draw its trail
        drawText(painter, object.getPosition() + Point3D(static_cast<long double>(diameter), static_cast<long double>(-diameter), static_cast<long double>(diameter)),
                          object.getName());

        // draw the velocity arrow
        if (options->displayVelocity())
        {
            Point3D velocityCurrent = transform(objects->at(newPlanet).getVelocity());
            painter->setPen(options->velocityColor());
            painter->setBrush(options->velocityColor());
            if (! velocityCurrent.isNull())
                drawLine(painter, object.getPosition() + velocityCurrent, object.getPosition());
        }

        // draw the acceleration arrow
        if (options->displayAccel())
        {
            Point3D accelCurrent = (transform(objects->at(newPlanet).getAccel()));
            painter->setPen(options->accelColor());
            painter->setBrush(options->accelColor());
            if (! accelCurrent.isNull())
                drawLine(painter, (object.getPosition() + 100000*accelCurrent), (object.getPosition()));
        }
    }
}

Point3D pictureFrame::transform(Point3D point)
{
    return transformMatrix*point;
}

void pictureFrame::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);//->visibleRegion());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2 + 10, height() / 2 + 10);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(-width()/2, -height()/2, width(), height());

//    painter->drawText(400, -400, 1000, 1000, 0, "PointerToOptions = " + toString(int(pointerToOptions)));
    paintAxes(&painter);

    if (options->displayGravField())
        paintGravField(&painter, objects);

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    QString message;
    message += "Time = " + toStringL(time);
    message += "\nThere are " + toStringI(objects->size()) + " objects in the system";
    if (removedObjects->getPosition().x() != 0.0L)
    {
        message += QString("\nObjects that have left the system:") +
                   "\n  number: " + toStringL(removedObjects->x()) +
                   "\n  mass: " + toStringL(removedObjects->getMass()) +
                   "\n  total momentum: " + toString(removedObjects->getMomentum());
        painter.drawText(width()/2 - 225, height()/2 - 90, 400, 400, 0, message);
    }
    else
        painter.drawText(width()/2 - 225, height()/2 - 40, 400, 400, 0, message);

    // draw the objects
    if (options->displayTrace())
        paintTrace(&painter);
    if (options->displayPlanets())
        paintPlanet(&painter);

    // draw the center of mass
    if (options->displayCenterOfMass())
    {
        painter.setPen(Qt::white);
        painter.setBrush(Qt::black);
        drawEllipse(&painter, transform(centerOfMass), qMin(qMax(2.0, 5/distanceScalar), 5.0));
        if (!options->systemCentered())
            drawLine(&painter, transform(centerOfMass),
                     transform(centerOfMass + options->systemVelocity()));
    }
    QFrame::paintEvent(event);
}

Point3D round(Point3D point, long double roundTo)
{
    return Point3D(int(point.x()*roundTo)/roundTo,
                   int(point.y()*roundTo)/roundTo,
                   int(point.z()*roundTo)/roundTo);
}

long double getDeflection(QPointF point, QList<Object> * objects)
{
    long double magnitude = 0;
    for (int i = 0; i < objects->size(); i++)
    {
        QPointF r = objects->at(i).getPosition().toPointF() - point;
        long double radius = static_cast<long double>(r.x()*r.x() + r.y()*r.y());
        magnitude += -0.025L*objects->at(i).getMass()/sqrtl(radius + 1);
//        if (radius < objects->at(i).getRadius())
//            magnitude += -0.025*objects->at(i).getMass()*radius;//*(objects->at(i).getPosition() - point);
//        else
//            magnitude += -0.025*objects->at(i).getMass()/(radius*radius);
    }
    return 1000*magnitude;
}

void findParabolaCoefficients(QPointF p1, QPointF p2, QPointF p3, long double& A, long double & B, long double & C)
{
    // (x_2-x_1)*(x_3^2-x_1 x_3-x_2 x_3+x_1 x_2)!=0
    // A = (-x_2 y_1+x_3 y_1+x_1 y_2-x_3 y_2-x_1 y_3+x_2 y_3)/demoninator
    // B = (x_1^2 (-y_2)+x_1^2 y_3+x_2^2 y_1-x_3^2 y_1+x_3^2 y_2-x_2^2 y_3)/demoninator
    // C = (x_3 x_1^2 y_2-x_2 x_1^2 y_3-x_3^2 x_1 y_2+x_2^2 x_1 y_3+x_2 x_3^2 y_1-x_2^2 x_3 y_1)/demoninator
    // denominator = ((x_2-x_1) (x_3-x_1) (x_3-x_2))
    long double x_1 = static_cast<long double>(p1.x()), y_1 = static_cast<long double>(p1.y());
    long double x_2 = static_cast<long double>(p2.x()), y_2 = static_cast<long double>(p2.y());
    long double x_3 = static_cast<long double>(p3.x()), y_3 = static_cast<long double>(p3.y());
    if (((x_1 - x_2) > zeroL) && ((x_1 - x_3) > zeroL) && ((x_2 - x_3) > zeroL))
    {
        long double denominator = ((x_2-x_1)*(x_3-x_1)*(x_3-x_2));
        A = (-x_2*y_1+x_3*y_1+x_1*y_2-x_3*y_2-x_1*y_3+x_2*y_3)/denominator;
        B = (x_1*x_1*(-y_2)+x_1*x_1*y_3+x_2*x_2*y_1-x_3*x_3*y_1+x_3*x_3*y_2-x_2*x_2*y_3)/denominator;
        C = (x_3*x_1*x_1*y_2-x_2*x_1*x_1*y_3-x_3*x_3*x_1*y_2+x_2*x_2*x_1*y_3+x_2*x_3*x_3*y_1-x_2*x_2*x_3*y_1)/denominator;
        return;
    }
    A = B = C = -1;
    return;
}

void pictureFrame::paintGravField(QPainter * painter, QList<Object> * objects)
{
    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);
    if (options->newtonian2D() || options->newtonian3D())
    {
        for (int index = 0; index < gravPoints.size(); index++)
        {
            painter->setPen(gravColors.at(index));
            painter->setBrush(gravColors.at(index));
            Point3D point = transform(gravPoints.at(index));
            drawLine(painter, point - transform(gravLengths.at(index)), point);
        }
    }
    else
    {
        const long double deflectionThreshhold = 4;
        const int spacing = options->deflectionSpacing();
        const int number = options->deflectionSquareWidth()/spacing;
        Point3D oldPoint;

        painter->setBrush(options->relativityColor());
        painter->setPen(options->relativityColor());
        for (int x = -number*spacing; x <= number*spacing; x += spacing)
        {
            oldPoint = Point3D(x, -(number + 1)*spacing, -getDeflection(QPointF(x, -(number + 1)*spacing), objects));
            for (int y = -number*spacing; y <= (number - 1)*spacing; y += spacing)
            {
                Point3D point1;
                if (std::abs(-getDeflection(QPointF(x, y), objects) - -getDeflection(QPointF(x, y + spacing), objects))
                        >= deflectionThreshhold ||
                        getDeflection(QPointF(x, y), objects) > 5)
                {
                    QPointF p1(y, static_cast<double>(-getDeflection(QPointF(x, y), objects))),
                            p2(y + 0.5*spacing, static_cast<double>(-getDeflection(QPointF(x, y + 0.5*spacing), objects))),
                            p3(y + spacing, static_cast<double>(-getDeflection(QPointF(x, y + spacing), objects)));

                    // find the coefficients forming the parabola between the two end points and the midpoint
                    long double A, B, C;
                    findParabolaCoefficients(p1, p2, p3, A, B, C);
                    Point3D newOldPoint = Point3D(x, y, A*y*y + B*y + C);
                    for (long double newY = y; newY <= y + spacing; newY += 0.1L*spacing)
                    {
                        point1 = Point3D(static_cast<long double>(x), newY, A*newY*newY + B*newY + C);
                        drawLine(painter, transform(point1), transform(newOldPoint));
                        newOldPoint = point1;
                    }
                }
                else
                {
                    point1 = Point3D(x, y, -getDeflection(QPointF(x, y), objects));
                    drawLine(painter, transform(point1),
                             transform(Point3D(x, y + spacing, -getDeflection(QPointF(x, y + spacing), objects))));
                }
                oldPoint = point1;
            }
        }
        for (int y = -number*spacing; y <= number*spacing; y += spacing)
        {
            oldPoint = Point3D(-(number + 1)*spacing, y, -getDeflection(QPointF(-(number + 1)*spacing, y), objects));
            for (int x = -number*spacing; x <= (number - 1)*spacing; x += spacing)
            {
                Point3D point1;
                if ((std::abs(-getDeflection(QPointF(x, y), objects) - -getDeflection(QPointF(x + spacing, y), objects))
                        >= deflectionThreshhold) ||
                        (getDeflection(QPointF(x, y), objects) > 5))
                {
                    QPointF p1(x, static_cast<double>(-getDeflection(QPointF(x, y), objects))),
                            p2(x + 0.5*spacing, static_cast<double>(-getDeflection(QPointF(x + 0.5*spacing, y), objects))),
                            p3(x + spacing, static_cast<double>(-getDeflection(QPointF(x + spacing, y), objects)));

                    // find the coefficients forming the parabola between the two end points and the midpoint
                    long double A, B, C;
                    findParabolaCoefficients(p1, p2, p3, A, B, C);
                    Point3D newOldPoint = Point3D(x, y, A*x*x + B*x + C);
                    for (long double newX = x; newX <= x + spacing; newX += 0.1L*spacing)
                    {
                        point1 = Point3D(newX, y, A*newX*newX + B*newX + C);
                        drawLine(painter, transform(point1), transform(newOldPoint));
                        newOldPoint = point1;
                    }
                }
                else
                {
                    point1 = Point3D(x, y, -getDeflection(QPointF(x, y), objects));
                    drawLine(painter, transform(point1), //transform(oldPoint));
                             transform(Point3D(x + spacing, y, -getDeflection(QPointF(x + spacing, y), objects))));
                }
                oldPoint = point1;
            }
        }
        painter->setBrush(Qt::white);
        painter->setPen(Qt::white);
    }
}

QPointF pictureFrame::to2D(Point3D point)
{
    Point3D camera(viewerPosition.x(), viewerPosition.y(), 1000);
    Point3D viewer(0, 0, viewerPosition.z());
    QPointF point2;
    if (options->perspectiveView())
        point2 = QPointF(static_cast<double>(point.x() - camera.x())*std::abs(static_cast<double>((camera.z() - viewer.z())/(camera.z() - point.z()))),
                         static_cast<double>(point.y() - camera.y())*std::abs(static_cast<double>((camera.z() - viewer.z())/(camera.z() - point.z()))));
    else
        point2 = QPointF(static_cast<double>(point.x() - camera.x()), static_cast<double>(point.y() - camera.y()));
    return (point2 - zoomPoint)/distanceScalar + zoomPoint;
}

void pictureFrame::drawEllipse(QPainter * painter, Point3D point, double width)
{
    painter->drawEllipse(to2D(point), width, width);
}

void pictureFrame::drawLine(QPainter * painter, Point3D point, Point3D fromPoint)
{
    painter->drawLine(to2D(fromPoint), to2D(point));
}

void pictureFrame::updateMatrix()
{
//{{1,0,0},{0,cos(x),-sin(x)},{0,sin(x),cos(x)}}*
//{{cos(y),0,sin(y)},{0,1,0},{-sin(y),0,cos(y)}}*
//{{cos(z),-sin(z),0},{sin(z),cos(z),0},{0,0,1}}
    // Matrix X
    // 1       0       0
    // 0 cos(xA) -sin(xA)
    // 0 sin(xA)  cos(xA)

    // Matrix Y
    //  cos(yA) 0 sin(yA)
    //        0 1       0
    // -sin(yA) 0 cos(yA)

    // Matrix Z
    // cos(zA) -sin(zA) 0
    // sin(zA)  cos(zA) 0
    //       0        0 1

    // for the special case X = -180, Y = Z = 0 (2D case)
    // the only matrix is
    // X = 1  0  0
    //     0 -1  0
    //     0  0 -1
    if ((options != nullptr) && (options->newtonian2D()))
    {
        transformMatrix = Matrix3x3::getIdentity();
        transformMatrix.setValue(1, 1, -1);
        transformMatrix.setValue(2, 2, -1);
        return;
    }

    transformMatrix = Matrix3x3::getIdentity();
    Matrix3x3 varMat;
    long double varAngle;
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
            varAngle = (viewerAngle.x() - 180.0L)*Pi/180.0L;
        else if (i == 1)
            varAngle = viewerAngle.y()*Pi/180.0L;
        else
            varAngle = viewerAngle.z()*Pi/180.0L;
        long double cosine = cosl(varAngle);
        long double sine = cosl(varAngle);
        if (std::abs(cosine) < 0.000001L) cosine = 0;
        if (std::abs(sine) < 0.000001L) sine = 0;

        varMat = Matrix3x3(0);

        if (i == 0) // X
        {
            varMat.setValue(0, 0, 1);
            varMat.setValue(1, 1, cosine);
            varMat.setValue(1, 2, -sine);
            varMat.setValue(2, 1, sine);
            varMat.setValue(2, 2, cosine);
        }
        else if (i == 1) // Y
        {
            varMat.setValue(0, 0, cosine);
            varMat.setValue(0, 2, sine);
            varMat.setValue(1, 1, 1);
            varMat.setValue(2, 0, -sine);
            varMat.setValue(2, 2, cosine);
        }
        else // Z
        {
            varMat.setValue(0, 0, cosine);
            varMat.setValue(0, 1, -sine);
            varMat.setValue(1, 0, sine);
            varMat.setValue(1, 1, cosine);
            varMat.setValue(2, 2, 1);
        }
        transformMatrix = transformMatrix*varMat;
    }
}

void pictureFrame::paintAxes(QPainter * painter)
{
    Point3D e[3];
    e[0] = transform(Point3D(50, 0, 0));
    e[1] = transform(Point3D( 0,50, 0));
    e[2] = transform(Point3D( 0, 0,50));

    QString axis;
    QColor color;
    for (int i = 0; i < 3; i++)
    {
        if (i == 0) {axis = "X"; color = Qt::red;}
        else if (i == 1) {axis = "Y"; color = Qt::blue;}
        else {axis = "Z"; color = Qt::green;}
        painter->setPen(color);
        QPointF offset(0 - this->width()/2 + 100, this->height()/2 - 100);
        QPointF newPoint = QPointF(static_cast<double>(e[i].x()), static_cast<double>(e[i].y())) + offset;
        painter->drawLine(newPoint, offset);
        painter->drawText(newPoint + QPointF(5, -5), axis + " axis");
        painter->drawEllipse(newPoint, 5, 5);
    }
}

void pictureFrame::drawText(QPainter * painter, Point3D point, QString text)
{
    painter->drawText(to2D(point), text);
}
