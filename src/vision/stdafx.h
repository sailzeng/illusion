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
#include <qt_windows.h>

#include <QtWidgets>
#include <QObject>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QAxBase>
#include <QAxObject>

#include <QTableWidget>
#include <QTableView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDialog>
#include <QKeyEvent>

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