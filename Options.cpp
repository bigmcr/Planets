#include "Options.h"
#include "ui_Options.h"

Options::Options(QWidget *parent) :
    QTabWidget(parent),
ui(new Ui::Options)
{
    ui->setupUi(this);
    connect(ui->traceCheckBox, SIGNAL(clicked()), this, SIGNAL(somethingChanged()));
    connect(ui->traceCheckBox, SIGNAL(clicked(bool)), this, SIGNAL(traceChanged(bool)));
    connect(ui->applyChanges1, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->applyChanges2, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->applyChanges3, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->newtonian2DRadioButton, SIGNAL(clicked()), this, SIGNAL(displayTypeChanged()));
    connect(ui->newtonian3DRadioButton, SIGNAL(clicked()), this, SIGNAL(displayTypeChanged()));
    connect(ui->relativity2DRadioButton, SIGNAL(clicked()), this, SIGNAL(displayTypeChanged()));

    QGridLayout * gridLayout = new QGridLayout(ui->planetFrame);
    for (int i = 0; i < 16; i++)
    {
        checks[i] = new QCheckBox("Planet " + QString::number(i + 1));
        gridLayout->addWidget(checks[i], i/4, i%4, 1, 1);
    }

    ui->label_6->setHidden(true);
    ui->arrowLengthSlider->setHidden(true);

    resize(sizeHint());

}

Options::~Options()
{
    delete ui;
}

QColor Options::numberToColor(int index) const
{
    switch (index)
    {
    case 0:
        return Qt::white;
    case 1:
        return Qt::black;
    case 2:
        return Qt::red;
    case 3:
        return Qt::green;
    case 4:
        return Qt::blue;
    case 5:
        return Qt::cyan;
    case 6:
        return Qt::magenta;
    case 7:
        return Qt::yellow;
    case 8:
        return Qt::gray;
    case 9:
        return Qt::darkRed;
    case 10:
        return Qt::darkGreen;
    case 11:
        return Qt::darkCyan;
    case 12:
        return Qt::darkMagenta;
    case 13:
        return Qt::darkYellow;
    case 14:
        return Qt::darkGray;
    case 15:
        return Qt::lightGray;
    }
    return Qt::white;
}

bool Options::usePolar() const
{
    return ui->usePolarCheckBox->isChecked();
}

bool Options::displayTrace() const
{
    return ui->traceCheckBox->isChecked();
}

int Options::traceNumber() const
{
    return ui->traceNumberSpinBox->value();
}

bool Options::systemCentered() const
{
    return ui->systemCenteredCheckBox->isChecked();
}

double Options::systemVelocityX() const
{
    return ui->systemVelocityXSpinBox->value();
}

double Options::systemVelocityY() const
{
    return ui->systemVelocityYSpinBox->value();
}

double Options::systemVelocityZ() const
{
    return ui->systemVelocityZSpinBox->value();
}

Point3D Options::systemVelocity() const
{
    return Point3D(static_cast<long double>(ui->systemVelocityXSpinBox->value()),
                   static_cast<long double>(ui->systemVelocityYSpinBox->value()),
                   static_cast<long double>(ui->systemVelocityZSpinBox->value()));
}

bool Options::displayCenterOfMass() const
{
    return ui->displayCenterOfMassCheckBox->isChecked();
}

bool Options::displayPlanets() const
{
    return ui->displayPlanetsCheckBox->isChecked();
}

bool Options::displayVelocity() const
{
    return ui->displayVelocityCheckBox->isChecked();
}

bool Options::displayAccel() const
{
    return ui->displayAccelCheckBox->isChecked();
}

QColor Options::velocityColor() const
{
    return numberToColor(ui->velocityColorComboBox->currentIndex());
}

QColor Options::accelColor() const
{
    return numberToColor(ui->accelColorComboBox->currentIndex());
}

QColor Options::relativityColor() const
{
    return numberToColor(ui->relativityColorComboBox->currentIndex());
}

bool Options::perspectiveView() const
{
return ui->perspectiveViewCheckBox->isChecked();
}

void Options::setUsePolar(bool newUsePolar)
{
    ui->usePolarCheckBox->setChecked(newUsePolar);
}

void Options::setTrace(bool newTrace)
{
    ui->traceCheckBox->setChecked(newTrace);
}

void Options::setTraceNumber(int newTraceNumber)
{
    ui->traceNumberSpinBox->setValue(newTraceNumber);
}

