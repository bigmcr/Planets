#include <Scenario.h>

// checks to see if the scenario is valid - IE all of the appropriate points have been written to
bool scenarioIsValid(scenario scen) {
    bool returnMe = true;
    returnMe = returnMe && (scen.name != QString());
    returnMe = returnMe && (scen.timeUnit != QString());
    returnMe = returnMe && (scen.lengthUnit != QString());
    returnMe = returnMe && (scen.massUnit != QString());
    returnMe = returnMe && (scen.bigG > 0);
    returnMe = returnMe && (scen.dt > 0);
    returnMe = returnMe && ((scen.endTime > scen.dt) || scen.endTime == -1);
    returnMe = returnMe && (scen.planets.length() != 0);
    returnMe = returnMe && (scen.iterationsPerDataPoint > 0);
    return returnMe;
}

// resets a scenario to defaults
scenario defaultScenario() {
    scenario newScenario;
    return newScenario;
}

