#include <toString.h>
QString toStringD(double number) {
    return QString::number(number);
}

QString toStringL(long double number) {
    return QString::number(static_cast<double>(number));
}

QString toStringI(int number) {
    return QString::number(number);
}
