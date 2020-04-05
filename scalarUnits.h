#ifndef SCALARUNITS_H
#define SCALARUNITS_H

#include <QString>
#include <toString.h>
#include <QList>
#include <cmath>
#include <ostream>

class scalarUnits
{
    enum {numerator = 0, denominator = 1};
    long double value;
    static constexpr long double errorValue = 0123456.789L;
    QList<QList<QString> > unit;

    QList<QList<QString> > simplifyUnits();
    bool unitsEqual (QList<QList<QString> > newUnit) const;
    long double round(long double number, int digits);

    friend scalarUnits operator+(const scalarUnits& left, const scalarUnits& right);
    friend scalarUnits operator-(const scalarUnits& left, const scalarUnits& right);
    friend scalarUnits operator*(const scalarUnits& left, const scalarUnits& right);
    friend scalarUnits operator/(const scalarUnits& left, const scalarUnits& right);
    friend bool operator==(const scalarUnits& left, const scalarUnits& right);
    friend bool operator!=(const scalarUnits& left, const scalarUnits& right);
    friend bool operator<(const scalarUnits& left, const scalarUnits& right);
    friend bool operator>(const scalarUnits& left, const scalarUnits& right);
    friend bool operator<=(const scalarUnits& left, const scalarUnits& right);
    friend bool operator>=(const scalarUnits& left, const scalarUnits& right);
    friend QString toString(scalarUnits scalar);

public:
    scalarUnits(long double newValue = 0.0L, QString newUnit = "");
    scalarUnits(long double newValue, QList<QList<QString> > newUnit);

    long double getValue() const {return value;}
    QList<QList<QString> > getUnit() const {return unit;}

    scalarUnits setValue(long double newValue) {value = newValue; return *this;}
    scalarUnits setUnit(QList<QList<QString> > newUnit) {unit = newUnit; return *this;}

    scalarUnits& operator+=(const scalarUnits& obj);
    scalarUnits& operator-=(const scalarUnits& obj);
    scalarUnits& operator*=(const scalarUnits& obj);
    scalarUnits& operator/=(const scalarUnits& obj);
    scalarUnits& operator++();
    scalarUnits& operator--();
    scalarUnits operator+() const;
    scalarUnits operator-() const;

};


scalarUnits cos(scalarUnits value) {return scalarUnits(cosl(value.getValue()), value.getUnit());}
scalarUnits sin(scalarUnits value) {return scalarUnits(sinl(value.getValue()), value.getUnit());}
scalarUnits tan(scalarUnits value) {return scalarUnits(tanl(value.getValue()), value.getUnit());}
scalarUnits sqrt(scalarUnits value) {return scalarUnits(sqrtl(value.getValue()), value.getUnit());}

#endif // SCALARUNITS_H
