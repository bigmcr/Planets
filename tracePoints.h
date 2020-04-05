#ifndef TRACEPOINTS_H
#define TRACEPOINTS_H
#include <QColor>
#include <QList>
#include <Point3D.h>

class tracePoints
{
public:
        tracePoints(QList<Point3D> newList, int newOldLength = 0, QColor newColor = Qt::white)
	{
		points = newList;
		numberOfPoints = newOldLength;
		color = newColor;
	}
        void setPoints(QList<Point3D> newList) {points = newList;}
	void setNumberOfOldPoints(int newOldPoints) {numberOfPoints = newOldPoints;}
	void setColor(QColor newColor) {color = newColor;}

        Point3D getPoint(int index) const
        {
            if (index < points.size() && index >= 0)
                return points.at(index);
            else
                return Point3D(0, 0, 0);
	}
	int getOldNumber() const {return numberOfPoints;}
	QColor getColor() const {return color;}

        void addPoint(Point3D newPoint) {points.append(newPoint);}
	void removeLastPoint() {points.removeFirst();}
	void clearPoints() {points.clear();}

	int size() const {return points.size();}
private:
	int numberOfPoints;
    QList<Point3D> points;
	QColor color;
};

#endif // TRACEPOINTS_H
