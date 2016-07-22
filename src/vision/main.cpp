#include "stdafx.h"
#include "VisionMainFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VisionMainFrame w;
	w.show();
	return a.exec();
}