void Options::setSystemCentered(bool newSystem)
{
    ui->systemCenteredCheckBox->setChecked(newSystem);
}

void Options::setSystemVelocityX(double newX)
{
    ui->systemVelocityXSpinBox->setValue(newX);
}

void Options::setSystemVelocityY(double newY)
{
    ui->systemVelocityYSpinBox->setValue(newY);
}

void Options::setSystemVelocityZ(double newZ)
{
    ui->systemVelocityZSpinBox->setValue(newZ);
}

void Options::setSystemVelocity(Point3D newSys)
{
    ui->systemVelocityXSpinBox->setValue(static_cast<double>(newSys.x()));
    ui->systemVelocityYSpinBox->setValue(static_cast<double>(newSys.y()));
    ui->systemVelocityZSpinBox->setValue(static_cast<double>(newSys.z()));
}

int Options::getRotateSensitivity() const
{
    return ui->rotateSlider->value();
}

int Options::getPanSensitivity() const
{
    return ui->panSlider->value();
}

bool Options::enableMouseRotate() const
{
    return ui->enableRotateCheckBox->isChecked();
}

bool Options::enableMousePan() const
{
    return ui->enablePanCheckBox->isChecked();
}


//accessor functions for Gravity Field
bool Options::displayGravField() const
{
    return ui->gravFieldCheckBox->isChecked();
}

bool Options::gravPlaneGoesThroughCenterOfMass() const
{
    return ui->centerOfMassPlane->isChecked();
}

void Options::setDisplayGravField(bool value)
{
    ui->gravFieldCheckBox->setChecked(value);
}

void Options::setEnableGravField(bool value)
{
ui->gravFieldCheckBox->setEnabled(value);
}

bool Options::transparentArrows() const
{
    return ui->arrowsAreTransparent->isChecked();
}

bool Options::arrowsAdjustLength() const
{
    return ui->arrowsAdjustLengthButton->isChecked();
}

int Options::arrowSpacing() const
{
    return ui->arrowSpacingSlider->value();
}

int Options::arrowSquareWidth() const
{
    return ui->arrowSquareWidthSlider->value();
}

int Options::arrowLength() const
{
    return ui->arrowLengthSlider->value();
}

int Options::arrowTransparency() const
{
    return ui->arrowTransparencySlider->value();
}

void Options::getGravityPlane(int &mode, int & planet1, int & planet2, int & planet3,
                              int numberOfPlanets, int &checkedPlanets) const
{
    if (ui->xyPlaneRadioButton->isChecked()) {mode = 0; return;}
    if (ui->xzPlaneRadioButton->isChecked()) {mode = 1; return;}
    if (ui->yzPlaneRadioButton->isChecked()) {mode = 2; return;}
    mode = 4;
    for (int i = 0; i < numberOfPlanets; i++)
        checks[i]->setVisible(true);
    // this is to send back the number of the planets that need to be selected.
    // so if Planet 1, 4 and 8 are checked, the planets would be 1, 4, and 8, and checkedPlanets would equal 3
    // if only Planet 1 was checked, the planets would be 1, 0, 0, abd checkedPlanets would equal 1
    planet1 = planet2 = planet3 = 0;
    QList<int> planets;
    for (int i = 0; i < numberOfPlanets; i++)
        if (checks[i]->isEnabled() && checks[i]->isChecked()) planets.append(i);
    if (planets.size() >= 1) {planet1 = planets.at(0); checkedPlanets++;}
    if (planets.size() >= 2) {planet2 = planets.at(1); checkedPlanets++;}
    if (planets.size() >= 3) {planet3 = planets.at(2); checkedPlanets++;}
    return;
}

// accessor functions for Relativity
bool Options::newtonian2D() const
{
    return ui->newtonian2DRadioButton->isChecked();
}

bool Options::newtonian3D() const
{
    return ui->newtonian3DRadioButton->isChecked();
}

bool Options::deflection() const
{
    return ui->relativity2DRadioButton->isChecked();
}

int Options::deflectionSquareWidth() const
{
    return ui->deflectionWidthSlider->value();
}

int Options::deflectionSpacing() const
{
    return ui->deflectionSpacingSlider->value();
}

void Options::check3DRadioButton()
{
    ui->newtonian3DRadioButton->setChecked(true);
}

void Options::check2DRadioButton()
{
    ui->newtonian2DRadioButton->setChecked(true);
}

void Options::checkRelativityRadioButton()
{
    return ui->relativity2DRadioButton->setChecked(true);
}
