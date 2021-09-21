#include "unit.h"
QStringList Unit::angleList = QStringList() << QString("radians")
                                            << QString("degrees")
                                            << QString("grads");
QStringList Unit::angleListSymbol = QStringList() << QString("rad")
                                                  << QString("deg")
                                                  << QString("grad");

QStringList Unit::lengthList = QStringList() << QString("attometer")
                                             << QString("femtometer")
                                             << QString("picometer")
                                             << QString("bohr")
                                             << QString("angstrom")
                                             << QString("nanometer")
                                             << QString("micrometer")
                                             << QString("mil")
                                             << QString("line")
                                             << QString("inch")
                                             << QString("foot")
                                             << QString("meter")
                                             << QString("fathom")
                                             << QString("chain")
                                             << QString("furlong")
                                             << QString("kilometer")
                                             << QString("mile")
                                             << QString("nautical mile")
                                             << QString("league")
                                             << QString("nautical league")
                                             << QString("light-second")
                                             << QString("light-minute")
                                             << QString("astronomical unit")
                                             << QString("light-hour")
                                             << QString("light-day")
                                             << QString("light-year")
                                             << QString("parsec");
QStringList Unit::lengthListSymbol = QStringList()  << QString("am")
                                                    << QString("fm")
                                                    << QString("pm")
                                                    << QString("a0")
                                                    << QString("A")
                                                    << QString("nm")
                                                    << QString("μm")
                                                    << QString("mil")
                                                    << QString("ln")
                                                    << QString("in")
                                                    << QString("ft")
                                                    << QString("m")
                                                    << QString("ftm")
                                                    << QString("ch")
                                                    << QString("fur")
                                                    << QString("km")
                                                    << QString("mi")
                                                    << QString("NM")
                                                    << QString("lea")
                                                    << QString("NL")
                                                    << QString("l-s")
                                                    << QString("l-min")
                                                    << QString("au")
                                                    << QString("l-hr")
                                                    << QString("l-d")
                                                    << QString("ly")
                                                    << QString("pc");

QStringList Unit::massList = QStringList()  << QString("electronvolt")
                                            << QString("gamma")
                                            << QString("grain")
                                            << QString("carat")
                                            << QString("carat")
                                            << QString("pennyweight")
                                            << QString("dram")
                                            << QString("ounce")
                                            << QString("pound")
                                            << QString("kilogram")
                                            << QString("stone")
                                            << QString("kip")
                                            << QString("ton, short")
                                            << QString("tonne (mts unit)")
                                            << QString("ton, long")
                                            << QString("Earth mass")
                                            << QString("Jupiter mass")
                                            << QString("Solar mass");
QStringList Unit::massListSymbol = QStringList() << QString("eV")
                                                 << QString("γ")
                                                 << QString("gr")
                                                 << QString("ct")
                                                 << QString("kt")
                                                 << QString("pwt")
                                                 << QString("dr av")
                                                 << QString("oz av")
                                                 << QString("lb")
                                                 << QString("kg")
                                                 << QString("st")
                                                 << QString("kip")
                                                 << QString("sh tn")
                                                 << QString("t")
                                                 << QString("long tn or ton")
                                                 << QString("m_e")
                                                 << QString("m_j")
                                                 << QString("m_s");

QStringList Unit::timeList = QStringList() << QString("second")
                                           << QString("minute")
                                           << QString("hour")
                                           << QString("day")
                                           << QString("week")
                                           << QString("month")
                                           << QString("year");
QStringList Unit::timeListSymbol = QStringList() << QString("s")
                                                 << QString("min")
                                                 << QString("hr")
                                                 << QString("day")
                                                 << QString("wk")
                                                 << QString("mon")
                                                 << QString("y");

Unit::Unit()
{

}

const QString Unit::getSymbol(QString newUnit) {
    if (isValidAngle(newUnit)) return angleListSymbol.at(angleList.indexOf(newUnit));
    else if (isValidLength(newUnit)) return lengthListSymbol.at(lengthList.indexOf(newUnit));
    else if (isValidMass(newUnit)) return massListSymbol.at(massList.indexOf(newUnit));
    else if (isValidTime(newUnit)) return timeListSymbol.at(timeList.indexOf(newUnit));
    return "";
}

const QString Unit::getLongName(QString symbol) {
    if (isValidAngle(symbol)) return angleList.at(angleListSymbol.indexOf(symbol));
    else if (isValidLength(symbol)) return lengthList.at(lengthListSymbol.indexOf(symbol));
    else if (isValidMass(symbol)) return massList.at(massListSymbol.indexOf(symbol));
    else if (isValidTime(symbol)) return timeList.at(timeListSymbol.indexOf(symbol));
    return "";
}
