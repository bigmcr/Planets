#ifndef SCENARIO_H
#define SCENARIO_H
#include <Object.h>

struct scenario {
    QString name = "";
    QString timeUnit = "", lengthUnit = "", massUnit = "";
    Point3D::dataType bigG = 0, dt = 0, endTime = 0;
    QVector<Object> planets;
    bool isActive = false;
    uint iterationsPerDataPoint;
};

// checks to see if the scenario is valid - IE all of the appropriate points have been written to
bool scenarioIsValid(scenario scen);

// resets a scenario to defaults
scenario defaultScenario();

// writes all constant scenarios to the specifed file
bool writeScenariosV1(QVector<scenario> scens, QString filename);

// read all constant scenarios from the specifed file
bool readScenariosV1(QVector<scenario> * scenarios, QString filename);

// writes all variable scenarios to the specifed file
bool writeScenariosV2(QVector<scenario> scens, QString filename);

// read all variable scenarios from the specifed file
bool readScenariosV2(QVector<scenario> * scenarios, QString filename);

#endif // SCENARIO_H
