#pragma warning ( push )
#pragma warning ( disable : 4702)
#pragma warning ( disable : 4267)
#pragma warning ( disable : 4996)

#include <biko_predefine.h>
#include <biko_read_config.h>
#include <biko_getopt.h>

#include <QtCore/QCoreApplication>


#include "game_config_01.pb.h"
#include "game_config_02.pb.h"

#pragma warning ( pop )

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	int ret = 0;

	HANDLE handle_out = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle_out == INVALID_HANDLE_VALUE)
	{
	    return -1;
	}
	//设置屏幕缓冲区和输出屏幕大小
	
	COORD coord = { 120,  300};
	BOOL bret = ::SetConsoleScreenBufferSize(handle_out, coord);
	if (bret == FALSE)
	{
		DWORD ret_error= ::GetLastError();
		//return -1;
	}
	SMALL_RECT rect = { 0, 0, 119, 59 };
	bret = ::SetConsoleWindowInfo(handle_out, TRUE, &rect);
	if (bret == FALSE)
	{
		DWORD ret_error = ::GetLastError();
	    //return -1;
	}

	std::ifstream if_strem;
	if_strem.open("E:\\Courage\\illusion.git\\example\\001\\outer\\GAME_CFG_STRUCT_1.bin");

	LIST_OF_GAME_CFG_STRUCT_1 game_cfg_1;
	bool succ = game_cfg_1.ParseFromIstream(&if_strem);
	if (!succ)
	{
		fprintf(stdout,"Read fail.\n" );
	}
	std::cout << game_cfg_1.list_data(0).s1_h() << std::endl;
	std::cout << game_cfg_1.list_data(1).s1_h() << std::endl;
	QString out_string = game_cfg_1.list_data(1).s1_h().c_str();
	std::cout << out_string.toLocal8Bit().toStdString().c_str() << std::endl;
	return a.quit();
}

