#include "Planets.h"
#include "ui_Planets.h"
#include "Point3D.h"
#include <plane.h>
#include <QMouseEvent>
#include <toString.h>
#include <QRandomGenerator>
#include <QDebug>
#include <QFileDialog>
#include <QDirIterator>
#include <unit.h>

Point3D::dataType Planets::w_0 = -powl(2.0 , 1/3.0)/(2.0 - powl(2, 1.0/3.0));
Point3D::dataType Planets::w_1 = 1 / (2.0 - powl(2.0 , 1/3.0));
Point3D::dataType Planets::yoshidaC1 = Planets::w_1/2.0;
Point3D::dataType Planets::yoshidaC4 = Planets::yoshidaC1;
Point3D::dataType Planets::yoshidaC2 = (Planets::w_0 + Planets::w_1)/2.0;
Point3D::dataType Planets::yoshidaC3 = Planets::yoshidaC2;
Point3D::dataType Planets::yoshidaD1 = Planets::w_1;
Point3D::dataType Planets::yoshidaD3 = Planets::yoshidaD1;
Point3D::dataType Planets::yoshidaD2 = Planets::w_0;
QString Planets::defaultIntegrationType = "Hermite";

Point3D::dataType generateNormalNumbers(Point3D::dataType mu = 0.0L, Point3D::dataType sigma = 1.0L)
{
	static bool deviateAvailable=false;        //        flag
    static Point3D::dataType storedDeviate;          //        deviate from previous calculation
    Point3D::dataType polar, rsquared, var1, var2;

	//        If no deviate has been stored, the polar Box-Muller transformation is
	//        performed, producing two independent normally-distributed random
	//        deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable)
	{
		//        choose pairs of uniformly distributed deviates, discarding those
		//        that don't fall within the unit circle
		do
		{
            var1 = 2.0L*( static_cast<Point3D::dataType>(rand())/static_cast<Point3D::dataType>(RAND_MAX) ) - 1.0L;
            var2 = 2.0L*( static_cast<Point3D::dataType>(rand())/static_cast<Point3D::dataType>(RAND_MAX) ) - 1.0L;
			rsquared=var1*var1+var2*var2;
        } while ( rsquared >= 1.0L || rsquared == 0.0L);

        //        calculate polar transformation for each deviate
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
inline T weightedAverage(T x, T y, Point3D::dataType xD, Point3D::dataType yD)
{
    return (x*xD + y*yD)/(xD+yD);
}

QColor weightedAverageColor(QColor color1, QColor color2, Point3D::dataType mass1, Point3D::dataType mass2)
{
	int r1, r2, g1, g2, b1, b2, a1, a2;
	color1.getRgb(&r1, &g1, &b1, &a1);
	color2.getRgb(&r2, &g2, &b2, &a2);
    return QColor(weightedAverage(r1, r2, mass1, mass2), weightedAverage(g1, g2, mass1, mass2),
                  weightedAverage(b1, b2, mass1, mass2), weightedAverage(a1, a2, mass1, mass2));
}

Point3D::dataType getRand(Point3D::dataType min, Point3D::dataType max, bool canBeNegative)
{
	if (min < 0) min = 0;
    Point3D::dataType range = (max - min) / RAND_MAX;
    Point3D::dataType random = min + rand()*range;
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
    timer->setInterval(1);
    box = new QMessageBox(this);                // for debug and messages
    options = new Options();              // the options dialog
    scenarioEditor = new ScenarioDialog();
    mouseIsRotating = false;
    mouseIsPanning = false;
    lastPos = QPoint(0,0);
    integrationType = defaultIntegrationType;

    filename = "C:\\Users\\McRed\\Desktop\\Motion Integration.csv";//QFileDialog::getSaveFileName(this, "Save File Name", "Motion Integration.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0); // getting the filename (full path)
    file.setFileName(filename);

    connectEverything();

    ui->viewGroupBox->setStyleSheet("color: green");
    resize(1000, 800);

    loadScenarios();

    ui->comboBox->clear();
    for (int i = 0; i < scenarios.length(); i++) {
        ui->comboBox->addItem(scenarios.at(i).name);
    }
    ui->comboBox->setCurrentIndex(0);
    calcAccels(0, true);
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

void Planets::connectEverything()
{
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
    connect(ui->scenarioEditorPushButton, SIGNAL(clicked()), scenarioEditor, SLOT(show()));

    connect(options, SIGNAL(somethingChanged()), this, SLOT(update()));
    connect(options, SIGNAL(traceChanged(bool)), this, SLOT(updateTrace(bool)));

    connect(ui->viewComboBox, SIGNAL(activated(QString)), this, SLOT(changeView(QString)));

    connect(ui->integrationComboBox, SIGNAL(activated(QString)), this, SLOT(changeIntegrationType(QString)));
}

void Planets::loadScenarios() {
    scenario newScenario;

    newScenario = defaultScenario();
    newScenario.name = "Sun and Comets";
    newScenario.planets.append(Object(Point3D(0, 0, 0), Point3D(0, 0, 0), 200, 20, "Sun", Qt::yellow));
    int numberOfComets = QRandomGenerator().generate()%10 + 1;
    for (int i = 0; i < numberOfComets; i++) {
        newScenario.planets.append(Object(Point3D(getRandomPoint(50, 250)),
                              Point3D(getRand(125.0L, 60.0L, true), getRand(125.0L, 60.0L, true), getRand(125.0L, 60.0L, true)), 0.1L,
                              0.1L, "Comet " + toStringI(i + 1), Qt::cyan));
    }
    newScenario.bigG = G_real_AU_day_M_E;
    newScenario.dt = powl(2, -7);
    newScenario.iterationsPerDataPoint = 512;
    newScenario.endTime = 365.25;
    newScenario.lengthUnit = "AU";
    newScenario.massUnit = "M_E";
    newScenario.timeUnit = "day";
    scenarios.append(newScenario);

    newScenario = defaultScenario();
    newScenario.name = "Solar System";
    Point3D::dataType Sun[4];
    Point3D::dataType Mercury[4], Venus[4], Earth[4], Mars[4];
    Point3D::dataType Jupiter[4], Saturn[4], Uranus[4];
    Point3D::dataType Neptune[4], Pluto[4];

//  planet[0] = Aphelion, planet[1] = Min orbital Velocity, planet[2] = Mass, planet[3] = Radius
    Sun[0] = 0; 	Sun[1] = 0; 	Sun[2] = 1.989E+30; 	Sun[3] = 695700;
    Mercury[0] = 69816296; 	Mercury[1] = 38.8594842887767; 	Mercury[2] = 3.302E+23; 	Mercury[3] = 2439.7;
    Venus[0] = 108935007; 	Venus[1] = 34.7866255181453; 	Venus[2] = 4.8685E+24; 	Venus[3] = 6051.8;
    Earth[0] = 152098320; 	Earth[1] = 29.2911638439679; 	Earth[2] = 5.9742E+24; 	Earth[3] = 6371;
    Mars[0] = 249230520; 	Mars[1] = 21.9704579543567; 	Mars[2] = 6.4185E+23; 	Mars[3] = 3397;
    Jupiter[0] = 816642073; 	Jupiter[1] = 12.4323444026568; 	Jupiter[2] = 1.8986E+27; 	Jupiter[3] = 71492;
    Saturn[0] = 1514524445; 	Saturn[1] = 9.09261129571815; 	Saturn[2] = 5.6846E+26; 	Saturn[3] = 60268;
    Uranus[0] = 3003731422; 	Uranus[1] = 6.49333381639041; 	Uranus[2] = 8.6832E+25; 	Uranus[3] = 25559;
    Neptune[0] = 4545854178; 	Neptune[1] = 5.37254836743227; 	Neptune[2] = 1.0243E+26; 	Neptune[3] = 24764;
    Pluto[0] = 7304204904; 	Pluto[1] = 3.70523124577267; 	Pluto[2] = 1.25E+22; 	Pluto[3] = 1195;

    newScenario.planets.append(Object(Point3D(Sun[0],     0), Point3D(0, Sun[0]),     Sun[2],     Sun[3],     "Sun",       Qt::yellow));
    newScenario.planets.append(Object(Point3D(Mercury[0], 0), Point3D(0, Mercury[1]), Mercury[2], Mercury[3], "Mercury",   Qt::gray));
    newScenario.planets.append(Object(Point3D(Venus[0],   0), Point3D(0, Venus[1]),   Venus[2],   Venus[3],   "Venus",     Qt::yellow));
    newScenario.planets.append(Object(Point3D(Earth[0],   0), Point3D(0, Earth[1]),   Earth[2],   Earth[3],   "Earth",     Qt::cyan));
    newScenario.planets.append(Object(Point3D(Mars[0],    0), Point3D(0, Mars[1]),    Mars[2],    Mars[3],    "Mars",      Qt::red));
    newScenario.planets.append(Object(Point3D(Jupiter[0], 0), Point3D(0, Jupiter[1]), Jupiter[2], Jupiter[3], "Jupiter",   Qt::red));
    newScenario.planets.append(Object(Point3D(Saturn[0],  0), Point3D(0, Saturn[1]),  Saturn[2],  Saturn[3],  "Saturn",    Qt::magenta));
    newScenario.planets.append(Object(Point3D(Uranus[0],  0), Point3D(0, Uranus[1]),  Uranus[2],  Uranus[3],  "Uranus",    Qt::magenta));
    newScenario.planets.append(Object(Point3D(Neptune[0], 0), Point3D(0, Neptune[1]), Saturn[2],  Saturn[3],  "Neptune",   Qt::magenta));
    newScenario.planets.append(Object(Point3D(Pluto[0],   0), Point3D(0, Pluto[1]),   Pluto[2],   Pluto[3],   "Pluto",     Qt::magenta));

    newScenario.bigG = G_real_kmks;
    newScenario.dt = 300;
    newScenario.iterationsPerDataPoint = 512;
    newScenario.endTime = 30780000*1.1;
    newScenario.lengthUnit = "km";
    newScenario.massUnit = "kg";
    newScenario.timeUnit = "s";
    scenarios.append(newScenario);

    newScenario = defaultScenario();
    newScenario.name = "Solar System Formation";
    for (int i = 0; i < ui->numberOfPlanetsSpinBox->value(); i++) {
        newScenario.planets.append(Object(getRandomPoint(0, ui->averagePositionSpinBox->value()),
                              getRandomPoint(0, ui->averageSpeedSpinBox->value()),
                              std::abs(generateNormalNumbers(static_cast<Point3D::dataType>(ui->averageMassSpinBox->value()), static_cast<Point3D::dataType>(ui->averageMassSpinBox->value())/2.0L)),
                              std::abs(generateNormalNumbers(7.5L, 2)), toStringI(i + 1), Qt::yellow));
    }
    newScenario.bigG = G_real_AU_day_M_E;
    newScenario.dt = powl(2, -7);
    newScenario.iterationsPerDataPoint = 512;
    newScenario.endTime = 365.25;
    newScenario.lengthUnit = "AU";
    newScenario.massUnit = "M_E";
    newScenario.timeUnit = "day";
    scenarios.append(newScenario);

    // Load the scenario files in the iCloud directory
    QFileInfoList hitList; // Container for matches
    QString directory = "C:\\Users\\McRed\\iCloudDrive\\QT Programming\\Planets\\Scenarios";
    QDirIterator it(directory, QDirIterator::Subdirectories);

    // Iterate through the directory using the QDirIterator
    while (it.hasNext()) {
        QString filename = it.next();
        QFileInfo file(filename);

        // Check if it's a dir
        if (file.isDir()) continue;

        // If the filename contains target string - put it in the hitlist
        if (file.fileName().endsWith(".scen", Qt::CaseSensitive)) {
            hitList.append(file);
        }
    }

    foreach (QFileInfo hit, hitList) {
        readScenariosV1(&scenarios, hit.absoluteFilePath());
    }
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
    delete scenarioEditor;
	delete options;
    delete ui;
}

void Planets::close()
{
	QMainWindow::close();
	options->close();
    scenarioEditor->close();
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
    painter.drawText(100,   0, 1000, 1000, 0, "B1 = " + toStringP(viewerPlane.getBasis1(),true));
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

void Planets::planetsCollide()
{
    int planet1 = -1, planet2 = -1;
    for (int i = 0; i < objects.length(); i++) {
        for (int j = i + 1; j < objects.length(); j++) {
            if ((objects[i].getPosition(), objects[j].getPosition()).length() < qMax(objects.at(i).getRadius(), objects.at(j).getRadius())) {
                planet1 = i;
                planet2 = j;
            }
        }
    }
    if (planet1 != -1) {
        QString p1Name = objects.at(planet1).getName(), p2Name = objects.at(planet2).getName();
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
        Point3D::dataType mass = star1.getMass() + star2.getMass();
        Point3D::dataType radius = weightedAverage(star1.getRadius(), star2.getRadius(), star1.getMass(), star2.getMass());
        QString name;
        if (star1.getMass() >= star2.getMass() * 9) name = star1.getName();
        else if (star2.getMass() >= star1.getMass() * 9) name = star2.getName();
        else name = star1.getName() + " + " + star2.getName();
        QColor color = weightedAverageColor(
                           star1.getColor(), star2.getColor(),
                           star1.getMass(), star2.getMass());

        trails.append(tracePoints(star1.getTrace(), star1.getTrace().size(), star1.getColor()));
        trails.append(tracePoints(star2.getTrace(), star2.getTrace().size(), star2.getColor()));

        objects.insert(qMin(planet1, planet2), Object(position, momentum/mass, mass, radius, name, color));
        qDebug() << p1Name + " and " + p2Name + " have collided!";
        // recursively call planetsCollide until all collisions have been resolved
        if (planet1 < objects.length() - 1) {qDebug() << "recursively calling planetsCollide";}
    }
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
                toStringP(planet.getVelocity(), true) +
                //Accel X, Accel Y, Accel Z
                toStringP(planet.getAccel(), true) +
                //Jerk X, Jerk Y, Jerk Z
                toStringP(planet.getJerk(), true);
	}
	return message;
}

void Planets::calculateSystemProperties()
{
	calculateSystemMass();
	calculateSystemMomentum();
	calculateCenterOfMass();
}

Point3D::dataType Planets::calculateEscapeVelocity(Object *planet)
{
    if (planet == nullptr) return 0.0L;
    Point3D::dataType sum = 0.0L;
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

Point3D::dataType Planets::calculateSystemMass()
{
	systemMass = 0;
	for (int i = 0; i < objects.size(); i++)
		systemMass += objects.at(i).getMass();
    return systemMass;
}

Point3D Planets::calculateSystemMomentum()
{
    systemMomentum = Point3D(0, 0, 0);
	for (int i = 0; i < objects.size(); i++)
		systemMomentum += objects.at(i).getMomentum();
    return systemMomentum;
}

Point3D::dataType Planets::calculateSystemEnergy() {
    return calculateSystemPotentialEnergy() + calculateSystemKineticEnergy();
}

Point3D::dataType Planets::calculateSystemPotentialEnergy() {
    Point3D::dataType systemEnergy = 0.0;
    for (int i = 0; i < objects.length(); i++) {
        for (int j = 0; j < objects.length(); j++) {
            // potential energy is -G * m1 * m2 / R
            if (i != j) systemEnergy -= G * objects.at(i).getMass() * objects.at(j).getMass()/(objects.at(i).getPosition() - objects.at(j).getPosition()).length();
        }
    }
    return systemEnergy;
}

Point3D::dataType Planets::calculateSystemKineticEnergy() {
    Point3D::dataType systemEnergy = 0.0;
    for (int i = 0; i < objects.length(); i++) {
        systemEnergy += objects.at(i).getKineticEnergy();
    }
    return systemEnergy;
}

Point3D Planets::calculateCenterOfMass()
{
    Point3D massPosition = Point3D(0, 0, 0);
    Point3D::dataType totalMass = 0;
    for (int i = 0; i < objects.size(); i++) {
        massPosition += objects.at(i).getMass()*objects.at(i).getPosition();
        totalMass += objects.at(i).getMass();
    }
    return centerOfMass = massPosition/totalMass;
}

Point3D Planets::getRandomPoint(Point3D::dataType minRadius, Point3D::dataType maxRadius)
{
	if (minRadius > maxRadius)
	{
        Point3D::dataType j = minRadius;
		minRadius = maxRadius;
		maxRadius = j;
	}

    Point3D::dataType radius = getRand(minRadius, maxRadius, false);
    Point3D::dataType x = getRand(0, radius, true);
    Point3D::dataType y = getRand(0, radius, true);
    int isNegative = rand()%10000 - 5000;
	if (isNegative == 0) isNegative++;
    Point3D::dataType z = sqrtl(std::abs(radius*radius - x*x - y*y)) * isNegative/std::abs(isNegative);
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
    if (options->gravPlaneGoesThroughCenterOfMass() && useCenterOfMass) basePlane.setAddPoint(centerOfMass);
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
                Point3D::dataType r_value = distance(objects.at(l).getPosition() - point);
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
                Point3D::dataType length = distance(field);
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
    bool fileJustOpened = false;
    if (!file.isOpen()) {
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            outputToFile.setDevice(&file);
            fileJustOpened = true;
        } else qDebug() << "Output file failed to open!";
    }
    if (fileJustOpened || firstRun) {
        outputToFile <<  "Time,";
        for (int i = 0; i < objects.size(); i++)
            for (int j = 0; j < 14; j++)
                outputToFile << objects.at(i).getName() + ",";
        outputToFile << "System Kinetic Energy,System Potential Energy,System Total Energy,,,Integration Type," << integrationType << "\n" + activeScenario.timeUnit + ",";
        for (int i = 0; i < objects.size(); i++)
            outputToFile << "Name,Mass,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z,Accel X,Accel Y,Accel Z,Jerk X,Jerk Y,Jerk Z,";
        QString energyUnits = activeScenario.massUnit + " " + activeScenario.lengthUnit + "^2/" + activeScenario.timeUnit + "^2";
        outputToFile << energyUnits << "," << energyUnits << "," << energyUnits << ",,,Big G," << double(G) << "\n";

        if (integrationType == "RK4") {
            QVector<Point3D> emptyArray;
            emptyArray.resize(objects.length());

            RK4_k_r.clear();
            while (RK4_k_r.length() != 4) {RK4_k_r.append(emptyArray);}

            RK4_k_v.clear();
            while (RK4_k_v.length() != 4) {RK4_k_v.append(emptyArray);}

            RK4_k_a.clear();
            while (RK4_k_a.length() != 4) {RK4_k_a.append(emptyArray);}
        }

        firstRun = false;
    }
    Point3D::dataType totalTime = 0.0L;
    if (file.isOpen()) {
        outputToFile << toStringL(systemTime + totalTime) + ",";
        for (int i = 0; i < objects.size(); i++) {outputToFile << toStringO(objects.at(i), true, true);}
        outputToFile << double(calculateSystemKineticEnergy()) << "," << double(calculateSystemPotentialEnergy()) << "," << double(calculateSystemEnergy());
        if (linesPrinted == 0) {outputToFile << ",,,,value,log_2 of value";}
        else if (linesPrinted == 1) {outputToFile << ",,,dt," << double(activeScenario.dt) << "," << double(log2(activeScenario.dt));}
        else if (linesPrinted == 2) {outputToFile << ",,,iterations per datapoint," << double(activeScenario.iterationsPerDataPoint) << "," << log2(activeScenario.iterationsPerDataPoint);}
        else if (linesPrinted == 3) {outputToFile << ",,,length unit," << activeScenario.lengthUnit << "," << Unit::getLongName(activeScenario.lengthUnit);}
        else if (linesPrinted == 4) {outputToFile << ",,,time unit," << activeScenario.timeUnit << "," << Unit::getLongName(activeScenario.timeUnit);}
        else if (linesPrinted == 5) {outputToFile << ",,,mass unit," << activeScenario.massUnit << "," << Unit::getLongName(activeScenario.massUnit);}
        outputToFile << "\n";
        linesPrinted += 1;
    }
    for (uint i = 0; i < activeScenario.iterationsPerDataPoint; i++) {
        totalTime += dt;
        if (integrationType == "Hermite") {Hermite();}
        else if (integrationType == "Euler-Cromber") {EulerCromber();}
        else if (integrationType == "Velocity Verlet") {VelocityVerlet();}
        else if (integrationType == "Yoshida") {Yoshida();}
        else if (integrationType == "Euler") {Euler();}
        else if (integrationType == "RK4") {RK4();}
        planetsCollide();
    }
    if (integrationType == "RK4") {calcAccels();}

    try {
        if (options->displayTrace())
        {
/*            for (int i = 0; i < objects.size(); i++)
            {
                objects[i].addToTrace(objects.at(i).getPosition());
                while (objects[i].getTrace().size() > options->traceNumber())
                    objects[i].eraseFirstTrace();
            }*/
            for (int i = 0; i < trails.size(); i++)
                trails[i].setNumberOfOldPoints(trails.at(i).getOldNumber() + 1);
            for (int i = 0; i < trails.size(); i++)
                if (trails.at(i).size() == 0)
                    trails.removeAt(i--);
            for (int i = 0; i < trails.size(); i++)
                while ((trails.at(i).size() + trails.at(i).getOldNumber()) > options->traceNumber())
                    trails[i].removeLastPoint();
        }
    } catch (...) {
        qDebug() << "tracing doesn't work well!";
    }

    systemTime += totalTime;

    // if any of the trigger conditions have hit, stop the simulation and close the file
    if (((activeScenario.endTime != -1) && (systemTime >= activeScenario.endTime)) || (objects.length() == 1)) {
        file.close();
        timer->stop();
    }
    update();
}

void Planets::calcAccels(int arguments, bool calcJerk) {
    Point3D::dataType r2, r3;
    Point3D r_i_k;
    Point3D v_i_k;
    // if arguments is zero, that means use the normal position
    if (arguments == 0) {
        for (int i = 0; i < objects.length(); i++) {
            Point3D accelCurrent = Point3D();
            Point3D jerkCurrent = Point3D();
            for (int k = 0; k < objects.size(); k++)
            {
                if (k == i || (objects.at(k).getMass() == 0.0L)) continue;
                r_i_k = objects.at(i).getPosition() - objects.at(k).getPosition();
                v_i_k = objects.at(i).getVelocity() - objects.at(k).getVelocity();
                r2 = r_i_k.magSqr();
                r3 = r_i_k.mag() * r2;
                accelCurrent += 0 - G * objects.at(k).getMass() / r2 * r_i_k.normal();
                if (calcJerk) {
                    jerkCurrent += 0 - G * objects.at(k).getMass() / r3 * (v_i_k - 3 * dot(r_i_k, v_i_k)*r_i_k / r2);
                }
            }
            objects[i].setAccel(accelCurrent);
            if (calcJerk) objects[i].setJerk(jerkCurrent);
        }
    } else if (arguments == 5) {
        for (int i = 0; i < objects.length(); i++) {
            Point3D accelCurrent;
            Point3D jerkCurrent;
            for (int k = 0; k < objects.size(); k++)
            {
                if (k == i || (objects.at(k).getMass() == 0.0L)) continue;
                r_i_k = objects.at(i).getR_P() - objects.at(k).getR_P();
                v_i_k = objects.at(i).getV_P() - objects.at(k).getV_P();
                r2 = r_i_k.magSqr();
                r3 = r_i_k.mag() * r2;
                accelCurrent += 0 - G * objects.at(k).getMass() / r2 * r_i_k.normal();
                jerkCurrent  += 0 - G * objects.at(k).getMass() / r3 * (v_i_k - 3 * dot(r_i_k, v_i_k)*r_i_k / r2);
            }
            objects[i].setAccel(accelCurrent);
            if (calcJerk) objects[i].setJerk(jerkCurrent);
        }
        // if arguments is non-zero, use the x_i position, where i is the argument
    } else {
        for (int i = 0; i < objects.length(); i++) {
            Point3D accelCurrent;
            for (int k = 0; k < objects.size(); k++)
            {
                if (k == i || (objects.at(k).getMass() == 0.0L)) continue;
                r_i_k = objects.at(i).getX_i(arguments) - objects.at(k).getX_i(arguments);
                accelCurrent += 0 - G * objects.at(k).getMass() / r_i_k.magSqr() * r_i_k.normal();
            }
            objects[i].setAccel(accelCurrent);
        }
    }
}

void Planets::Euler() {
    calcAccels(0,true);
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setPosition((objects.at(i).getPosition() + objects.at(i).getVelocity()*dt));
        objects[i].setVelocity(objects.at(i).getVelocity() + objects[i].getAccel()*dt);
    }
}

void Planets::EulerCromber() {
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setPosition((objects.at(i).getPosition() + objects.at(i).getVelocity()*dt));
    }
    calcAccels();
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setVelocity(objects.at(i).getVelocity() + objects[i].getAccel()*dt);
    }
}

