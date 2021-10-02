#include "unit.h"

QVector<Unit::singleUnit> Unit::angleUnits = {{"radians","rad",57.2957795130823},
                                              {"degrees","deg",1},
                                              {"grads","grad",0.9}};

QVector<Unit::singleUnit> Unit::lengthUnits = {{"attometer","am",0.000000000000000001},
                                               {"femtometer","fm",0.000000000000001},
                                               {"picometer","pm",0.000000000001},
                                               {"bohr","a0",5.2917721092E-11},
                                               {"angstrom","a",0.0000000001},
                                               {"nanometer","nm",0.000000001},
                                               {"micrometer","μm",0.000001},
                                               {"mil","mil",0.0000254},
                                               {"line","ln",0.002116},
                                               {"inch","in",0.0254},
                                               {"foot","ft",0.3048},
                                               {"meter","m",1},
                                               {"fathom","ftm",1.8288},
                                               {"chain","ch",20.11684},
                                               {"furlong","fur",201.168},
                                               {"kilometer","km",1000},
                                               {"mile","mi",1609.344},
                                               {"nautical mile","nm",1852},
                                               {"league","lea",4828},
                                               {"nautical league","nl",5556},
                                               {"light-second","l-s",299792458},
                                               {"light-minute","l-min",17987547480},
                                               {"astronomical unit","au",149597870700},
                                               {"light-hour","l-hr",1079252848800},
                                               {"light-day","l-d",25902068371200},
                                               {"light-year","ly",9460730472580800},
                                               {"parsec","pc",30856775814913700}};

QVector<Unit::singleUnit> Unit::massUnits = {{"electronvolt","ev",1.78266184E-36},
                                             {"gamma","γ",0.000001},
                                             {"grain","gr",0.00006479891},
                                             {"carat","ct",0.0002},
                                             {"carat","kt",0.0002051965483},
                                             {"pennyweight","pwt",0.00155517384},
                                             {"dram","dr av",0.0017718451953125},
                                             {"ounce","oz av",0.028349523125},
                                             {"pound","lb",0.45359237},
                                             {"kilogram","kg",1},
                                             {"stone","st",6.35029318},
                                             {"kip","kip",453.59237},
                                             {"ton, short","sh tn",907.18474},
                                             {"tonne (mts unit)","t",1000},
                                             {"ton, long","ton",1016.0469088},
                                             {"earth mass","m_e",5.972E+24},
                                             {"jupiter mass","m_j",1.898E+27},
                                             {"solar mass","m_s",1.989E+30}};

QVector<Unit::singleUnit> Unit::timeUnits = {{"second","s",1},
                                             {"minute","min",60},
                                             {"hour","hr",3600},
                                             {"day","day",86400},
                                             {"week","wk",604800},
                                             {"month","mon",2592000},
                                             {"year","y",31557600},
                                             {"decade","dec",315576000},
                                             {"century","cent",3155760000}};

Unit::Unit()
{

}

bool Unit::isValidAngle(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < angleUnits.length(); i++) returnMe = returnMe || (angleUnits.at(i).name == newUnit) || (angleUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidAngleName(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < angleUnits.length(); i++) returnMe = returnMe || (angleUnits.at(i).name == newUnit);
    return returnMe;
}

