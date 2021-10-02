#include "scenariodialog.h"
#include "ui_scenariodialog.h"
#include <unit.h>

long double powerOfTen(int power) {
    long double returnMe = 1.0;
    if (power > 0) {
        for (int i = 0; i < power; i++) {
            returnMe *= 10.0;
        }
    } else {
        for (int i = 0; i < -power; i++) {
            returnMe /= 10.0;
        }
    }
    return returnMe;
}

ScenarioDialog::ScenarioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScenarioDialog)
{
    ui->setupUi(this);

    for (int i = 0; i < QColor::colorNames().length(); i++) {
        ui->planetColorComboBox->addItem(QColor::colorNames().at(i));
    }

    for (int i = 0; i < Unit::lengthUnitList().length(); i++) {
        ui->lengthUnitComboBox->insertItem(0, Unit::lengthUnitList().at(i));
    }
    for (int i = 0; i < Unit::timeUnitList().length(); i++) {
        ui->timeUnitComboBox->insertItem(0, Unit::timeUnitList().at(i));
    }
    for (int i = 0; i < Unit::massUnitList().length(); i++) {
        ui->massUnitComboBox->insertItem(0, Unit::massUnitList().at(i));
    }
    activeScen.bigG = ui->gravConstSpinBox->value() * powerOfTen(ui->gravConstPowerSpinBox->value());
    ui->lengthUnitComboBox->setCurrentText("meter");
    ui->timeUnitComboBox->setCurrentText("second");
    ui->massUnitComboBox->setCurrentText("kilogram");
    on_lengthUnitComboBox_currentIndexChanged(ui->lengthUnitComboBox->currentText());
    on_timeUnitComboBox_currentIndexChanged(ui->timeUnitComboBox->currentText());
    on_massUnitComboBox_currentIndexChanged(ui->massUnitComboBox->currentText());
    on_planetCountNumber_valueChanged(ui->planetCountNumber->value());
    setGravConstDisplay();
}

ScenarioDialog::~ScenarioDialog()
{
    delete ui;
}

void ScenarioDialog::on_gravConstSpinBox_valueChanged(double newBigG)
{
    if (newBigG > 0) activeScen.bigG = newBigG * powerOfTen(ui->gravConstPowerSpinBox->value());
    setGravConstDisplay();
}

void ScenarioDialog::on_dtSpinBox_valueChanged(int newPower)
{
    activeScen.dt = powl(2.0, newPower);
}

void ScenarioDialog::on_endTimeSpinBox_valueChanged(double newEndTime)
{
    if (newEndTime > 0) activeScen.endTime = newEndTime;
}

void ScenarioDialog::on_iterPerDataSpinBox_valueChanged(int newIterPerData)
{
    if (newIterPerData > 0) activeScen.iterationsPerDataPoint = newIterPerData;
}

void ScenarioDialog::on_nameLineEdit_editingFinished()
{
    if (!ui->nameLineEdit->text().isNull()) activeScen.name = ui->nameLineEdit->text();
}

void ScenarioDialog::on_gravConstPowerSpinBox_valueChanged(int arg1)
{
    activeScen.bigG = ui->gravConstSpinBox->value() * powerOfTen(arg1);
    setGravConstDisplay();
}

void ScenarioDialog::on_lengthUnitComboBox_currentIndexChanged(const QString &arg1)
{
    if (Unit::isValidLength(arg1)) {
        QString newSymbol = Unit::getSymbol(arg1);
        ui->lengthUnitSymbolLineEdit->setText(newSymbol);

        setGravConstDisplay();
        ui->planetRadiusSpinBox->setSuffix(" " + newSymbol);
        ui->positionUnitLabel->setText(newSymbol);
        ui->velocityUnitLabel->setText(ui->lengthUnitSymbolLineEdit->text() + "/" + ui->timeUnitSymbolLineEdit->text());
    }
}

void ScenarioDialog::on_timeUnitComboBox_currentIndexChanged(const QString &arg1)
{
    if (Unit::isValidTime(arg1)) {
        QString newSymbol = Unit::getSymbol(arg1);
        ui->timeUnitSymbolLineEdit->setText(newSymbol);

        setGravConstDisplay();
        ui->dtLabelUnit->setText(newSymbol);
        ui->endTimeSpinBox->setSuffix(" " + newSymbol);
        ui->velocityUnitLabel->setText(ui->lengthUnitSymbolLineEdit->text() + "/" + ui->timeUnitSymbolLineEdit->text());
    }
}

void ScenarioDialog::on_massUnitComboBox_currentIndexChanged(const QString &arg1)
{
    if (Unit::isValidMass(arg1)) {
        QString newSymbol = Unit::getSymbol(arg1);
        ui->massUnitSymbolLineEdit->setText(newSymbol);

        setGravConstDisplay();
        ui->planetMassSpinBox->setSuffix(" " + newSymbol);
    }
}

QString toString(QColor color) {
    for (int i = 0; i < QColor::colorNames().length(); i++) {
        if (color == QColor(QColor::colorNames().at(i))) return QColor::colorNames().at(i);
    }
    return "white";
}

void ScenarioDialog::on_planetSelectedComboBox_currentIndexChanged(int index)
{
    if (oldPlanetIndex < activeScen.planets.length()) loadPlanetFromUI(oldPlanetIndex);
    oldPlanetIndex = index;
    loadPlanetIntoUI(index);
}