void Planets::VelocityVerlet() {
    if (integrationInitialize != 0) {                 // initialization is required
        for (int i = 0; i < objects.length(); i++) {
            objects[i].setVelocity_plus_half(objects[i].getVelocity() + objects[i].getAccel() * dt / 2.0);
        }
        integrationInitialize = 0;
    }
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setVelocity_plus_half(objects[i].getVelocity() + objects[i].getAccel() * dt / 2.0);
    }
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setPosition(objects[i].getPosition() + objects[i].getVelocity_plus_half() * dt);
    }
    calcAccels();
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setVelocity(objects[i].getVelocity_plus_half() + objects[i].getAccel() * dt / 2.0);
    }
}

void Planets::Yoshida() {
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setX_i(1, objects[i].getPosition() + yoshidaC1*objects.at(i).getVelocity()*dt);
    }
    calcAccels(1);
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setV_i(1,objects[i].getVelocity() + yoshidaD1*objects[i].getAccel()*dt);
    }

    for (int i = 0; i < objects.length(); i++) {
        objects[i].setX_i(2, objects[i].getX_i(1) + yoshidaC2*objects[i].getV_i(1)*dt);
    }
    calcAccels(2);
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setV_i(2, objects[i].getV_i(1) + yoshidaD2*objects[i].getAccel()*dt);
    }

    for (int i = 0; i < objects.length(); i++) {
        objects[i].setX_i(3, objects[i].getX_i(2) + yoshidaC3*objects[i].getV_i(2)*dt);
    }
    calcAccels(3);
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setV_i(3, objects[i].getV_i(2) + yoshidaD3*objects[i].getAccel()*dt);
    }

    for (int i = 0; i < objects.length(); i++) {
        objects[i].setX_i(4, objects[i].getX_i(3) + yoshidaC4*objects[i].getV_i(3)*dt);
    }
    for (int i = 0; i < objects.length(); i++) {
        objects[i].setV_i(4, objects[i].getV_i(3));
    }

    for (int i = 0; i < objects.length(); i++) {
        objects[i].setPosition(objects[i].getX_i(4));
        objects[i].setVelocity(objects[i].getV_i(4));
    }
}