bool Unit::isValidAngleSymbol(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < angleUnits.length(); i++) returnMe = returnMe || (angleUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidLength(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < lengthUnits.length(); i++) returnMe = returnMe || (lengthUnits.at(i).name == newUnit) || (lengthUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidLengthName(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < lengthUnits.length(); i++) returnMe = returnMe || (lengthUnits.at(i).name == newUnit);
    return returnMe;
}

bool Unit::isValidLengthSymbol(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < lengthUnits.length(); i++) returnMe = returnMe || (lengthUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidMass(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < massUnits.length(); i++) returnMe = returnMe || (massUnits.at(i).name == newUnit) || (massUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidMassName(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < massUnits.length(); i++) returnMe = returnMe || (massUnits.at(i).name == newUnit);
    return returnMe;
}

bool Unit::isValidMassSymbol(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < massUnits.length(); i++) returnMe = returnMe || (massUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidTime(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < timeUnits.length(); i++) returnMe = returnMe || (timeUnits.at(i).name == newUnit) || (timeUnits.at(i).symbol == newUnit);
    return returnMe;
}

bool Unit::isValidTimeName(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < timeUnits.length(); i++) returnMe = returnMe || (timeUnits.at(i).name == newUnit);
    return returnMe;
}

bool Unit::isValidTimeSymbol(QString newUnit)
{
    bool returnMe = false;
    for (int i = 0; i < timeUnits.length(); i++) returnMe = returnMe || (timeUnits.at(i).symbol == newUnit);
    return returnMe;
}

const QStringList Unit::angleUnitList()
{
    QStringList returnMe;
    for (int i = 0; i < angleUnits.length(); i++) returnMe << angleUnits.at(i).name;
    return returnMe;
}

const QStringList Unit::lengthUnitList()
{
    QStringList returnMe;
    for (int i = 0; i < lengthUnits.length(); i++) returnMe << lengthUnits.at(i).name;
    return returnMe;
}

const QStringList Unit::massUnitList()
{
    QStringList returnMe;
    for (int i = 0; i < massUnits.length(); i++) returnMe << massUnits.at(i).name;
    return returnMe;
}

const QStringList Unit::timeUnitList()
{
    QStringList returnMe;
    for (int i = 0; i < timeUnits.length(); i++) returnMe << timeUnits.at(i).name;
    return returnMe;
}

const QStringList Unit::angleSymbolList()
{
    QStringList returnMe;
    for (int i = 0; i < angleUnits.length(); i++) returnMe << angleUnits.at(i).symbol;
    return returnMe;
}

const QStringList Unit::lengthSymbolList()
{
    QStringList returnMe;
    for (int i = 0; i < lengthUnits.length(); i++) returnMe << lengthUnits.at(i).symbol;
    return returnMe;
}

const QStringList Unit::massSymbolList()
{
    QStringList returnMe;
    for (int i = 0; i < massUnits.length(); i++) returnMe << massUnits.at(i).symbol;
    return returnMe;
}

const QStringList Unit::timeSymbolList()
{
    QStringList returnMe;
    for (int i = 0; i < timeUnits.length(); i++) returnMe << timeUnits.at(i).symbol;
    return returnMe;
}

const QString Unit::getSymbol(QString name) {
    if (isValidAngleName(name)) return angleSymbolList().at(angleUnitList().indexOf(name));
    else if (isValidLengthName(name)) return lengthSymbolList().at(lengthUnitList().indexOf(name));
    else if (isValidMassName(name)) return massSymbolList().at(massUnitList().indexOf(name));
    else if (isValidTimeName(name)) return timeSymbolList().at(timeUnitList().indexOf(name));
    return "";
}

const QString Unit::getLongName(QString symbol) {
    if (isValidAngleSymbol(symbol)) return angleUnitList().at(angleSymbolList().indexOf(symbol));
    else if (isValidLengthSymbol(symbol)) return lengthUnitList().at(lengthSymbolList().indexOf(symbol));
    else if (isValidMassSymbol(symbol)) return massUnitList().at(massSymbolList().indexOf(symbol));
    else if (isValidTimeSymbol(symbol)) return timeUnitList().at(timeSymbolList().indexOf(symbol));
    return "";
}

long double Unit::getRatio(QString symbolOrUnit) {
    for (int i = 0; i < angleUnits.length(); i++) if (symbolOrUnit == angleUnits.at(i).name || symbolOrUnit == angleUnits.at(i).symbol) return angleUnits.at(i).ratioToBase;
    for (int i = 0; i < lengthUnits.length(); i++) if (symbolOrUnit == lengthUnits.at(i).name || symbolOrUnit == lengthUnits.at(i).symbol) return lengthUnits.at(i).ratioToBase;
    for (int i = 0; i < massUnits.length(); i++) if (symbolOrUnit == massUnits.at(i).name || symbolOrUnit == massUnits.at(i).symbol) return massUnits.at(i).ratioToBase;
    for (int i = 0; i < timeUnits.length(); i++) if (symbolOrUnit == timeUnits.at(i).name || symbolOrUnit == timeUnits.at(i).symbol) return timeUnits.at(i).ratioToBase;
    return 0.0;
}
