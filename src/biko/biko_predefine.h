#ifndef POPULOUS_PREDEFINE_H_
#define POPULOUS_PREDEFINE_H_



// 在WINDOWS下和POSIX标准兼容的宏，VS2003以下版本如何，我没有测试，2003以后，Windows对于很多代码宏会使用"_"前缀，
#ifndef _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS  1
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

// 用于告诉VC++，我不强迫使用_s类型的API，_CRT_SECURE_NO_DEPRECATE是_CRT_SECURE_NO_WARNINGS的老版本
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS   1
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  1
#endif

#pragma warning ( push )
#pragma warning ( disable : 4091)

// 重新定义FD_SETSIZE来，要在winsock2.h前面，也请其他人注意
#ifndef FD_SETSIZE
#define FD_SETSIZE   1024
#endif

//有些文件前缀是大写的，看起来怪怪的，但Windows下他就真是大写的。

#include <winsock2.h>
#include <MSWSock.h>
#include <winerror.h>
#include <windows.h>
#include <winnt.h>
#include <winbase.h>
#include <Psapi.h>
#include <windef.h>
#include <WTypes.h>
#include <process.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <crtdbg.h>
#include <io.h>
#include <share.h>
#include <DbgHelp.h>
#include <intrin.h>

#pragma warning ( pop )

// C 头文件
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>

//==================================================================================================
#if defined ZCE_OS_WINDOWS
#pragma warning ( push )
#pragma warning ( disable : 4702)
#pragma warning ( disable : 4267)
#endif

// c++头文件
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <utility>
#include <iostream>
#include <iomanip>
#include <functional>
#include <iomanip>
#include <typeinfo>
#include <algorithm>
#include <memory>
#include <limits>

//hash_map,hash_set的头文件包含处理要麻烦一点

//在VS2008以后，才有unordered_map和unordered_set，所以在这之前，你必须用stlport，
//当然由于stlport的性能强过微软自带的容器，其他版本也建议大家用stlport
#if defined ZCE_OS_WINDOWS && !defined _STLP_CONFIX_H && defined (_MSC_VER) && (_MSC_VER <= 1400)
#error " Please use stlport ,in Visual studio 2005, have not unordered_map and unordered_set ."
#endif

//在VC++2008版本,VC++2005+STLport，GCC 4.6版本以及更早的版本，unordered_map的名字空间是std::tr1
//在VC++2008版本以前(包括),必须实用STLport
//在VC++2008版本后，可以考虑是否实用STLport,如果_STLP_CONFIX_H 被定义了，我认为你有使用
#if (defined ZCE_OS_LINUX && (_GCC_VER <= 40600)) \
    || ( defined ZCE_OS_WINDOWS && (_MSC_VER <= 1400) ) \
    || ( defined ZCE_OS_WINDOWS && (_MSC_VER > 1400) && defined _STLP_CONFIX_H)
#include <unordered_map>
#include <unordered_set>
using std::tr1::unordered_map;
using std::tr1::unordered_set;

//后面的版本都是直接用了std的名字空间
#else
#include <unordered_set>
#include <unordered_map>
using std::unordered_map;
using std::unordered_set;
#endif
//更早的版本其实是支持hash_map和hash_set的头文件的，先我放弃支持了,那个要改一点代码。

#if defined ZCE_OS_WINDOWS
#pragma warning ( pop )
#endif



#pragma warning ( push )
#pragma warning ( disable : 4127)

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>

#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtCore/QVariant>
#include <QtCore/QDir> 

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>

#pragma warning ( pop )




#if defined (ZCE_OS_WINDOWS)
#pragma warning ( push )
#pragma warning ( disable : 4512)
#pragma warning ( disable : 4100)
#elif defined (ZCE_OS_LINUX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/text_format.h>

#if defined (ZCE_OS_WINDOWS)
#pragma warning ( pop )
#elif defined (ZCE_OS_LINUX)
#pragma GCC diagnostic pop
#endif

#endif //


