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
#include <Scenario.h>
#include <QFile>
#include <scenariodialog.h>
#include <Butcher_Tableau.h>

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

    void changeIntegrationType(QString newType);

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

    void setText();
    void connectEverything();
    void loadScenarios();

    bool loadingPlanets;
    Ui::Planets *ui;
    Options * options;
    ScenarioDialog * scenarioEditor;

    Point3D viewerPosition;
    Point3D viewerAngle;
    QPoint lastPos;
    Plane viewerPlane;
    QPoint zoomPoint;
    QList <Point3D> gravPoints;         // the list of the points that gravity is displayed at
    QList <Point3D> gravLength;         // the list of lengths of gravity at the points
    QList <QColor> gravColor;           // the color that the gravity should be
    Point3D::dataType systemTime;       // the time that the system has been running
    Point3D::dataType positionError;    // current position error, per body
    Point3D::dataType velocityError;    // current velocity error, per body
    Point3D::dataType positionErrorTol; // tolerable position error, per body
    Point3D::dataType velocityErrorTol; // tolerable velocity error, per body
    QString data;                       // this string holds the data that will be exported to a CSV file for analysis
    Object removedObjects;              // this holds properties of the objects that got too far from the system
    QTimer * timer;                     // timer for the animation
    QMessageBox * box;                  // for debug and messages
    Point3D centerOfMass;               // the current center of mass of the system
    QList<Object> objects;              // the list of the orbiting objects
    QList<tracePoints> trails;          // this is the list of the old trails that deleted objects leave behind
    Point3D systemMomentum;             // this is the system momentum. use it to find systemVelocity
    Point3D::dataType systemMass;       // this is all of the mass in the entire system
    QString integrationType;            // integration type
    bool integrationInitialize;         // initialize integration

    static constexpr Point3D::dataType G_fake = 0.025L;
    Point3D::dataType G;

//  use these constants when using the solar system realistic model
    static constexpr Point3D::dataType G_real_mks = 6.67e-11L;         // meters^3/(kilograms*seconds^2)
    static constexpr Point3D::dataType G_real_kmks = 6.67e-20L;         // kilometers^3/(kilograms*seconds^2)
    static constexpr Point3D::dataType G_real_AU_day_M_E = 8.88744E-10;   // AU^3/(earth mass * days^2)      Distance - AU, time - days, mass - Earth mass
    Point3D::dataType dt = 0.000976562500;
    Point3D::dataType dt2 = dt*dt;
    Point3D::dataType dt3 = dt*dt2;

    Point3D getRandomPoint(Point3D::dataType minRadius = 0, Point3D::dataType maxRadius = 100);

    void calcAccels(int arguments = 0, bool calcJerks = true);
    Point3D calcAccel(Point3D distance, Point3D::dataType mass);
    QVector<Point3D> calcJerk(Point3D distance, Point3D velocity, Point3D::dataType mass);
    void Euler();
    void EulerCromber();
    void VelocityVerlet();
    void Yoshida();
    void Hermite();
    void RK4();
    void RKF();
    void documentVector(QVector<Point3D> vect);
    static Point3D::dataType w_0;
    static Point3D::dataType w_1;
    static Point3D::dataType yoshidaC1;
    static Point3D::dataType yoshidaC4;
    static Point3D::dataType yoshidaC2;
    static Point3D::dataType yoshidaC3;
    static Point3D::dataType yoshidaD1;
    static Point3D::dataType yoshidaD3;
    static Point3D::dataType yoshidaD2;
//    butcherTableau RK4Tableau;
//    butcherTableau RKF1Tableau;

    QVector<Point3D> calcAccelerations(QVector<Point3D> positions, QVector<Point3D::dataType> masses);
    QVector<Point3D> calcFVector(Point3D::dataType t, QVector<Point3D> yVector, QVector<Point3D::dataType> masses);
    bool firstRun;

    static QString defaultIntegrationType;

    void calculateSystemProperties();
    Point3D::dataType calculateSystemMass();
    Point3D calculateSystemMomentum();
    Point3D::dataType calculateSystemEnergy();
    Point3D::dataType calculateSystemPotentialEnergy();
    Point3D::dataType calculateSystemKineticEnergy();
    long unsigned int linesPrinted;

    QString filename;
    QFile file, fileDebug;
    QTextStream outputToFile, outputToFileDebug;

    Point3D::dataType calculateEscapeVelocity(Object * planet);
    void setNumberOfPlanets(int number);
    void planetsCollide();
    Point3D calculateCenterOfMass();

    QString toStringO(Object planet, bool showEverything = false, bool CSVFormat = false);
    QString toStringP(Point3D point, bool CSVFormat = false) const;

    Point3D::dataType getTheta(Point3D point) {return static_cast<Point3D::dataType>(atan2(double(point.y()), double(point.x())));}
    void removeObject(int index, bool isTooFar = false);

    QVector<scenario> scenarios;
    scenario activeScenario;

    void setupSolarSystem();
    void setupMenus();
private slots:
    void saveStateButton_clicked();
};

#endif // PLANETS_H
