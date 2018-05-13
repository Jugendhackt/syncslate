#include "SyncSlate.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SyncSlate w;
	w.show();
	return a.exec();
}
