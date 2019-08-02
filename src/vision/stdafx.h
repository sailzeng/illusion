#ifndef _STDAFX_H
#define _STDAFX_H

#include <Winsock2.h>

#include <stdio.h>
#include <time.h>
#include <assert.h>

#pragma warning ( push )
#pragma warning ( disable : 4800)

#include <list>
#include <tuple>
#include <deque>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>

#pragma warning ( pop )

#pragma warning ( push )
#pragma warning ( disable : 4127)
#include <QtCore/qt_windows.h>

#include <QtWidgets/QtWidgets>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>
#include <QtCore/QDebug>
#include <QtWidgets/QDialog>
#include <QtGui/QKeyEvent>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <QtMultimedia/QSound>

#pragma warning ( pop )

#pragma warning ( push )
#pragma warning ( disable : 4100)
#pragma warning ( disable : 4512)
#pragma warning ( disable : 4800)
#pragma warning ( disable : 4127)

#include <biko_predefine.h>
#include <biko_read_config.h>
#include <biko_getopt.h>

#pragma warning ( pop )


#endif