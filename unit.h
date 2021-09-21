#ifndef UNIT_H
#define UNIT_H
#include <QString>
#include <QStringList>

class Unit
{
private:
    static QStringList angleList, angleListSymbol;
    static QStringList lengthList, lengthListSymbol;
    static QStringList timeList, timeListSymbol;
    static QStringList massList, massListSymbol;
public:
    Unit();
    static bool isValidAngle(QString newUnit) {return angleList.contains(newUnit) || angleListSymbol.contains(newUnit);}
    static bool isValidLength(QString newUnit) {return lengthList.contains(newUnit) || lengthListSymbol.contains(newUnit);}
    static bool isValidTime(QString newUnit) {return timeList.contains(newUnit) || timeListSymbol.contains(newUnit);}
    static bool isValidMass(QString newUnit) {return massList.contains(newUnit) || massListSymbol.contains(newUnit);}
    static const QStringList angleUnitList() {return angleList;}
    static const QStringList lengthUnitList() {return lengthList;}
    static const QStringList timeUnitList() {return timeList;}
    static const QStringList massUnitList() {return massList;}
    static const QString getSymbol(QString newUnit);
    static const QString getLongName(QString symbol);
};

#endif // UNIT_H
