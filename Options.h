#ifndef OPTIONS_H
#define OPTIONS_H

#include <QTabWidget>
#include <QCheckBox>
#include "Point3D.h"

namespace Ui {
    class Options;
}

class Options : public QTabWidget
{
    Q_OBJECT

private:
	Ui::Options *ui;
        QCheckBox * checks[16];
	QColor numberToColor(int index) const;

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    // accessor functions for Gravity Field
    bool displayGravField() const;
    bool gravPlaneGoesThroughCenterOfMass() const;

    // function that returns three points that define a plane in 3D space.
    void getGravityPlane(int &mode, int &planet1, int &planet2, int &planet3, int numberOfPlanets, int &checkedPlanets) const;

    void setDisplayGravField(bool value);
    void setEnableGravField(bool value);

    bool transparentArrows() const;
    bool arrowsAdjustLength() const;
    int arrowSpacing() const;
    int arrowLength() const;
    int arrowTransparency() const;
    int arrowSquareWidth() const;

    // accessor functions for System
    bool usePolar() const;
    bool displayTrace() const;
    int traceNumber() const;
    bool systemCentered() const;
    double systemVelocityX() const;
    double systemVelocityY() const;
    double systemVelocityZ() const;
    Point3D systemVelocity() const;

    void setUsePolar(bool newUsePolar);
    void setTrace(bool newTrace);
    void setTraceNumber(int newTraceNumber);
    void setSystemCentered(bool newSystem);
    void setSystemVelocityX(double newX);
    void setSystemVelocityY(double newY);
    void setSystemVelocityZ(double newY);
    void setSystemVelocity(Point3D newSys);

    // accessor functions for Display Options
    bool displayCenterOfMass() const;
    bool displayPlanets() const;
    bool displayVelocity() const;
    bool displayAccel() const;
    QColor velocityColor() const;
    QColor accelColor() const;
    bool perspectiveView() const;

    int getRotateSensitivity() const;
    int getPanSensitivity() const;
    bool enableMouseRotate() const;
    bool enableMousePan() const;

    // accessor functions for Relativity
    bool newtonian2D() const;
    bool newtonian3D() const;
    bool deflection() const;
    int deflectionSquareWidth() const;
    int deflectionSpacing() const;
    QColor relativityColor() const;

signals:
	void somethingChanged();
    void traceChanged(bool traceValue);

    void displayTypeChanged();

public slots:
    void check3DRadioButton();
    void check2DRadioButton();
    void checkRelativityRadioButton();
};

#endif // OPTIONS_H
