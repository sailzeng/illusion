#include "raper_predefine.h"




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
	
	COORD coord = { 121,  301};
	BOOL bret = ::SetConsoleScreenBufferSize(handle_out, coord);
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

	QStringList tips_ary;
	QString allinone_path = "E:/Courage/illusion.git/example/001";

	const struct option RAPER_LONG_OPTION[]=
	{
		{ "excel", required_argument, NULL,'x' },
		{ "proto", required_argument,NULL, 'p' },
		{ "outer", required_argument,NULL, 'o' },
		{ "import", required_argument,NULL, 'i' },
		{ "message", required_argument,NULL, 'm' },
		{ "all", no_argument,NULL, 'a' },
		{ "title", no_argument,NULL, 't' },
		{ "help", no_argument,NULL, 'h' },
		{ NULL, no_argument, NULL, 0 }
	};

	bool all_in_one = false, read_one_message = false,save_title = false;
	QString allinone_dir , messge_full_name, proto_dir, outer_dir, excel_dir;
	QStringList import_list;
	int long_index = 0;
	const char RAPER_OPT_STRING[] = "c:x:p:o:m:i:ath";
	int opt = -1;
	while (-1 != (opt = ZCE_LIB::getopt_long(argc, argv,
											 RAPER_OPT_STRING,
											 RAPER_LONG_OPTION,
											 &long_index)))
	{
		switch (opt) 
		{
		case 'x':
			excel_dir = optarg;
			break;
		case 'p':
			proto_dir = optarg;
			break;
		case 'o':
			outer_dir = optarg;
			break;
		case 'i':
			import_list.push_back(optarg);
			break;
		case 'm':
			read_one_message = true;
			messge_full_name = optarg;
			break;
		case 'a':
			all_in_one = true;
			allinone_dir = optarg;
			break;
		case 't':
			save_title = true;
			break;
		};
	}
	ret = Biko_Read_Config::instance()->init_read_all2(allinone_path,
													   tips_ary);
	if (ret != 0)
	{
		Biko_Read_Config::clean_instance();
		return -1;
	}

	//ret = Biko_Read_Config::instance()->save_excel_tablehead("LIST_OF_GAME_CFG_STRUCT_2",
	//														 tips_ary);
	//if (ret != 0)
	//{
	//	Biko_Read_Config::clean_instance();
	//	return -1;
	//}

	ret = Biko_Read_Config::instance()->read_all_message(tips_ary);
	if (ret != 0)
	{
		Biko_Read_Config::clean_instance();
		return -1;
	}
	Biko_Read_Config::clean_instance();


	return a.quit();
}

