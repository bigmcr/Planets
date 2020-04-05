#ifndef PLANETS_H
#define PLANETS_H

#include <QMainWindow>
#include <QMessageBox>
#include <tracePoints.h>
#include "Object.h"
#include <QTimer>
#include <QMessageBox>
#include <Options.h>
#include <cmath>
#include <Point3D.h>
#include <plane.h>

namespace Ui {
    class Planets;
}

class Planets : public QMainWindow
{
    Q_OBJECT

public:
    explicit Planets(QWidget *parent = nullptr);
	~Planets();

public slots:

	void paintEvent(QPaintEvent *);
	// these are the slots that deal with MainFrame

	void forward();
	void status();
	void reset();
	void pause();
	void planet();

	void close();

    void planetsChangeNumber();

	void stopTimer() {timer->stop();}
	void update();
    void updateTrace(bool);

    void resetView();
    void changeView(QString newView);

//    void update3DRadioButton();
//    void update2DRadioButton();
    void updateViewOption();

private:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent * event);

    bool mouseIsRotating;
    bool mouseIsPanning;

    Plane calculateGravityPlane();
    void calculateGravityForce(Plane gravPlane);

    void loadPlanet();

//    Point3D calculateForceOnPoint(Point3D point, const int planet);
//    void calculateNextPosition(const int i);
    QColor getColor(int);
    int getNumber(QColor color);

    void setText();
    void connectEverything();

    bool loadingPlanets;
    Ui::Planets *ui;
    Options * options;

    void debug(QString, QString = "");

    Point3D viewerPosition;
    Point3D viewerAngle;
    QPoint lastPos;
    Plane viewerPlane;
    QPoint zoomPoint;
    QList <Point3D> gravPoints;  // the list of the points that gravity is displayed at
    QList <Point3D> gravLength;  // the list of lengths of gravity at the points
    QList <QColor> gravColor;    // the color that the gravity should be
    int paintNumber;             // used to keep track of the number of times the screen has been painted
    long double systemTime;      // the time that the system has been running
    QString data;                // this string holds the data that will be exported to a CSV file for analysis
    Object removedObjects;       // this holds properties of the objects that got too far from the system
    QTimer * timer;              // timer for the animation
    QMessageBox * box;           // for debug and messages
    Point3D centerOfMass;        // the current center of mass of the system
    QList<Object> objects;       // the list of the orbiting objects
    QList<tracePoints> trails;   // this is the list of the old trails that deleted objects leave behind
    Point3D systemMomentum;      // this is the system momentum. use it to find systemVelocity
    long double systemMass;      // this is all of the mass in the entire system
    static const int time = 25;  // the time in milliseconds between screen updates

    static constexpr long double G_fake = 0.025L;
    long double G;

//  use these constants when using the solar system realistic model
    static constexpr long double G_real = 6.67e-11L;         // meters^3/(kilograms*seconds^2)
    static constexpr long double dt = 1/(900000.0L);

    Point3D getRandomPoint(long double minRadius = 0, long double maxRadius = 100);

    void calculateSystemProperties();
    void calculateSystemMass();
    void calculateSystemMomentum();

    long double calculateEscapeVelocity(Object * planet);
    void setNumberOfPlanets(int number);
    void planetsCollide(int planet1, int planet2);
    long double calculateGravForce(const Object * obj1, const Object * obj2);
    Point3D calculateCenterOfMass();

    QString toStringO(Object planet, bool showEverything = false, bool CSVFormat = false);
    QString toStringP(Point3D point, bool CSVFormat = false) const;

    long double getTheta(Point3D point) {return static_cast<long double>(atan2(double(point.y()), double(point.x())));}
    void forwardNext();
    void removeObject(int index, bool isTooFar = false);

    void setupSolarSystem();
    void setupMenus();
private slots:
    void changeScenario1();
    void changeScenario2();
    void changeScenario3();
    void changeScenario4();
    void changeScenario5();
    void changeScenario6();
    void changeScenario7();
    void changeScenario8();
    void changeScenario9();
    void changeScenario0();
};

#endif // PLANETS_H
