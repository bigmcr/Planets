#ifndef UNIT_H
#define UNIT_H
#include <QStringList>

class Unit
{
private:
    struct singleUnit {QString name, symbol; long double ratioToBase;};
    static QVector<singleUnit> angleUnits, lengthUnits, timeUnits, massUnits;
public:
    Unit();
    static bool isValidAngle(QString newUnit);
    static bool isValidAngleName(QString newUnit);
    static bool isValidAngleSymbol(QString newUnit);
    static bool isValidLength(QString newUnit);
    static bool isValidLengthName(QString newUnit);
    static bool isValidLengthSymbol(QString newUnit);
    static bool isValidTime(QString newUnit);
    static bool isValidTimeName(QString newUnit);
    static bool isValidTimeSymbol(QString newUnit);
    static bool isValidMass(QString newUnit);
    static bool isValidMassName(QString newUnit);
    static bool isValidMassSymbol(QString newUnit);
    static const QStringList angleUnitList();
    static const QStringList lengthUnitList();
    static const QStringList timeUnitList();
    static const QStringList massUnitList();
    static const QStringList angleSymbolList();
    static const QStringList lengthSymbolList();
    static const QStringList timeSymbolList();
    static const QStringList massSymbolList();
    static const QString getSymbol(QString name);
    static const QString getLongName(QString symbol);
    static long double getRatio(QString symbolOrUnit);
};

#endif // UNIT_H
