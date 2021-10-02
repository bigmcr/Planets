#ifndef SCENARIODIALOG_H
#define SCENARIODIALOG_H

#include <QDialog>
#include <Scenario.h>

namespace Ui {
class ScenarioDialog;
}

class ScenarioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScenarioDialog(QWidget *parent = nullptr);
    ~ScenarioDialog();

signals:
    scenario loadScenario();

private slots:
    void on_gravConstSpinBox_valueChanged(double newBigG);
    void on_dtSpinBox_valueChanged(int newPower);
    void on_endTimeSpinBox_valueChanged(double newEndTime);
    void on_iterPerDataSpinBox_valueChanged(int newIterPerData);
    void on_nameLineEdit_editingFinished();
    void on_gravConstPowerSpinBox_valueChanged(int arg1);
    void on_lengthUnitComboBox_currentIndexChanged(const QString &arg1);
    void on_timeUnitComboBox_currentIndexChanged(const QString &arg1);
    void on_massUnitComboBox_currentIndexChanged(const QString &arg1);
    void on_planetSelectedComboBox_currentIndexChanged(int index);
    void on_planetCountNumber_valueChanged(int arg1);

    void on_readScenarioPushButton_released();

    void on_writeScenarioPushButton_released();

private:
    Ui::ScenarioDialog *ui;
    scenario activeScen;
    int oldPlanetIndex;

    void loadPlanetIntoUI(int planetNumber);
    void loadPlanetFromUI(int planetNumber);
    void setGravConstDisplay();
};

#endif // SCENARIODIALOG_H
