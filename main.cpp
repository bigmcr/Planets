#include <QApplication>
#include "Planets.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Planets w;
	w.show();

	return a.exec();
}
