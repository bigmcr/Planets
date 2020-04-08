#include "scalarUnits.h"
#include <cstdio>
#include <QTextStream>
#include "precision.h"
#include "toString.h"

scalarUnits::scalarUnits(long double newValue, QString newUnit)
{
    value = newValue;
    QList<QString> empty;
    unit.append(empty);           // numerators
    unit.append(empty);           // denominators
    unit[numerator].append(newUnit);
    unit[denominator].append("");
}

scalarUnits::scalarUnits(long double newValue, QList<QList<QString> > newUnit)
{
    value = newValue;
    unit = newUnit;
}

QList<QList<QString> > scalarUnits::simplifyUnits()
{
    for (int indexNumer = 0; indexNumer < unit[numerator].length(); indexNumer++)
    {
        for (int indexDenom = 0; indexDenom < unit[denominator].length(); indexDenom++)
        {
            if (unit[denominator][indexDenom] == unit[denominator][indexNumer])
            {
                unit[numerator].removeAt(indexNumer);
                unit[denominator].removeAt(indexDenom);
                indexNumer--;
                indexDenom--;
            }
        }
    }
    return unit;
}

bool scalarUnits::unitsEqual(QList<QList<QString> > newUnit) const
{
    if ((unit[numerator][0] == "") || (unit[denominator][0] == "")) return false;
    if (newUnit.length() != unit.length()) return false;
    if (newUnit[numerator].length() != unit[numerator].length()) return false;
    if (newUnit[denominator].length() != unit[denominator].length()) return false;

    for (int index = 0; index < newUnit[numerator].length(); index++) {
        if (newUnit[numerator][index] != unit[numerator][index]) return false;
    }

    for (int index = 0; index < newUnit[denominator].length(); index++) {
        if (newUnit[denominator][index] != unit[denominator][index]) return false;
    }

    return true;
}

long double scalarUnits::round(long double number, int digits)
{
    long double multiplier = 1;
    if (digits > 0)
        for (int index = 0; index < digits; index ++)
        {
            multiplier *= 10;
        }
    else {
        for (int index = 0; index > digits; index --)
        {
            multiplier /= 10;
        }
    }
    return roundl(number * multiplier) / multiplier;
}

QString toString(scalarUnits scalar)
{
    QString unitsFinal = scalar.unit[scalarUnits::numerator][0];
    for (int index = 1; index < scalar.unit[scalarUnits::numerator].length(); index++)
    {
        unitsFinal += " * " + scalar.unit[scalarUnits::numerator][index];
    }
    if (scalar.unit[scalarUnits::denominator][0] != "")
    {
        unitsFinal += " / (";
        for (int index = 0; index < scalar.unit[scalarUnits::denominator].length(); index++)
        {
            unitsFinal += " * " + scalar.unit[scalarUnits::denominator][index];
        }
        unitsFinal += ")";
    }
    return toStringD(qRound(static_cast<double>(scalar.getValue()))) + unitsFinal;
}

scalarUnits operator+(const scalarUnits& left, const scalarUnits& right) {
    scalarUnits temp;
    if ( left.unitsEqual(right.getUnit())) {
        temp.unit = left.unit;
        temp.value = left.value + right.value;
    }
    else {
        temp.unit = left.unit;
        temp.value = scalarUnits::errorValue;
        QTextStream stream(stderr);
        stream << "Tried to add " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return temp;
}

scalarUnits operator-(const scalarUnits& left, const scalarUnits& right) {
  return left+(-right);
}

scalarUnits operator*(const scalarUnits& left, const scalarUnits& right) {
    scalarUnits temp;
    if( left.unitsEqual(right.getUnit())) {
        temp.unit = left.unit;
        temp.value = left.value * right.value;
    }
    else {
        temp.unit = left.unit;
        temp.value = scalarUnits::errorValue;
        QTextStream stream(stderr);
        stream << "Tried to multiply " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return temp;
}

scalarUnits operator/(const scalarUnits& left, const scalarUnits& right) {
    scalarUnits temp;
    if( left.unitsEqual(right.unit)) {
        temp.unit = left.unit;
        temp.value = left.value / right.value;
    }
    else {
        temp.unit = left.unit;
        temp.value = scalarUnits::errorValue;
        QTextStream stream(stderr);
        stream << "Tried to divide" + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return temp;
}

bool operator==(const scalarUnits& left, const scalarUnits& right) {
    return ((std::abs(left.value - right.value) < zeroL) && left.unitsEqual(right.unit));
}

bool operator!=(const scalarUnits& left, const scalarUnits& right) {
    return !(left == right);
}

bool operator<(const scalarUnits& left, const scalarUnits& right) {
    if( left.unitsEqual(right.unit)) {
        return left.value < right.value;
    }
    else {
        QTextStream stream(stderr);
        stream << "Tried to compare (<) " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return false;
}

bool operator>(const scalarUnits& left, const scalarUnits& right) {
    if( left.unitsEqual(right.unit)) {
        return left.value > right.value;
    }
    else {
        QTextStream stream(stderr);
        stream << "Tried to compare (>) " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return false;
}

bool operator<=(const scalarUnits& left, const scalarUnits& right) {
    if( left.unitsEqual(right.unit)) {
        return left.value <= right.value;
    }
    else {
        QTextStream stream(stderr);
        stream << "Tried to compare (<=) " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return false;
}

bool operator>=(const scalarUnits& left, const scalarUnits& right) {
    if( left.unitsEqual(right.unit)) {
        return left.value >= right.value;
    }
    else {
        QTextStream stream(stderr);
        stream << "Tried to compare (>=) " + toString(left) + " and " + toString(right) + ", units do not match";
    }
    return false;
}

scalarUnits& scalarUnits::operator+=(const scalarUnits& obj) {
  *this = *this+obj;
  return *this;
}

scalarUnits& scalarUnits::operator-=(const scalarUnits& obj) {
  *this = *this-obj;
  return *this;
}

scalarUnits& scalarUnits::operator*=(const scalarUnits& obj) {
  *this = *this*obj;
  return *this;
}

scalarUnits& scalarUnits::operator/=(const scalarUnits& obj) {
  *this = *this/obj;
  return *this;
}
scalarUnits& scalarUnits::operator++() {
  *this = *this+1;
  return *this;
}

scalarUnits& scalarUnits::operator--() {
  *this = *this-1;
  return *this;
}

scalarUnits scalarUnits::operator+() const {
  return *this;
}

scalarUnits scalarUnits::operator-() const {
  scalarUnits temp;
  temp.value = -temp.value;
  return temp;
}

