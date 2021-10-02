#include <Scenario.h>
#include <QDataStream>
#include <QFileDialog>

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

Point3D stringToPoint3D(QString string) {
    Point3D point(-1234, -1234, -1234);
    QStringList individualStrings = string.split(",");
    if (individualStrings.length() == 3) {
        point.setX(individualStrings.at(0).toDouble());
        point.setY(individualStrings.at(1).toDouble());
        point.setZ(individualStrings.at(2).toDouble());
    }
    return point;
}

bool writeScenariosV1(QVector<scenario> scens, QString filename) {
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream outputToFile;
        outputToFile.setRealNumberPrecision(16);
        outputToFile.setDevice(&file);
        outputToFile << "Scenario v0.1\n";
        for (int i = 0; i < scens.length(); i++) {
            outputToFile << "name " << scens.at(i).name << "\n";
            outputToFile << "bigG " << static_cast<double>(scens.at(i).bigG) << "\n";
            outputToFile << "dt " << static_cast<double>(log2(scens.at(i).dt)) << "\n";
            outputToFile << "iterationsPerDataPoint " << scens.at(i).iterationsPerDataPoint << "\n";
            outputToFile << "endTime " << static_cast<double>(scens.at(i).endTime) << "\n";
            outputToFile << "lengthUnit " << scens.at(i).lengthUnit << "\n";
            outputToFile << "massUnit " << scens.at(i).massUnit << "\n";
            outputToFile << "timeUnit " << scens.at(i).timeUnit << "\n";
            for (int j = 0; j < scens.at(i).planets.length(); j++) {
                outputToFile << "planet\n";
                outputToFile << "    position " << scens.at(i).planets.at(j).getPosition().toString() << "\n";
                outputToFile << "    velocity " << scens.at(i).planets.at(j).getVelocity().toString() << "\n";
                outputToFile << "    mass " << static_cast<double>(scens.at(i).planets.at(j).getMass()) << "\n";
                outputToFile << "    radius " << static_cast<double>(scens.at(i).planets.at(j).getRadius()) << "\n";
                outputToFile << "    name " << scens.at(i).planets.at(j).getName() << "\n";
                outputToFile << "    color " << Point3D(scens.at(i).planets.at(j).getColor().red(), scens.at(i).planets.at(j).getColor().green(), scens.at(i).planets.at(j).getColor().blue()).toString() << "\n";
            }
            outputToFile << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

bool readScenariosV1(QVector<scenario> * scenarios, QString filename) {
    QFile file(filename);
    QString errorMessage;
    QString errorLine;
    if (file.open(QFile::ReadOnly)) {
        QString temp;
        QTextStream inputFromFile;
        inputFromFile.setRealNumberPrecision(16);
        inputFromFile.setDevice(&file);
        temp = inputFromFile.readLine();
        if (temp != QString("Scenario v0.1")) {errorMessage = "Wrong Scenario Version!";}

        scenario newScen;
        Object newPlanet;
        while (!inputFromFile.atEnd() && errorMessage.isNull()) {
            newScen = defaultScenario();
            temp = inputFromFile.readLine();
            if (temp.indexOf("name ") != 0) {errorMessage = "Name incorrect!"; errorLine = temp;}
            else newScen.name = temp.mid(5);

            temp = inputFromFile.readLine();
            if (temp.indexOf("bigG ") != 0)  {errorMessage = "bigG incorrect!"; errorLine = temp;}
            else newScen.bigG = temp.midRef(5).toDouble();

            temp = inputFromFile.readLine();
            if (temp.indexOf("dt ") != 0)  {errorMessage = "dt incorrect!"; errorLine = temp;}
            else newScen.dt = powl(2.0, temp.midRef(3).toDouble());

            temp = inputFromFile.readLine();
            if (temp.indexOf("iterationsPerDataPoint ") != 0) {errorMessage = "iterationsPerDataPoint incorrect!"; errorLine = temp;}
            else newScen.iterationsPerDataPoint = temp.midRef(23).toInt();

            temp = inputFromFile.readLine();
            if (temp.indexOf("endTime ") != 0) {errorMessage = "endTime incorrect!"; errorLine = temp;}
            else newScen.endTime = temp.midRef(8).toDouble();

            temp = inputFromFile.readLine();
            if (temp.indexOf("lengthUnit ") != 0) {errorMessage = "lengthUnit incorrect!"; errorLine = temp;}
            else newScen.lengthUnit = temp.mid(11);

            temp = inputFromFile.readLine();
            if (temp.indexOf("massUnit ") != 0) {errorMessage = "massUnit incorrect!"; errorLine = temp;}
            else newScen.massUnit = temp.mid(9);

            temp = inputFromFile.readLine();
            if (temp.indexOf("timeUnit ") != 0) {errorMessage = "timeUnit incorrect!"; errorLine = temp;}
            else newScen.timeUnit = temp.mid(9);

            while (inputFromFile.readLine() == "planet" && errorMessage.isNull()) {
                newPlanet = Object();

                temp = inputFromFile.readLine();
                if (temp.indexOf("    position ") != 0)  {errorMessage = "position incorrect!"; errorLine = temp;}
                else newPlanet.setPosition(stringToPoint3D(temp.mid(13)));

                temp = inputFromFile.readLine();
                if (temp.indexOf("    velocity ") != 0) {errorMessage = "velocity incorrect!"; errorLine = temp;}
                else newPlanet.setVelocity(stringToPoint3D(temp.mid(13)));

                temp = inputFromFile.readLine();
                if (temp.indexOf("    mass ") != 0) {errorMessage = "mass incorrect!"; errorLine = temp;}
                else newPlanet.setMass(temp.midRef(9).toDouble());

                temp = inputFromFile.readLine();
                if (temp.indexOf("    radius ") != 0) {errorMessage = "radius incorrect!"; errorLine = temp;}
                else newPlanet.setRadius(temp.midRef(11).toDouble());

                temp = inputFromFile.readLine();
                if (temp.indexOf("    name ") != 0) {errorMessage = "name incorrect!"; errorLine = temp;}
                else newPlanet.setName(temp.mid(9));

                temp = inputFromFile.readLine();
                if (temp.indexOf("    color ") != 0) {errorMessage = "color incorrect!"; errorLine = temp;}
                else {
                    Point3D colorPoint = stringToPoint3D(temp.mid(10));
                    newPlanet.setColor(QColor(colorPoint.x(), colorPoint.y(), colorPoint.z(), 255));
                }

                newScen.planets.append(newPlanet);

            }
            scenarios->append(newScen);
        }
        file.close();
    } else errorMessage = "Failed to open the file";
    if (errorMessage.isNull()) return true;
    else {
        qDebug() << "message:" << errorMessage;
        qDebug() << "line:" << errorLine;
        return false;
    }
}
