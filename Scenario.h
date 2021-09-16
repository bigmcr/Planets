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

#endif // SCENARIO_H
