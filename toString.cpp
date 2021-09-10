#include <toString.h>
QString toStringD(double number) {
    return QString("%1").arg(number, 0, 'g', 15);
}

QString toStringL(long double number) {
    return QString("%1").arg(number, 0, 'g', 15);

}

QString toStringI(int number) {
    return QString::number(number);
}
