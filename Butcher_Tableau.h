#ifndef BUTCHER_TABLEAU_H
#define BUTCHER_TABLEAU_H
#include <QVector>
#include <Point3D.h>

class butcherTableau
{
public:
    QVector<Point3D::dataType> cList, CTList, bList, bPrimeList;
    QVector<QVector<Point3D::dataType>> aList;
    int stages;
    int order;
    butcherTableau() {
        stages = 0;
        order = 0;
    }
    bool isValid() {
        bool returnMe = aList.length() == stages;
        returnMe = returnMe || aList.at(0).length() == stages - 1;
        returnMe = returnMe || bList.length() == stages;
        returnMe = returnMe || bPrimeList.length() == stages;
        returnMe = returnMe || CTList.length() == stages;
        returnMe = returnMe || cList.length() == stages;
        returnMe = returnMe || order >= stages;
        return returnMe;
    }
    QVector<Point3D::dataType> calcCT() {
        if (!isValid()) return QVector<Point3D::dataType>();
        QVector<Point3D::dataType> returnMe;
        returnMe.reserve(bList.size());
        for (int i = 0; i < bList.length(); i++) {
            returnMe.append(bPrimeList.at(i) - bList.at(i));
        }
        return returnMe;
    }
};

#endif // BUTCHER_TABLEAU_H
