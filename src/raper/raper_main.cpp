#include "raper_predefine.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	QCoreApplication a(argc, argv);
	BOOL bret = false;
	bret = ::SetConsoleOutputCP(936);
	if (bret == FALSE)
	{
	    return -1;
	}

	HANDLE handle_out = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle_out == INVALID_HANDLE_VALUE)
	{
	    return -1;
	}
	//设置屏幕缓冲区和输出屏幕大小
	
	COORD coord = { 121,  301};
	bret = ::SetConsoleScreenBufferSize(handle_out, coord);
	if (bret == FALSE)
	{
		DWORD ret_error= ::GetLastError();
		//
	}
	SMALL_RECT rect = { 0, 0, 120, 60 };
	bret = ::SetConsoleWindowInfo(handle_out, TRUE, &rect);
	if (bret == FALSE)
	{
	    return -1;
	}

	QString error_tips;
	QString allinone_path = "E:/Courage/illusion.git/example/001";

	ret = Biko_Read_Config::instance()->init_read_all2(allinone_path,
													   error_tips);
	if (ret != 0)
	{
		Biko_Read_Config::clean_instance();
		return -1;
	}

	ret = Biko_Read_Config::instance()->read_excel(error_tips);
	if (ret != 0)
	{
		Biko_Read_Config::clean_instance();
		return -1;
	}
	Biko_Read_Config::clean_instance();


	return a.exec();
}