void ScenarioDialog::on_planetCountNumber_valueChanged(int arg1)
{
    if (arg1 > 0) {
        while (activeScen.planets.length() != arg1) {
            if (activeScen.planets.length() < arg1) {
                activeScen.planets.append(Object());
            }
            if (activeScen.planets.length() > arg1) {
                activeScen.planets.removeLast();
            }
        }
        while (ui->planetSelectedComboBox->count() != arg1) {
            if (ui->planetSelectedComboBox->count() < arg1) {
                ui->planetSelectedComboBox->addItem("Planet " + QString::number(ui->planetSelectedComboBox->count() + 1));
            }
            if (ui->planetSelectedComboBox->count() > arg1) {
                ui->planetSelectedComboBox->removeItem(ui->planetSelectedComboBox->count() - 1);
            }
        }
    }
}

void ScenarioDialog::setGravConstDisplay()
{
    ui->gravConstValue->setText(QString::number(double(activeScen.bigG)));
    ui->lengthLabel->setText(ui->lengthUnitSymbolLineEdit->text() + "^3");
    ui->timeLabel->setText(ui->timeUnitSymbolLineEdit->text() + "^2");
    ui->massLabel->setText(ui->massUnitSymbolLineEdit->text() + " *");
}

void ScenarioDialog::loadPlanetIntoUI(int planetNumber)
{
    ui->planetNameLine->setText(activeScen.planets.at(planetNumber).getName());
    ui->planetRadiusSpinBox->setValue(activeScen.planets.at(planetNumber).getRadius());
    ui->planetMassSpinBox->setValue(activeScen.planets.at(planetNumber).getMass());
    ui->planetPositionX->setValue(activeScen.planets.at(planetNumber).getPosition().x());
    ui->planetPositionY->setValue(activeScen.planets.at(planetNumber).getPosition().y());
    ui->planetPositionZ->setValue(activeScen.planets.at(planetNumber).getPosition().z());
    ui->planetVelocityX->setValue(activeScen.planets.at(planetNumber).getVelocity().x());
    ui->planetVelocityY->setValue(activeScen.planets.at(planetNumber).getVelocity().y());
    ui->planetVelocityZ->setValue(activeScen.planets.at(planetNumber).getVelocity().z());
    ui->planetColorComboBox->setCurrentText(toString(activeScen.planets.at(planetNumber).getColor()));
}

void ScenarioDialog::loadPlanetFromUI(int planetNumber)
{
    activeScen.planets[planetNumber].setName(ui->planetNameLine->text());
    activeScen.planets[planetNumber].setRadius(ui->planetRadiusSpinBox->value());
    activeScen.planets[planetNumber].setMass(ui->planetMassSpinBox->value());
    activeScen.planets[planetNumber].setPosition(Point3D(ui->planetPositionX->value(), ui->planetPositionY->value(), ui->planetPositionZ->value()));
    activeScen.planets[planetNumber].setVelocity(Point3D(ui->planetVelocityX->value(), ui->planetVelocityY->value(), ui->planetVelocityZ->value()));
    QColor tempColor;
    tempColor.setNamedColor(ui->planetColorComboBox->currentText());
    activeScen.planets[planetNumber].setColor(tempColor);
}

void ScenarioDialog::on_readScenarioPushButton_released()
{
    QVector<scenario> readScenarios;
    if (readScenariosV1(&readScenarios,"C:\\Users\\McRed\\iCloudDrive\\QT Programming\\Planets\\Scenarios\\4 Star Ballet.scen"))
    {
        activeScen = readScenarios.at(0);
        double base2mantissa, base10mantissa;
        int base2power, base10power;
        base2mantissa = frexp(activeScen.bigG, &base2power);
        base10power = int(log10(base2mantissa*powl(2,base2power))) - 1;
        base10mantissa = base2mantissa*powl(10,-base10power)*powl(2,base2power);
        ui->gravConstSpinBox->setValue(base10mantissa);
        ui->gravConstPowerSpinBox->setValue(base10power);
        ui->nameLineEdit->setText(activeScen.name);
        ui->dtSpinBox->setValue(log2(activeScen.dt));
        ui->iterPerDataSpinBox->setValue(activeScen.iterationsPerDataPoint);
        ui->endTimeSpinBox->setValue(activeScen.endTime);
        ui->lengthUnitComboBox->setCurrentText(Unit::getLongName(activeScen.lengthUnit));
        ui->timeUnitComboBox->setCurrentText(Unit::getLongName(activeScen.timeUnit));
        ui->massUnitComboBox->setCurrentText(Unit::getLongName(activeScen.massUnit));
        ui->planetCountNumber->setValue(activeScen.planets.length());
        if (activeScen.planets.length() > 0) ui->planetSelectedComboBox->setCurrentIndex(0);
        for (int i = 0; i < activeScen.planets.length(); i++) {ui->planetSelectedComboBox->setItemText(i, activeScen.planets.at(i).getName());}
        loadPlanetIntoUI(0);
    }
}

void ScenarioDialog::on_writeScenarioPushButton_released()
{
    loadPlanetFromUI(ui->planetSelectedComboBox->currentIndex());
    QVector<scenario> scenarioList;
    scenarioList.append(activeScen);
    writeScenariosV1(scenarioList, "C:\\Users\\McRed\\iCloudDrive\\QT Programming\\Planets\\Scenarios\\New Scenario.scen");
}