void Planets::Hermite() {
    calcAccels(0, true);

    // these are the predicted values
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setR_P(objects.at(i).getPosition() + objects.at(i).getVelocity() * dt + 1/2.0L*objects.at(i).getAccel() * dt2 + 1/6.0L * objects.at(i).getJerk()*dt3);
        objects[i].setV_P(objects.at(i).getVelocity() + objects.at(i).getAccel()    * dt + 1/2.0L*objects.at(i).getJerk()  * dt2);
    }

    calcAccels(5, true);

    // these are the corrected values
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].setVelocity(objects.at(i).getVelocity() + dt/2.0L * (objects.at(i).getOldAccel() + objects.at(i).getAccel()   ) + dt2/12.0L * (objects.at(i).getOldJerk()  +  objects.at(i).getJerk()));
        objects[i].setPosition(objects.at(i).getPosition() + dt/2.0L * (objects.at(i).getOldVel()   + objects.at(i).getVelocity()) + dt2/12.0L * (objects.at(i).getOldAccel() + objects.at(i).getAccel()));
    }
}

void Planets::RK4() {
    Point3D d;
    for (int i = 0; i < objects.length(); i++) {
        RK4_k_r[0][i] = objects.at(i).getPosition();
        RK4_k_v[0][i] = objects.at(i).getVelocity();
        RK4_k_a[0][i] = Point3D();
        for (int j = 0; j < objects.length(); j++) {
            if (i != j) {
                d = objects.at(j).getPosition() - RK4_k_r[0][i];
                RK4_k_a[0][i] += G * objects.at(j).getMass() * d / (d.length() * d.magSqr());
            }
        }
    }

    for (int i = 0; i < objects.length(); i++) {
        RK4_k_v[1][i] = RK4_k_v[0][i] + RK4_k_a[0][i] * dt * 0.5;
        RK4_k_r[1][i] = RK4_k_r[0][i] + RK4_k_v[0][i] * dt * 0.5;
        RK4_k_a[1][i] = Point3D();
        for (int j = 0; j < objects.length(); j++) {
            if (i != j) {
                d = objects.at(j).getPosition() - RK4_k_r[1][i];
                RK4_k_a[1][i] += G * objects.at(j).getMass() * d / (d.length() * d.magSqr());
            }
        }
    }

    for (int i = 0; i < objects.length(); i++) {
        RK4_k_v[2][i] = RK4_k_v[0][i] + RK4_k_a[1][i] * dt * 0.5;
        RK4_k_r[2][i] = RK4_k_r[0][i] + RK4_k_v[1][i] * dt * 0.5;
        RK4_k_a[2][i] = Point3D();
        for (int j = 0; j < objects.length(); j++) {
            if (i != j) {
                d = objects.at(j).getPosition() - RK4_k_r[2][i];
                RK4_k_a[2][i] += G * objects.at(j).getMass() * d / (d.length() * d.magSqr());
            }
        }
    }

    for (int i = 0; i < objects.length(); i++) {
        RK4_k_v[3][i] = RK4_k_v[0][i] + RK4_k_a[2][i] * dt;
        RK4_k_r[3][i] = RK4_k_r[0][i] + RK4_k_v[2][i] * dt;
        RK4_k_a[3][i] = Point3D();
        for (int j = 0; j < objects.length(); j++) {
            if (i != j) {
                d = objects.at(j).getPosition() - RK4_k_r[3][i];
                RK4_k_a[3][i] += G * objects.at(j).getMass() * d / (d.length() * d.magSqr());
            }
        }
    }

    for (int i = 0; i < objects.length(); i++) {
        objects[i].setPosition(objects.at(i).getPosition() + (RK4_k_v[0][i] + 2.0 * RK4_k_v[1][i] + 2.0 * RK4_k_v[2][i] + RK4_k_v[3][i]) / 6.0L * dt);
        objects[i].setVelocity(objects.at(i).getVelocity() + (RK4_k_a[0][i] + 2.0 * RK4_k_a[1][i] + 2.0 * RK4_k_a[2][i] + RK4_k_a[3][i]) / 6.0L * dt);
    }
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
    viewerAngle = Point3D(static_cast<Point3D::dataType>(ui->viewerAngleXSpinBox->value()),
                          static_cast<Point3D::dataType>(ui->viewerAngleYSpinBox->value()),
                          static_cast<Point3D::dataType>(ui->viewerAngleZSpinBox->value()));
    viewerPosition = Point3D(static_cast<Point3D::dataType>(ui->viewerPositionXSpinBox->value()),
                             static_cast<Point3D::dataType>(ui->viewerPositionYSpinBox->value()),
                             static_cast<Point3D::dataType>(ui->viewerPositionZSpinBox->value()));
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
    systemTime = 0;                             // how long the system has been running
    options->setTrace(true);                    // whether or not to draw the previous positions of the planets
    options->setTraceNumber(1000);              // the number of previous positions to record
    options->setSystemCentered(true);           // whether or not the screen stays centered on the system
    systemMass = 100;                           // this is all of the mass in the entire system
    centerOfMass = Point3D(0, 0, 0);            // the current center of mass of the system
    objects.clear();                            // the list of the orbiting objects
    trails.clear();                             // the list of the trails of all of the orbiting objects
    systemMomentum = Point3D(0, 0, 0);
    timer->stop();
    removedObjects.setPosition(Point3D(0, 0, 0));
    removedObjects.setMass(0);
    removedObjects.setVelocity(Point3D(0, 0, 0));
    activeScenario = defaultScenario();
    firstRun = true;
    linesPrinted = 0;
    if (file.isOpen()) file.close();

    for (int i = 0; i < scenarios.length(); i++ ) {
        if (scenarios.at(i).name == ui->comboBox->currentText() && scenarioIsValid(scenarios.at(i))) {
            activeScenario = scenarios.at(i);
        }
    }

    if (scenarioIsValid(activeScenario)) {
        for (int i = 0; i < activeScenario.planets.length(); i++) {
            objects.append(activeScenario.planets.at(i));
            G = activeScenario.bigG;
            dt = activeScenario.dt;
        }
    }
    // this is to ensure that the UI doesn't try to calculate the grav field when the user can't see the result
    if (activeScenario.name == "Solar System") {
        options->setDisplayGravField(false);
        options->setEnableGravField(false);
        ui->distanceScalarSpin->setMinimum(2000);
        ui->distanceScalarSpin->setMaximum(15000000);
        ui->distanceScalarSpin->setSingleStep(1000);
        ui->distanceScalarSpin->setValue(200000);
        ui->integrationComboBox->setCurrentText(defaultIntegrationType);
    } else {
        options->setEnableGravField(true);
        ui->distanceScalarSpin->setMinimum(0.05);
        ui->distanceScalarSpin->setMaximum(100);
        ui->distanceScalarSpin->setSingleStep(0.05);
        ui->integrationComboBox->setCurrentText(defaultIntegrationType);
    }

    ui->dT->setText(QString::number(double(dt)));
    ui->timeUnits->setText(activeScenario.timeUnit);
    ui->distanceUnits->setText(activeScenario.lengthUnit);
    ui->massUnits->setText(activeScenario.massUnit);
    ui->gravConst->setText(QString::number(double(activeScenario.bigG)));
    ui->bodyCount->setText(QString::number(double(activeScenario.planets.length())));
    ui->endTime->setText(QString::number(double(activeScenario.endTime)));
    ui->iterPerData->setText(QString::number(int(activeScenario.iterationsPerDataPoint)));

    // the following code makes the system centered and insures that the whole system does not go flying off
    calculateSystemProperties();
    for (int i = 0; i < objects.size(); i++) {
        objects[i].setVelocity(objects[i].getVelocity() - systemMomentum/systemMass);
    }
    options->setSystemVelocity(systemMomentum/systemMass);

    ui->freeFormBox->setVisible(activeScenario.name == "Free Form");
    ui->SolarSystemFormationGroupBox->setVisible(activeScenario.name == "Solar System Formation");

    ui->planetComboBox->clear();
    for (int i = 0; i < objects.size(); i++)
    {
        // if the system is centered, move everything so that the center of mass is at the origin
//        if (options->systemCentered())
//            objects[i].setPosition(objects.at(i).getPosition() - centerOfMass);
        // if operating in 2D, make sure that everything is in the XY plane
        if (options->newtonian2D() || options->deflection())
        {
            objects[i].setPosition(Point3D(objects[i].getPosition().x(), objects[i].getPosition().y(), 0));
            objects[i].setVelocity(Point3D(objects[i].getVelocity().x(), objects[i].getVelocity().y(), 0));
        }

        // add everything to the free form combo box
        ui->planetComboBox->addItem(objects.at(i).getName());
    }

    dt2 = dt*dt;
    dt3 = dt*dt2;

    // this section initializes all of the data storage and capture.
    // it adds all of the headings, so there is more than just a ton of numbers
    data = "Time,";
    for (int i = 0; i < objects.size(); i++)
        for (int j = 0; j < 14; j++)
            data += objects.at(i).getName() + ",";
    data += "\n" + activeScenario.timeUnit + ",";
    for (int i = 0; i < objects.size(); i++)
        data += "Name,Mass,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z,Accel X,Accel Y,Accel Z,Jerk X,Jerk Y,Jerk Z,";
    data += "\n";

    update();

/*    qDebug() << "Reset complete," << activeScenario.planets.length() << "objects, scenario name" << activeScenario.name;
    for (int i = 0; i < scenarios.length(); i++) {
        qDebug() << "    Scenario " << scenarios.at(i).name << " has" << scenarios.at(i).planets.length() << "objects";
    }*/
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
                        Object(Point3D(static_cast<Point3D::dataType>(ui->positionX->value()), static_cast<Point3D::dataType>(ui->positionY->value()), static_cast<Point3D::dataType>(ui->positionZ->value())),
                               Point3D(static_cast<Point3D::dataType>(ui->velocityX->value()), static_cast<Point3D::dataType>(ui->velocityY->value()), static_cast<Point3D::dataType>(ui->velocityZ->value())),
                               static_cast<Point3D::dataType>(ui->massSpin->value()), 20, ui->nameLine->text(),
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
    QWidget::mouseReleaseEvent(event);
}

void Planets::wheelEvent(QWheelEvent * event)
{
    double step = 1.0;
    if (ui->distanceScalarSpin->value() < 5) step = 0.25;
    else if (ui->distanceScalarSpin->value() < 50) step = 2.5;
    else if (ui->distanceScalarSpin->value() < 500) step = 25;
    else if (ui->distanceScalarSpin->value() < 5000) step = 250;
    else if (ui->distanceScalarSpin->value() < 50000) step = 2500;

    ui->distanceScalarSpin->setValue(ui->distanceScalarSpin->value() + step * event->angleDelta().y()/abs(event->angleDelta().y()));
    zoomPoint = (event->position() - QPointF(width()/2, height()/2)).toPoint();
}

void Planets::changeIntegrationType(QString newType)
{
    integrationType = newType;
    if (integrationType == "Velocity Verlet") {
        integrationInitialize = 1;
    } else integrationInitialize = 0;
}

void Planets::saveStateButton_clicked()
{
    calcAccels(0,true);
    QString saveMessage;
    for (int i = 0; i < objects.size(); i++)
        for (int j = 0; j < 16; j++)
            saveMessage += objects.at(i).getName() + ",";
    saveMessage += "\n,";
    for (int i = 0; i < objects.size(); i++)
        saveMessage += "Name,Mass,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z,Speed,|Momentum|,Accel X,Accel Y,Accel Z,Jerk X,Jerk Y,Jerk Z,";
    saveMessage += "\n";
    saveMessage += toStringL(systemTime) + ",";
    for (int i = 0; i < objects.size(); i++)
    {
        saveMessage += toStringO(objects.at(i), true, true);
    }
    saveMessage += "\n";
    ui->textBrowser->setText(saveMessage);
}
