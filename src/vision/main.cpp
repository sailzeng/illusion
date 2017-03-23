#include "stdafx.h"
#include "VisionMainFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QDir mk_cfgdir;
    mk_cfgdir.mkdir(".\\cfg\\");

    VisionMainFrame w;
    VisionMainFrame::set_instance(&w);
    w.show();
    return a.exec();
}
