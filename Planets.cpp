#include "Planets.h"
#include "ui_Planets.h"
#include "Point3D.h"
#include <plane.h>
#include <QMouseEvent>
#include <toString.h>

long double generateNormalNumbers(long double mu = 0.0L, long double sigma = 1.0L)
{
	static bool deviateAvailable=false;        //        flag
    static long double storedDeviate;          //        deviate from previous calculation
    long double polar, rsquared, var1, var2;

	//        If no deviate has been stored, the polar Box-Muller transformation is
	//        performed, producing two independent normally-distributed random
	//        deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable)
	{
		//        choose pairs of uniformly distributed deviates, discarding those
		//        that don't fall within the unit circle
		do
		{
            var1 = 2.0L*( static_cast<long double>(rand())/static_cast<long double>(RAND_MAX) ) - 1.0L;
            var2 = 2.0L*( static_cast<long double>(rand())/static_cast<long double>(RAND_MAX) ) - 1.0L;
			rsquared=var1*var1+var2*var2;
        } while ( rsquared >= 1.0L || rsquared == 0.0L);

		//        calculate polar tranformation for each deviate
        polar = sqrtl(-2.0L*logl(rsquared)/rsquared);

		//        store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;

		//        return second deviate
		return var2*polar*sigma + mu;
	}

	//        If a deviate is available from a previous call to this function, it is
	//        returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}

template <typename T>
inline T weightedAverage(T x, T y, long double xD, long double yD)
{
    return (x*xD + y*yD)/(xD+yD);
}

QColor weightedAverageColor(QColor color1, QColor color2, long double mass1, long double mass2)
{
	int r1, r2, g1, g2, b1, b2, a1, a2;
	color1.getRgb(&r1, &g1, &b1, &a1);
	color2.getRgb(&r2, &g2, &b2, &a2);
    return QColor(weightedAverage(r1, r2, mass1, mass2), weightedAverage(g1, g2, mass1, mass2),
                  weightedAverage(b1, b2, mass1, mass2), weightedAverage(a1, a2, mass1, mass2));
}

long double getRand(long double min, long double max, bool canBeNegative)
{
	if (min < 0) min = 0;
    long double range = (max - min) / RAND_MAX;
    long double random = min + rand()*range;
	if (! canBeNegative)
		return random;
	double negative = rand() - RAND_MAX/2;
	if (negative >= 0) return random;
	return -random;
}

Planets::Planets(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::Planets)
{
    ui->setupUi(this);
//    setupMenus();
    box = new QMessageBox(this);
    loadingPlanets = false;

    systemTime = 0;
    timer = new QTimer(this);                   // timer for the animation
    timer->setInterval(25);
    box = new QMessageBox(this);                // for debug and messages
    options = new Options();              // the options dialog
    mouseIsRotating = false;
    mouseIsPanning = false;
    lastPos = QPoint(0,0);

    connectEverything();

    ui->viewGroupBox->setStyleSheet("color: green");
    resize(1000, 800);
    reset();
}

void Planets::setupMenus()
{
    QMenu * optionsMenu = new QMenu("&Options");
    QMenu * display = new QMenu("&Display options");
    QAction * action = new QAction("Display Planets", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    display->addAction(action);

    action = new QAction("Display Center of Mass", nullptr);
    action->setCheckable(true);
    action->setChecked(false);
    display->addAction(action);

    action = new QAction("Display Planetary Velocity", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    display->addAction(action);

    display->addAction("Display Planetary Acceleration");
    action->setCheckable(true);
    action->setChecked(true);
    display->addAction(action);

    optionsMenu->addMenu(display);
    QMenu * sysMech = new QMenu("System Mechanics Options");

    action = new QAction("Use Polar Coordinates", nullptr);
    action->setCheckable(true);
    action->setChecked(false);
    sysMech->addAction(action);

    action = new QAction("Display Planet Trails", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    sysMech->addAction(action);

    action = new QAction("Keep System Centered", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    sysMech->addAction(action);

    optionsMenu->addMenu(sysMech);
    QMenu * grav = new QMenu("Gravity Field Options");

    action = new QAction("Display Local Gravity Field", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    grav->addAction(action);

    optionsMenu->addMenu(grav);

    ui->menuBar->addMenu(optionsMenu);
    return;
}

void Planets::changeScenario1()
{
    ui->comboBox->setCurrentIndex(0);
}

void Planets::changeScenario2()
{
    ui->comboBox->setCurrentIndex(1);
}

void Planets::changeScenario3()
{
    ui->comboBox->setCurrentIndex(2);
}

void Planets::changeScenario4()
{
    ui->comboBox->setCurrentIndex(3);
}

void Planets::changeScenario5()
{
    ui->comboBox->setCurrentIndex(4);
}

void Planets::changeScenario6()
{
    ui->comboBox->setCurrentIndex(5);
}

void Planets::changeScenario7()
{
    ui->comboBox->setCurrentIndex(6);
}

void Planets::changeScenario8()
{
    ui->comboBox->setCurrentIndex(7);
}

void Planets::changeScenario9()
{
    ui->comboBox->setCurrentIndex(8);
}

void Planets::changeScenario0()
{
    ui->comboBox->setCurrentIndex(9);
}

void Planets::connectEverything()
{
    // These connections connect the different scenario shortcuts with the different actions for the combobox
    connect(this->ui->actionEarth_and_Sun, SIGNAL(triggered()), this, SLOT(changeScenario1()));
//    connect(this->ui->actionLagrangian_Points, SIGNAL(triggered()), this, SLOT(changeScenario2()));
    connect(this->ui->actionDouble_Slingshot, SIGNAL(triggered()), this, SLOT(changeScenario3()));
    connect(this->ui->actionBinary_System, SIGNAL(triggered()), this, SLOT(changeScenario4()));
    connect(this->ui->actionSun_and_Comets, SIGNAL(triggered()), this, SLOT(changeScenario5()));
    connect(this->ui->action4_Star_Ballet, SIGNAL(triggered()), this, SLOT(changeScenario6()));
    connect(this->ui->actionDouble_Double, SIGNAL(triggered()), this, SLOT(changeScenario7()));
    connect(this->ui->actionSolar_System, SIGNAL(triggered()), this, SLOT(changeScenario8()));
    connect(this->ui->actionSolar_System_Formation, SIGNAL(triggered()), this, SLOT(changeScenario9()));
    connect(this->ui->actionFree_Form, SIGNAL(triggered()), this, SLOT(changeScenario0()));

    // These connections are for the viewer settings in the main display.
    // They ensure that the main display setting and the Options setting are synchronized
    connect(this->ui->display2DRadioButton, SIGNAL(clicked()), this->options, SLOT(check2DRadioButton()));
    connect(this->ui->display3DRadioButton, SIGNAL(clicked()), this->options, SLOT(check3DRadioButton()));
    connect(this->ui->displayRelativityRadioButton, SIGNAL(clicked()), this->options, SLOT(checkRelativityRadioButton()));

    connect(this->options, SIGNAL(displayTypeChanged()), this, SLOT(updateViewOption()));

    // this is the normal UI interface. These connections just make everything work correctly
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(reset()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));                        // change tab to the current tab
    connect(ui->forwardButton, SIGNAL(clicked()), SLOT(forward()));
    connect(timer, SIGNAL(timeout()), SLOT(forward()));
    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    connect(ui->statusButton, SIGNAL(clicked()), this, SLOT(status()));

    // these are the Free Form interface buttons/spin boxes. They all connect to planet();
    connect(ui->massSpin, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->positionX, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->positionY, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->positionZ, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->velocityX, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->velocityY, SIGNAL(valueChanged(double)), this, SLOT(planet()));
    connect(ui->nameLine, SIGNAL(textChanged(QString)), this, SLOT(planet()));

    connect(ui->planetNumber, SIGNAL(valueChanged(int)), this, SLOT(planetsChangeNumber()));

    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->distanceScalarSpin, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerAngleXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerAngleYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerAngleZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerPositionXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerPositionYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->viewerPositionZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));

    connect(ui->actionOptions, SIGNAL(triggered()), options, SLOT(show()));

    connect(options, SIGNAL(somethingChanged()), this, SLOT(update()));
    connect(options, SIGNAL(traceChanged(bool)), this, SLOT(updateTrace(bool)));

    connect(ui->viewComboBox, SIGNAL(activated(QString)), this, SLOT(changeView(QString)));
}

void Planets::changeView(QString newView)
{
    if (newView == "Front")
    {
        ui->viewerAngleXSpinBox->setValue(0);
        ui->viewerAngleYSpinBox->setValue(0);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Back")
    {
        ui->viewerAngleXSpinBox->setValue(180);
        ui->viewerAngleYSpinBox->setValue(0);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Left")
    {
        ui->viewerAngleXSpinBox->setValue(0);
        ui->viewerAngleYSpinBox->setValue(90);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Right")
    {
        ui->viewerAngleXSpinBox->setValue(0);
        ui->viewerAngleYSpinBox->setValue(-90);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Top")
    {
        ui->viewerAngleXSpinBox->setValue(-90);
        ui->viewerAngleYSpinBox->setValue(0);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Bottom")
    {
        ui->viewerAngleXSpinBox->setValue(90);
        ui->viewerAngleYSpinBox->setValue(0);
        ui->viewerAngleZSpinBox->setValue(0);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }
    else if (newView == "Isometric")
    {
        ui->viewerAngleXSpinBox->setValue(0);
        ui->viewerAngleYSpinBox->setValue(35.264);
        ui->viewerAngleZSpinBox->setValue(45);

        ui->viewerPositionXSpinBox->setValue(0);
        ui->viewerPositionYSpinBox->setValue(0);
        ui->viewerPositionZSpinBox->setValue(0);
    }

    zoomPoint = QPoint(0,0);
    update();
}

void Planets::resetView()
{
    ui->viewerAngleXSpinBox->setValue(0);
    ui->viewerAngleYSpinBox->setValue(0);
    ui->viewerAngleZSpinBox->setValue(0);

    ui->viewerPositionXSpinBox->setValue(0);
    ui->viewerPositionYSpinBox->setValue(0);
    ui->viewerPositionZSpinBox->setValue(0);

    zoomPoint = QPoint(0,0);
}

Planets::~Planets()
{
	delete options;
    delete ui;
}

void Planets::close()
{
	QMainWindow::close();
	options->close();
}

void Planets::paintEvent(QPaintEvent * event)
{
    if (ui->comboBox->currentText() == "Free Form")
        loadPlanet();
    QPainter painter(this);
    ui->mainFrame->setZoomPoint(zoomPoint);
    ui->mainFrame->setDistanceScalar(ui->distanceScalarSpin->value());
    ui->mainFrame->setCenterOfMass(centerOfMass);
    ui->mainFrame->setViewerPosition(viewerPosition);
    ui->mainFrame->setViewerAngle(viewerAngle);
    ui->mainFrame->setOptionsPointer(options);
    ui->mainFrame->setTraces(&trails);
    ui->mainFrame->setObjects(&objects);
    ui->mainFrame->setTime(systemTime);
    ui->mainFrame->setRemovedObjects(&removedObjects);
    painter.drawText(100, 0, 1000, 1000, 0, "B1 = " + toStringP(viewerPlane.getBasis1(),true));
    painter.drawText(100, 100, 1000, 1000, 0, "B1 = " + toStringP(viewerPlane.getBasis2(), true));
    QMainWindow::paintEvent(event);
}

void Planets::setText()
{
    QString message;
    for (int i = 0; i < objects.size(); i++)
        message += toStringO(objects.at(i), true) + "\n";
    if (timer->isActive())
        message += "timer is active";
    else
        message += "timer is not active";
    data += toStringL(systemTime) + ",";
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setVelocity(objects.at(i).getVelocity() + options->systemVelocity()*(! options->systemCentered()));
        data += toStringO(objects.at(i), true, true);
        objects[i].setVelocity(objects.at(i).getVelocity() - options->systemVelocity()*(! options->systemCentered()));
    }
    data += "\n";

    ui->textBrowser->setText(message);
}

void Planets::debug(QString message, QString subMessage)
{
	box->setText(message);
	box->setDetailedText(subMessage);
	box->exec();
}

int Planets::getNumber(QColor color)
{
	if (color == Qt::white) return 0;
	if (color == Qt::black) return 1;
	if (color == Qt::red) return 2;
	if (color == Qt::darkRed) return 3;
	if (color == Qt::green) return 4;
	if (color == Qt::darkGreen) return 5;
	if (color == Qt::blue) return 6;
	if (color == Qt::darkBlue) return 7;
	if (color == Qt::cyan) return 8;
	if (color == Qt::darkCyan) return 9;
	if (color == Qt::magenta) return 10;
	if (color == Qt::darkMagenta) return 11;
	if (color == Qt::yellow) return 12;
	if (color == Qt::darkYellow) return 13;
	if (color == Qt::gray) return 14;
	if (color == Qt::lightGray) return 15;
	if (color == Qt::darkGray) return 16;
	return 0;
}

QColor Planets::getColor(int newColor)
{
	switch (newColor)
	{
	case 0:
		return Qt::white;
	case 1:
		return Qt::black;
	case 2:
		return Qt::red;
	case 3:
		return Qt::darkRed;
	case 4:
		return Qt::green;
	case 5:
		return Qt::darkGreen;
	case 6:
		return Qt::blue;
	case 7:
		return Qt::darkBlue;
	case 8:
		return Qt::cyan;
	case 9:
		return Qt::darkCyan;
	case 10:
		return Qt::magenta;
	case 11:
		return Qt::darkMagenta;
	case 12:
		return Qt::yellow;
	case 13:
		return Qt::darkYellow;
	case 14:
		return Qt::gray;
	case 15:
		return Qt::lightGray;
	case 16:
		return Qt::darkGray;
	}
	return Qt::white;
}

void Planets::planetsChangeNumber()
{
    if (ui->planetComboBox->currentText() == "Free Form")
    {
        stopTimer();
        ui->textBrowser->setText("This is a test.\nThere are " + toStringI(objects.size()) + " objects in the system.");
        if (ui->planetNumber->value() > objects.size())
        {
            debug("Adding " + toStringI(ui->planetNumber->value() - objects.size()) + " objects to the system.");
        }
    }
    return;
}

void Planets::updateViewOption()
{
    if (options->newtonian2D()) ui->display2DRadioButton->setChecked(true);
    if (options->newtonian3D()) ui->display3DRadioButton->setChecked(true);
    if (options->deflection()) ui->displayRelativityRadioButton->setChecked(true);
}


void Planets::pause()
{
    if (timer->isActive())
        timer->stop();
    else
        timer->start();
}

void Planets::planetsCollide(int planet1, int planet2)
{
//    debug(toString(objects.at(planet1)) + " and planet " + toString(objects.at(planet2)) + " are colliding!");
    if (planet1 > planet2)
    {
        int temp;
        temp = planet1;
        planet1 = planet2;
        planet2 = temp;
    }
    Point3D position = weightedAverage(
                           objects.at(planet1).getPosition(),
                           objects.at(planet2).getPosition(),
                           objects.at(planet1).getMass(),
                           objects.at(planet2).getMass());
    objects[planet1].addToTrace(position);
    objects[planet2].addToTrace(position);

    Object star1 = objects.at(planet1), star2 = objects.at(planet2);

    removeObject(planet2, false);
    removeObject(planet1, false);

    Point3D momentum = star1.getMomentum() + star2.getMomentum();
    long double mass = star1.getMass() + star2.getMass();
    long double radius = weightedAverage(star1.getRadius(), star2.getRadius(), star1.getMass(), star2.getMass());
    QString name;
    if (star1.getMass() >= star2.getMass() * 9) name = star1.getName();
    else if (star2.getMass() >= star1.getMass() * 9) name = star2.getName();
    else name = star1.getName() + " + " + star2.getName();
    QColor color = weightedAverageColor(
                       star1.getColor(), star2.getColor(),
                       star1.getMass(), star2.getMass());

    trails.append(tracePoints(star1.getTrace(), star1.getTrace().size(), star1.getColor()));
    trails.append(tracePoints(star2.getTrace(), star2.getTrace().size(), star2.getColor()));

    objects.insert(planet1, Object(position, momentum/mass, mass, radius, name, color));
}

QString Planets::toStringO(Object planet, bool showEverything, bool CSVFormat)
{
	QString message = planet.getName();
	if (showEverything == false)
	{
        message += "\n  Mass: " + toStringL(planet.getMass());
        message += "\n  Position: " + toStringP(planet.getPosition());
        message += "\n  Velocity: " + toStringP(planet.getVelocity());
	}
	else
	{
        message += "\n  mass: " + toStringL(planet.getMass());
        message += "\n  position: " + toStringP(planet.getPosition());
        message += "\n  velocity: " + toStringP(planet.getVelocity());
        message += "\n  acceleration: " + toStringP(planet.getAccel());
        message += "\n  radius: " + toStringL(planet.getRadius());
	}
	if (CSVFormat)
	{
            //data += Name,                     Mass                    Position X,Position Y, Position Z
            message = planet.getName() + "," + toStringL(planet.getMass()) + "," + toStringP(planet.getPosition(), true) +
                      //Velocity X, Velocity Y, Velocity Z, Speed
                      toStringP(planet.getVelocity(), true) + toStringL(distance(planet.getVelocity())) + "," +
                      //|Momentum|
                      toStringL(distance(planet.getVelocity()*planet.getMass())) + "," +
                      //Accel X, Accel Y, Accel Z
                      toStringP(planet.getAccel(), true);
	}
	return message;
}

void Planets::calculateSystemProperties()
{
	calculateSystemMass();
	calculateSystemMomentum();
	calculateCenterOfMass();
}

void Planets::setupSolarSystem()
{
	//	Name	Aphelion (Earth)	Min Velocity (Earth)	Mass (Earth)     Radius(Earth)
	//	Sun 	0                  	0	            0	               0	        1.9891E+30	332981.7865
	//	Earth	149598261           1	            29.78	            1	        5.9736E+24	1
	// for units, I will use the AU for distance, Me for mass, and solar day for time
	// 1 AU = 149597870.7 kilometers
	// 1 Me = 5.9742e24 kilograms
	// 1 solar day = 23 hours, 56 minutes, 4.091 seconds = 86164.091
	// G = 11.172961e-36 meters^3 / (kilograms * seconds^2)
	//   = 11.172961e-36 * (meters/1000)^3 / (kilograms * seconds^2)
	//   = 11.172961e-36 * ((meters/1000)/149597870.7)^3 * / (


    long double SunMass, SunRadius;
    long double Mercury[4], Venus[4], Earth[4], Mars[4];
    long double Jupiter[4], Saturn[4], Uranus[4];
    long double Neptune[4], Pluto[4];

//  planet[0] = Aphelion, planet[1] = Min orbital Velocity, planet[2] = Mass, planet[3] = Radius
//  these are the values that work fine when G = 0.025 and DISTANCE_SCALAR = 1
    Earth[0] = 152098232; Earth[1] = 29.29L*299792458L/10.0L/*29.29*/; Earth[2] = 5.9736L*powl(10,24); Earth[3] = 6371L;
//	Earth[0] = 1.4960E+08; Earth[1] = 2.9780E+01; Earth[2] = 5.9736E+24;
//	Earth[0] = 4.9901E+02; Earth[1] = 8.5824E-03; Earth[2] = 1;
    SunMass = Earth[2]*332981.7865L; SunRadius = Earth[3]*109.166535866L;
    Mercury[0] = Earth[0]*0.38709708L; Mercury[1] = Earth[1]*1.607454668L;
    Mercury[2] = Earth[2]*0.055279898L; Mercury[3] = Earth[3]*0.382938314L;
    Venus[0] = Earth[0]*0.723331831L; Venus[1] = Earth[1]*1.175957018L;
    Venus[2] = Earth[2]*0.815002678L; Venus[3] = Earth[3]*0.949897975L;
    Mars[0] = Earth[0]*1.523675131L; Mars[1] = Earth[1]*0.808495635L;
    Mars[2] = Earth[2]*0.10744777L; Mars[3] = Earth[3]*0.533071731L;
    Jupiter[0] = Earth[0]*5.204253009L; Jupiter[1] = Earth[1]*0.438885158L;
    Jupiter[2] = Earth[2]*317.8317932L; Jupiter[3] = Earth[3]*11.221472296L;
    Saturn[0] = Earth[0]*9.581992196L; Saturn[1] = Earth[1]*0.325386165L;
    Saturn[2] = Earth[2]*95.16204634L; Saturn[3] = Earth[3]*9.459739444L;
    Uranus[0] = Earth[0]*19.22936178L; Uranus[1] = Earth[1]*0.228676964L;
    Uranus[2] = Earth[2]*14.53227534L; Uranus[3] = Earth[3]*4.011772092L;
    Neptune[0] = Earth[0]*30.10358297L; Neptune[1] = Earth[1]*0.182337139L;
    Neptune[2] = Earth[2]*17.14711397L; Neptune[3] = Earth[3]*3.886987914L;
    Pluto[0] = Earth[0]*39.48158376L; Pluto[1] = Earth[1]*0.157824043L;
    Pluto[2] = Earth[2]*0.002184612L; Pluto[3] = Earth[3]*0.180976299L;
    objects.append(Object(Point3D(0,          0), Point3D(0, 0, 0),          SunMass,    SunRadius,  "Sun",       Qt::yellow));
    objects.append(Object(Point3D(Mercury[0], 0), Point3D(0, Mercury[1]), Mercury[2], Mercury[3], "Mercury",   Qt::red));
    objects.append(Object(Point3D(Venus[0],   0), Point3D(0, Venus[1]),   Venus[2],   Venus[3],   "Venus",     Qt::yellow));
    objects.append(Object(Point3D(Earth[0],   0), Point3D(0, Earth[1]),   Earth[2],   Earth[3],   "Earth",     Qt::cyan));
    objects.append(Object(Point3D(Mars[0],    0), Point3D(0, Mars[1]),    Mars[2],    Mars[3],    "Mars",      Qt::red));
    objects.append(Object(Point3D(Jupiter[0], 0), Point3D(0, Jupiter[1]), Jupiter[2], Jupiter[3], "Jupiter",   Qt::red));
    objects.append(Object(Point3D(Saturn[0],  0), Point3D(0, Saturn[1]),  Saturn[2],  Saturn[3],  "Saturn",    Qt::magenta));
    objects.append(Object(Point3D(Uranus[0],  0), Point3D(0, Uranus[1]),  Uranus[2],  Uranus[3],  "Uranus",    Qt::magenta));
    objects.append(Object(Point3D(Neptune[0], 0), Point3D(0, Neptune[1]), Saturn[2],  Saturn[3],  "Neptune",   Qt::magenta));
    objects.append(Object(Point3D(Pluto[0],   0), Point3D(0, Pluto[1]),   Pluto[2],   Pluto[3],   "Pluto",     Qt::magenta));

/*    objects.append(Object(Point3D(935918L, 7.77E+07L), Point3D(-87812.2L, 112664L),    1.99E+30L,    SunRadius,  "Sun", Qt::yellow));
    objects.append(Object(Point3D(7.01E+07L, 8.13E+07L), Point3D(-6.90E+07L, 1.30E+09L), Mercury[2], Mercury[3], "Mercury",   Qt::red));
    objects.append(Object(Point3D(8.56E+07L,   2.99E+06L), Point3D(6.85E+08L, 7.58E+08L),   Venus[2],   Venus[3],   "Venus",     Qt::yellow));
    objects.append(Object(Point3D(-1.17E+08L,   -1.27E+07L), Point3D(5.39E+08L, -7.23E+08L),   Earth[2],   Earth[3],   "Earth",     Qt::cyan));
    objects.append(Object(Point3D(6.60E+07L,    2.97E+08L), Point3D(-6.95E+08L, 1.90E+08L),    Mars[2],    Mars[3],    "Mars",      Qt::red));
    objects.append(Object(Point3D(7.89E+08L, 1.58E+08L), Point3D(-3.99E+07L, 3.83E+08L), Jupiter[2], Jupiter[3], "Jupiter",   Qt::red));
    objects.append(Object(Point3D(2.67E+07L,  -1.37E+09L), Point3D(2.88E+08L, 3.36E+06L),  Saturn[2],  Saturn[3],  "Saturn",    Qt::magenta));
    objects.append(Object(Point3D(-7.88E+08L,  -2.66E+09L), Point3D(1.97E+08L, -6.01E+07L),  Uranus[2],  Uranus[3],  "Uranus",    Qt::magenta));
    objects.append(Object(Point3D(3.31E+09L, -3.04E+09L), Point3D(1.12E+08L, 1.16E+08L), Saturn[2],  Saturn[3],  "Neptune",   Qt::magenta));
    objects.append(Object(Point3D(-4.40E+09L,   -3.36E+09L), Point3D(8.81E+07L, -1.19E+08L),   Pluto[2],   Pluto[3],   "Pluto",     Qt::magenta));*/

    Point3D position = objects.at(0).getPosition();

	for (int i = 0; i < objects.size(); i++)
		objects[i].setPosition(objects.at(i).getPosition() - position);
}

long double Planets::calculateEscapeVelocity(Object *planet)
{
    if (planet == nullptr) return 0.0L;
    long double sum = 0.0L;
//	debug("Calculating the escape velocity for the " + planet->getName());
	for (int i = 0; i < objects.size(); i++)
	{
//		debug("Continuing with " + objects.at(i).getName() + "\nG = " + QString::number(G) +
//			  "\nM = " + QString::number(systemMass - planet->getMass()) +
//			  "\nr = " + QString::number(distance(planet->getPosition() - objects.at(i).getPosition())));
        sum  += (2*G*(systemMass - planet->getMass())/
							  distance(planet->getPosition() - objects.at(i).getPosition()))
				*(2*G*(systemMass - planet->getMass())/
                  distance(planet->getPosition() - objects.at(i).getPosition()));
	}
    return sqrtl(sqrtl(sum));
}

void Planets::calculateSystemMass()
{
	systemMass = 0;
	for (int i = 0; i < objects.size(); i++)
		systemMass += objects.at(i).getMass();
}

void Planets::calculateSystemMomentum()
{
        systemMomentum = Point3D(0, 0, 0);
	for (int i = 0; i < objects.size(); i++)
		systemMomentum += objects.at(i).getMomentum();
}

Point3D Planets::getRandomPoint(long double minRadius, long double maxRadius)
{
	if (minRadius > maxRadius)
	{
        long double j = minRadius;
		minRadius = maxRadius;
		maxRadius = j;
	}

    long double radius = getRand(minRadius, maxRadius, false);
    long double x = getRand(0, radius, true);
    long double y = getRand(0, radius, true);
    int isNegative = rand()%10000 - 5000;
	if (isNegative == 0) isNegative++;
    long double z = sqrtl(std::abs(radius*radius - x*x - y*y)) * isNegative/std::abs(isNegative);
    return Point3D(x, y, z);
}

void Planets::removeObject(int index, bool isTooFar)
{
	if (isTooFar)
	{
		// add one to the removed objects counter, which is x of the position
                removedObjects.setPosition(removedObjects.getPosition() + Point3D(1, 0));
		// add the removed mass to the object
		removedObjects.setMass(removedObjects.getMass() + objects.at(index).getMass());
		// add the momuntum together by adding the individual momentums and dividing by the sum of the masses
		removedObjects.setVelocity((removedObjects.getMomentum() + objects.at(index).getMomentum())
								   /removedObjects.getMass());
		//remove the momentum the object carried with it from the system
		systemMomentum -= objects.at(index).getMomentum();
	}

        trails.append(tracePoints(objects[index].getTrace(),
                                  objects[index].getTrace().size(),
                                  objects.at(index).getColor()));

	objects.removeAt(index);
	ui->planetNumber->setMaximum(ui->planetNumber->maximum() - 1);
	ui->planetComboBox->clear();
	for (int i = 0; i < objects.size(); i++)
		ui->planetComboBox->insertItem(i, objects.at(i).getName());
}

long double Planets::calculateGravForce(const Object * planet1, const Object * planet2)
{
    long double radius = distance(planet1->getPosition() - planet2->getPosition());
	return 0 - G * planet1->getMass() * planet2->getMass()/ (radius * radius);
}
/*
Point3D Planets::calculateForceOnPoint(Point3D point, const int planet)
{
	Point3D accelCurrent;
	for (int k = 0; k < objects.size(); k++)
	{
		if (k == planet) continue;
		accelCurrent += calculateGravForce(&objects.at(planet), &objects.at(k))
			* normalize(objects.at(planet).getPosition() - objects.at(k).getPosition());
	}
	return accelCurrent;
}

void Planets::calculateNextPosition(const int i)
{
//  this calculates position via the PEFRL algorithm
    Point3D position = objects[i].getPosition();
    Point3D velocity = objects[i].getVelocity();

    double const_A = 0.1786178958448091;
    double const_B = -0.2123418310626054;
    double const_C = -0.066264582669818491;
	position = position + const_A*dt*velocity;// (38a)
    velocity = velocity + (1 - 2*const_B)*dt/2.0*calculateForceOnPoint(position, i);// (38b)
    position = position + const_C*dt*velocity;// (38c)
    velocity = velocity + const_B*dt*calculateForceOnPoint(position, i);// (38d)
    position = position + (1 - 2*(const_C + const_A))*dt*velocity;// (PEFRL algorithm) (38e)
    velocity = velocity + const_B*dt*calculateForceOnPoint(position, i);// (38f)
    position = position + const_C*dt*velocity;// (38g)
    velocity = velocity + (1 - 2*const_B)*dt/2.0*calculateForceOnPoint(position, i);// (38h)
    position = position + const_A*dt*velocity;// (38i)
    objects[i].setPosition(position);
    objects[i].setVelocity(velocity);
}
*/
Plane Planets::calculateGravityPlane()
{
    int checked = 0, planet1, planet2, planet3, mode = 0;
    bool useCenterOfMass = true;
    options->getGravityPlane(mode, planet1, planet2, planet3, objects.size(), checked);
    Point3D pt1, pt2, pt3;
    if (mode == 0) // xy plane selected
    {
        pt1 = Point3D(0,0,0);
        pt2 = Point3D(1,0,0);
        pt3 = Point3D(0,1,0);
    }
    else if (mode == 1) // xz plane selected
    {
        pt1 = Point3D(0,0,0);
        pt2 = Point3D(1,0,0);
        pt3 = Point3D(0,0,1);
    }
    else if (mode == 2) // yz plane selected
    {
        pt1 = Point3D(0,0,0);
        pt2 = Point3D(0,1,0);
        pt3 = Point3D(0,0,1);
    }
    // none of the preset planes are selected, so use the planets
    else
    {
        // none of the checkboxes are checked, so use the first planet
        if (checked == 0)
        {
            pt1 = objects.at(0).getPosition();
            const int size = objects.at(0).getTrace().size();
            if (size < 3) // if the simulation just started, so there are no tracePoints yet, go with the below points
            {
                pt2 = Point3D(0, 1, 0);
                pt3 = Point3D(1, 0, 0);
            }
            else // if the simulation has been going for a while, use the trace points
            {
                pt2 = objects.at(0).getTrace().at(size - 3);
                pt3 = objects.at(0).getTrace().at(size - 2);
            }
        }
        else if (checked == 1)// one planet is selected
        {
            pt1 = objects.at(planet1).getPosition();
            const int size = objects.at(planet1).getTrace().size();
            if (size < 3) // if the simulation just started, so there are no tracePoints yet, go with the below points
            {
                pt2 = Point3D(0, 1, 0);
                pt3 = Point3D(1, 0, 0);
            }
            else // if the simulation has been going for a while, use the trace points
            {
                pt2 = objects.at(planet1).getTrace().at(size - 3);
                pt3 = objects.at(planet1).getTrace().at(size - 2);
            }
        }
        else if (checked == 2)// two planets are selected
        {
            pt1 = objects.at(planet1).getPosition();
            pt2 = objects.at(planet2).getPosition();
            const int size = objects.at(planet1).getTrace().size();
            if (size < 4)
                pt3 = Point3D(1, 0, 0);
            else
                pt3 = objects.at(planet1).getTrace().at(size - 2);
        }
        else // three or more planets are selected
        {
            pt1 = objects.at(planet1).getPosition();
            pt2 = objects.at(planet2).getPosition();
            pt3 = objects.at(planet3).getPosition();
        }
        useCenterOfMass = false;
        if (options->gravPlaneGoesThroughCenterOfMass()) pt3 = centerOfMass;
    }
    Plane basePlane(pt1, pt2, pt3);
//    if (options->gravPlaneGoesThroughCenterOfMass() && useCenterOfMass) basePlane.setAddPoint(centerOfMass);
    basePlane.setBasisLength(options->arrowSpacing());
//    debug("Mode = " + toString(mode) + "\nPt1 = " + toString(pt1, true) + "\nPt2 = " +
//          toString(pt2, true) + "\nPt3 = " + toString(pt3, true));
    return basePlane;
}

void Planets::calculateGravityForce(Plane plane)
{
    plane.setBasisLength(1);
    gravPoints.clear();
    gravLength.clear();
    gravColor.clear();
    const int spacing = options->arrowSpacing();
    const int number = options->arrowSquareWidth()/spacing;
    for (int x = -number*spacing; x <= number*spacing; x += spacing)
        for (int y = -number*spacing; y <= (number - 1)*spacing; y += spacing)
        {
            Point3D point = plane.at(x,y);
//            point = Point3D(round(point.x(), 25), round(point.y(), 25), round(point.z(),25));
            Point3D field(0,0,0);// = plane.at(x,y) - plane.at(x-1,y-1);
            QColor color = Qt::white;
            for (int l = 0; l < objects.size(); l++)
            {
                if (objects.at(l).getPosition() == point) continue;
                long double r_value = distance(objects.at(l).getPosition() - point);
                field += -0.025L*objects.at(l).getMass()/(r_value*r_value*r_value)*(objects.at(l).getPosition() - point);
            }
            if (options->transparentArrows())
            {
                int alpha = qMin(qMax(int(5*191*(1-(logl(distance(field)) + 9))/-8.0L), 0), 255);
                alpha *= options->arrowTransparency()/75.0;
                if (alpha < 10) continue; // don't bother drawing something that is transparent anyway
                if (alpha >= 255) alpha = 255;
                color.setAlpha(alpha);
                field = normalize(field, 10);
            }
            else
            {
                field *= 20000;
                long double length = distance(field);
                if (length < 1) continue; // don't bother drawing something that won't show up anyway
                if (length > options->arrowLength())
                    field = normalize(field, options->arrowLength());
            }
            gravPoints.append(point);
            gravLength.append(field);
            gravColor.append(color);
        }
}

void Planets::forward()
{
    Point3D * nextPositions = new Point3D[objects.size()];
    int arrayWidth = objects.size() - 1;
/*  QVector<QVector<double> > forceArray;
    for (int i = 0; i < arrayWidth; i++)
    {
        forceArray.append(QVector<double> );
        for (int j = 0; j < arrayWidth; j++)
            forceArray[i].append(0);
    }*/
    long double ** forceArray = new long double*[arrayWidth];
    for (int i = 0; i < arrayWidth; i++)
        forceArray[i] = new long double[arrayWidth];

    for (int i = 0; i < objects.size() - 1; i++)
        for (int j = 0; j < objects.size() - 1; j++)
            forceArray[i][j] = 0.0L;
    long double totalTime = 0.0L;
    const Point3D systemVelocityAndDT = (! options->systemCentered())*options->systemVelocity() * dt;
    while (totalTime <= static_cast<long double>(0.005 * ui->speedDoubleSpinBox->value()))
    {
        totalTime += dt;
        if (objects.size() > 1)
        {
            for (int i = 0; i < objects.size(); i++)
            {
                if (objects.at(i).getMass() == 0.0L) continue;
                for (int j = 0; j < objects.size(); j++)
                {
                    if (j == i) continue;
                    long double radius = distance(objects.at(i).getPosition() - objects.at(j%objects.size()).getPosition());
                    if (radius < qMax(objects.at(i).getRadius(), objects.at(j%objects.size()).getRadius()))
                    {
                        planetsCollide(i, j);
                        radius = 100000;
                        i = 1000;
                        j = 1000;
                        continue;
                    }
                }
                if (i > objects.size())           // this makes sure that the for loop exits after
                        //   planetsCollide is called
                {
                    totalTime = 0.015L * static_cast<long double>(ui->speedDoubleSpinBox->value()) + 0.00000000001L;
                    i = objects.size();
                    continue;
                }

                Point3D accelCurrent;
                for (int k = 0; k < objects.size(); k++)
                {
                    if (k == i || objects.at(k).getMass() == 0.0L) continue;
                    accelCurrent += calculateGravForce(&objects.at(i), &objects.at(k))/objects.at(i).getMass()
                                    * normalize(objects.at(i).getPosition() - objects.at(k).getPosition());
                }

                nextPositions[i] =
                        (objects.at(i).getPosition() + objects.at(i).getVelocity()*dt+0.5L*accelCurrent*dt*dt) +
                        systemVelocityAndDT;
                objects[i].setVelocity(objects.at(i).getVelocity() + accelCurrent);
                objects[i].setAccel(accelCurrent);
            }
            if (totalTime <= static_cast<long double>(0.015 * ui->speedDoubleSpinBox->value()))
                for (int i = 0; i < objects.size(); i++)
                {
                    if (objects.at(i).getMass() == 0.0L) continue;
                        objects[i].setPosition(nextPositions[i]);
                }
        }
        else  // if there is only one object, things are fairly simple
        {
            objects[0].setPosition(objects.at(0).getPosition() + objects.at(0).getVelocity()*dt);
//          double time = dt;//totalTime + systemTime;
//          objects[0].setAccel(Point3D(0,10));
//          objects[0].setVelocity(objects.at(0).getVelocity() + objects.at(0).getAccel()*time);
//          objects[0].setPosition(objects.at(0).getPosition() + objects.at(0).getVelocity()*time +
//                                 objects.at(0).getAccel()*time*time/2.0);
//          if (objects[0].getVelocity().y() > 100.0) timer->stop();
        }
//          a(t) = a_int
//          v(t) = v_int + a_int*t
//          x(t) = x_int+v(t)*t+a(t)/2*t^2
    }
    if (options->displayTrace())
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i].addToTrace(objects.at(i).getPosition());
            while (objects[i].getTrace().size() > options->traceNumber())
                objects[i].eraseFirstTrace();
        }
        for (int i = 0; i < trails.size(); i++)
            trails[i].setNumberOfOldPoints(trails.at(i).getOldNumber() + 1);
        for (int i = 0; i < trails.size(); i++)
            if (trails.at(i).size() == 0)
                trails.removeAt(i--);
        for (int i = 0; i < trails.size(); i++)
            while ((trails.at(i).size() + trails.at(i).getOldNumber()) > options->traceNumber())
                trails[i].removeLastPoint();
    }

//  for (int i = 0; i < objects.size(); i++)
//	if (distance(objects.at(i).getPosition() - centerOfMass) >= 3000)
//          removeObject(i--, true);
//  if (ui->showPlanetStatus->isVisible()) loadPlanet();
    systemTime += totalTime;
//  if (systemTime >= 100 || objects.size() == 1) timer->stop();
    delete[] nextPositions;
    update();
}

Point3D Planets::calculateCenterOfMass()
{
        Point3D massPosition = Point3D(0, 0, 0);
    long double totalMass = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		massPosition += objects.at(i).getMass()*objects.at(i).getPosition();
		totalMass += objects.at(i).getMass();
	}
	return centerOfMass = massPosition/totalMass;
}

void Planets::status()
{
	timer->stop();
	QString subMessage;
	QString message = "The following is a report on the planets:";
	for (int i = 0; i < objects.size(); i++)
	{
		message += "\n" + objects.at(i).getName();
        subMessage += toStringO(objects.at(i), true) + "\n";
	}
	ui->textBrowser->setText(data);
        debug(message, subMessage);
}

void Planets::update()
{
    viewerAngle = Point3D(static_cast<long double>(ui->viewerAngleXSpinBox->value()),
                          static_cast<long double>(ui->viewerAngleYSpinBox->value()),
                          static_cast<long double>(ui->viewerAngleZSpinBox->value()));
    viewerPosition = Point3D(static_cast<long double>(ui->viewerPositionXSpinBox->value()),
                             static_cast<long double>(ui->viewerPositionYSpinBox->value()),
                             static_cast<long double>(ui->viewerPositionZSpinBox->value()));
    viewerPlane = Plane(Point3D(0,0,0), Point3D(1,0,0), Point3D(0,1,0));
    viewerPlane.setBasisLength(1);
    setText();
    calculateCenterOfMass();

    // if they are going to be used, calculate the arrow lengths and positions, and send them to the painter function
    if (options->displayGravField() && !(options->deflection()))
    {
        calculateGravityForce(calculateGravityPlane());
        ui->mainFrame->setGravPoints(gravPoints);
        ui->mainFrame->setGravLengths(gravLength);
        ui->mainFrame->setGravColors(gravColor);
    }
    repaint();
}

void Planets::reset()
{
    zoomPoint = QPoint(0,0);                    // the point that the mouse wheel will zoom into
    paintNumber = 0;                            // how many times the main screen has been repainted
    systemTime = 0;                             // how long the system has been running
    ui->speedDoubleSpinBox->setValue(1.0);      // the percentage time modifier, to make time go fast or slow
    options->setTrace(true);                    // whether or not to draw the previous positions of the planets
    options->setTraceNumber(1000);              // the number of previous positions to record
    options->setSystemCentered(true);           // whether or not the screen stays centered on the system
    systemMass = 100;                           // this is all of the mass in the entire system
    ui->distanceScalarSpin->setValue(1);        // this is the scalar that decides how far out the view is

    centerOfMass = Point3D(0, 0, 0);            // the current center of mass of the system
    objects.clear();                            // the list of the orbiting objects
    trails.clear();                             // the list of the trails of all of the orbiting objects
    systemMomentum = Point3D(0, 0, 0);
    systemMass = 1;                             // systemMass needs to be nonzero so systemVelocity can be calculated
    ui->speedDoubleSpinBox->setValue(1);
    int numberOfComets = qrand()%10 + 1;
    timer->stop();
    objects.clear();
    removedObjects.setPosition(Point3D(0, 0, 0));
    removedObjects.setMass(0);
    removedObjects.setVelocity(Point3D(0, 0, 0));
    Point3D totalMomentum;

    QString scenario = ui->comboBox->currentText();
    long double totalMass = 0;
    if (scenario == "Earth and Sun")
    {
        objects.append(Object(Point3D(0, 0, 0), Point3D(0, 0, 0), 200, 20, "Sun", Qt::yellow));
//        objects.append(Object(Point3D(150, 0, 0), Point3D(0, -175, 0), 0.1, 1, "Earth", Qt::blue));
        objects.append(Object(Point3D(300, 0, 0), Point3D(0, -50, 0), 0.1L, 1.0L, "Earth", Qt::blue));
        options->setSystemVelocity(Point3D(0, 0, 0));
    }
//    else if (scenario == "Lagrangian Points")
//    {
//        objects.append(Object(Point3D(  0,   0, 0), Point3D(0,    0, 0), 200, 20,   "Sun", Qt::yellow));
//        objects.append(Object(Point3D(150,   0, 0), Point3D(0, -175, 0), 0.1,  1, "Earth", Qt::blue));
//        objects.append(Object(Point3D( 50, -60, 0), Point3D(0,    0, 0),   0,  1,    "L5", Qt::red));
//    }
    else if (scenario == "Binary System")
    {
        // for dt = 1/900000, v = 75
        // for dt = 1/9000000, v = 237
        // for dt = 1/90000000, v = 237*237/75 = 750
        // for dt = 1/900000000, v = 237*237/75 = 2370
        objects.append(Object(Point3D(-100, 0, 0), Point3D(0, -75, 25), 100, 20, "Sun 1", Qt::yellow));
        objects.append(Object(Point3D(100, 0, 0), Point3D(0, 75, 25), 100, 20, "Sun 2", Qt::red));
        options->setSystemVelocity(Point3D(25, 25, 0));
    }
    else if (scenario == "Sun and Comets")
    {// One to ten comets and sun
        objects.append(Object(Point3D(0, 0, 0), Point3D(0, 0, 0), 200, 20, "Sun", Qt::yellow));
        for (int i = 0; i < numberOfComets; i++)
            objects.append(Object(Point3D(getRandomPoint(50, 250)),
                                  Point3D(getRand(125.0L, 60.0L, true), getRand(125.0L, 60.0L, true), getRand(125.0L, 60.0L, true)), 0.1L,
                                  0.1L, "Comet " + toStringI(i + 1), Qt::cyan));
            options->setSystemVelocity(Point3D(0, 0, 0));
    }
    else if (scenario == "4 Star Ballet")
    {
        objects.append(Object(Point3D(-150, 0, 100), Point3D(  0, -50, 0), 100, 20, "", Qt::cyan));
        objects.append(Object(Point3D( 150, 0, 100), Point3D(  0,  50, 0), 100, 20, "", Qt::magenta));
        objects.append(Object(Point3D(0,  150, -100), Point3D(-50,   0, 0), 100, 20, "", Qt::cyan));
        objects.append(Object(Point3D(0, -150, -100), Point3D( 50,   0, 0), 100, 20, "", Qt::magenta));
        for (int i = 0; i < objects.size(); i++)
            objects[i].setName("Planet " + QString::number(i + 1));
        options->setSystemVelocity(Point3D(25, 25, 25));
    }
    else if (scenario == "Double Double")
    {
        objects.append(Object(Point3D(0,  275, -25), Point3D(65,   150, 0), 100, 20, "", Qt::red));
        objects.append(Object(Point3D(0,  275,  25), Point3D(65,  -150, 0), 100, 20, "", Qt::blue));
        objects.append(Object(Point3D(0, -275, -25), Point3D(-65,  150, 0), 100, 20, "", Qt::cyan));
        objects.append(Object(Point3D(0, -275,  25), Point3D(-65, -150, 0), 100, 20, "", Qt::magenta));
        for (int i = 0; i < objects.size(); i++)
            objects[i].setName("Planet " + QString::number(i + 1));
        options->setSystemVelocity(Point3D(0, 0, 0));
    }
    else if (scenario == "Solar System")
    {
        setupSolarSystem();
        ui->speedDoubleSpinBox->setValue(0.25);
        ui->distanceScalarSpin->setValue(1000000);
        options->setSystemVelocity(Point3D(0, 0, 0));
    }
    else if (scenario == "Solar System Formation")
    {
    for (int i = 0; i < ui->numberOfPlanetsSpinBox->value(); i++)
        objects.append(Object(getRandomPoint(0, ui->averagePositionSpinBox->value()),
                              getRandomPoint(0, ui->averageSpeedSpinBox->value()),
                              std::abs(generateNormalNumbers(static_cast<long double>(ui->averageMassSpinBox->value()), static_cast<long double>(ui->averageMassSpinBox->value())/2.0L)),
                              std::abs(generateNormalNumbers(7.5L, 2)), toStringI(i + 1), Qt::yellow));
        // the following code makes the system centered and insures that the whole system does not go flying off
        for (int i = 0; i < objects.size(); i++)
        {
                totalMomentum += objects.at(i).getMomentum();
                totalMass += objects.at(i).getMass();
        }
        for (int i = 0; i < objects.size(); i++)
                objects[i].setVelocity(objects[i].getVelocity() - totalMomentum/totalMass);
        options->setSystemVelocity(totalMomentum/totalMass);
    }
    else if (scenario == "Double Slingshot")
    {
        objects.append(Object(Point3D(   0.0L,    0.0L, 0.0L), Point3D(    0.0L,    0.0L, 0.0L), 1000.0L, 30.0L, "Sun", Qt::yellow));
        objects.append(Object(Point3D(-150.0L,    0.0L, 0.0L), Point3D(    0.0L, -400.0L, 0.0L),   20.0L, 10.0L, "Planet 1", Qt::blue));
        objects.append(Object(Point3D(   0.0L, -350.0L, 0.0L), Point3D(  250.0L,    0.0L, 0.0L),   20.0L, 10.0L, "Planet 2", Qt::green));
        objects.append(Object(Point3D(  10.0L,  38.72L, 0.0L), Point3D( -919.8L,  237.5L, 0.0L),    0.1L,  5.0L, "Commet", Qt::gray));
        options->setSystemVelocity(Point3D(0, 0, 0));
    }
    else // the free form option
    {
        objects.append(Object(Point3D(   0,  200, 0), Point3D(-50,   0, 0), 100, 20, "", Qt::red));
        objects.append(Object(Point3D( 200,    0, 0), Point3D(  0,  50, 0), 100, 20, "", Qt::blue));
        objects.append(Object(Point3D(   0, -200, 0), Point3D( 50,   0, 0), 100, 20, "", Qt::cyan));
        objects.append(Object(Point3D(-200,    0, 0), Point3D(  0, -50, 0), 100, 20, "", Qt::magenta));
        for (int i = 0; i < objects.size(); i++)
            objects[i].setName("Planet " + QString::number(i + 1));
        options->setSystemVelocity(Point3D(25, 25, 25));
    }
    calculateSystemProperties();

    if (scenario == "Solar System")
        // this is to ensure that the UI doesn't try to calculate the grav field when the user can't see the result
    {
        options->setDisplayGravField(false);
        options->setEnableGravField(false);
        ui->distanceScalarSpin->setMinimum(200000);
        ui->distanceScalarSpin->setMaximum(15000000);
        ui->distanceScalarSpin->setSingleStep(100000);
        ui->distanceScalarSpin->setValue(200000);
    }
    else
    {
        options->setEnableGravField(true);
        ui->distanceScalarSpin->setMinimum(0.25);
        ui->distanceScalarSpin->setMaximum(100);
        ui->distanceScalarSpin->setSingleStep(0.25);
        ui->distanceScalarSpin->setValue(1);
    }

    if (scenario == "Free Form")
        ui->freeFormBox->show();
    else
        ui->freeFormBox->hide();

    if (scenario == "Solar System Formation")
        ui->SolarSystemFormationGroupBox->show();
    else
        ui->SolarSystemFormationGroupBox->hide();

    ui->planetComboBox->clear();
    for (int i = 0; i < objects.size(); i++)
    {
        // if the system is centered, move everything so that the center of mass is at the origin
        if (options->systemCentered())
            objects[i].setPosition(objects.at(i).getPosition() - centerOfMass);
        // if operating in 2D, make sure that everything is in the XY plane
        if (options->newtonian2D() || options->deflection())
        {
            objects[i].setPosition(Point3D(objects[i].getPosition().x(), objects[i].getPosition().y(), 0));
            objects[i].setVelocity(Point3D(objects[i].getVelocity().x(), objects[i].getVelocity().y(), 0));
        }

        // add everything to the free form combo box
        ui->planetComboBox->addItem(objects.at(i).getName());
    }

    // if there is a single body in the system with mass more than one million, use the realistic model
    long double maxMass = 0;
    for (int i = 0; i < objects.size(); i++)
        if (objects.at(i).getMass() > maxMass)
            maxMass = objects.at(i).getMass();
    if (maxMass > 1000000)
        G = G_real;
    else
        G = G_fake;

    // this section initializes all of the data storage and capture.
    // it adds all of the headings, so there is more than just a ton of numbers
    data = "Time,";
    for (int i = 0; i < objects.size(); i++)
        for (int j = 0; j < 13; j++)
            data += objects.at(i).getName() + ",";
    data += "\n,";
    for (int i = 0; i < objects.size(); i++)
        data += "Name,Mass,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z,Speed,|Momentum|,Accel X,Accel Y,Accel Z,";
    data += "\n";

    update();
}

QString Planets::toStringP(Point3D point, bool CSVFormat) const
{
	if (CSVFormat)
            return toStringL(point.x()) + "," + toStringL(point.y()) + "," + toStringL(point.z()) + ",";
	if (options->usePolar())
        {
            double r = double(distance(point));
            return "r = " + toStringD(r) + ", " +
                    QChar(0x03C1) + " = " + toStringD(atan2(double(point.z()), r) * 180/3.14159) + ", " +
                    QChar(952) + " = " + toStringD(atan2(double(point.y()), double(point.x())));
        }
        return "x = " + toStringL(point.x()) + "\n\ty = " + toStringL(point.y()) + "\n\tz = " + toStringL(point.z());
}

void Planets::planet()
{
    if (!loadingPlanets)
    {
        objects.replace(ui->planetComboBox->currentIndex(),
                        Object(Point3D(static_cast<long double>(ui->positionX->value()), static_cast<long double>(ui->positionY->value()), static_cast<long double>(ui->positionZ->value())),
                               Point3D(static_cast<long double>(ui->velocityX->value()), static_cast<long double>(ui->velocityY->value()), static_cast<long double>(ui->velocityZ->value())),
                               static_cast<long double>(ui->massSpin->value()), 20, ui->nameLine->text(),
                               objects.at(ui->planetComboBox->currentIndex()).getColor()));
        update();
    }
}

void Planets::loadPlanet()
{
	int number = ui->planetComboBox->currentIndex();
	if (number >= 0 && number < objects.size())
	{
		loadingPlanets = true;
        ui->positionX->setValue(double(objects.at(number).getPosition().x()));
                ui->positionY->setValue(double(objects.at(number).getPosition().y()));
                ui->positionZ->setValue(double(objects.at(number).getPosition().z()));
                ui->velocityX->setValue(double(objects.at(number).getVelocity().x()));
                ui->velocityY->setValue(double(objects.at(number).getVelocity().y()));
                ui->velocityZ->setValue(double(objects.at(number).getVelocity().z()));
                ui->massSpin->setValue(double(objects.at(number).getMass()));
		ui->nameLine->setText(objects.at(number).getName());
		loadingPlanets = false;
	}
}

void Planets::updateTrace(bool keepTrace)
{
    if (! keepTrace)
    {
        for (int i = 0; i < trails.size(); i++)
            trails[i].clearPoints();
        for (int i = 0; i < objects.size(); i++)
            objects[i].clearTrace();
    }
}

void Planets::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mouseIsPanning = true;
    else if (event->button() == Qt::LeftButton)
        mouseIsRotating = true;
    lastPos = event->pos();
}

void Planets::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseIsPanning && options->enableMousePan())
    {
        const QPoint move = lastPos - event->pos();
        if (distance(Point3D(move.x(), move.y(), 0)) < 2)
            return;
        ui->viewerPositionXSpinBox->setValue(ui->viewerPositionXSpinBox->value() + move.x()/(2*options->getPanSensitivity()));
        ui->viewerPositionYSpinBox->setValue(ui->viewerPositionYSpinBox->value() + move.y()/(2*options->getPanSensitivity()));
    }
    if (mouseIsRotating && options->enableMouseRotate())
    {
        const Point3D move = viewerPlane.at(event->pos() - lastPos);
        ui->viewerAngleXSpinBox->setValue(int(ui->viewerAngleXSpinBox->value() + double(move.x())/(2*options->getRotateSensitivity())));
        ui->viewerAngleYSpinBox->setValue(int(ui->viewerAngleYSpinBox->value() + double(move.y())/(2*options->getRotateSensitivity())));
    }
    lastPos = event->pos();
}

void Planets::mouseReleaseEvent(QMouseEvent *event)
{
    mouseIsRotating = false;
    mouseIsPanning = false;
}

void Planets::wheelEvent(QWheelEvent * event)
{
    double step = 1.0;
    if (ui->distanceScalarSpin->value() < 5) step = 0.25;
    else if (ui->distanceScalarSpin->value() < 50) step = 2.5;
    else if (ui->distanceScalarSpin->value() < 500) step = 25;
    else if (ui->distanceScalarSpin->value() < 5000) step = 250;
    else if (ui->distanceScalarSpin->value() < 50000) step = 2500;

    ui->distanceScalarSpin->setValue(
                ui->distanceScalarSpin->value() + step * event->delta()/abs(event->delta()));
    zoomPoint = event->pos() - QPoint(width()/2, height()/2);
}
