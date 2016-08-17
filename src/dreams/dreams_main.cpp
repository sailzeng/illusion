#include <biko_predefine.h>
#include <biko_read_config.h>
#include <biko_getopt.h>

#include <QtCore/QCoreApplication>


int print_cmd_parameter()
{
	fprintf(stdout, "-c,--readconfig                 Read proto and excel file to create config \n");
	fprintf(stdout, "                                to save in outer directory.\n");
	fprintf(stdout, "-n,--newer                      If --readconfig sign ,at the same time sign --newer only .\n");
	fprintf(stdout, "                                read newer excel file.If excel is old than outer file \n");
	fprintf(stdout, "                                don't create outer file.\n");
	fprintf(stdout, "-s,--savetitle                  Save proto message titile to excel file.\n");
	fprintf(stdout, "-a[dir],--allinone=[dir]        Sign all in one directory.\n");
	fprintf(stdout, "                                All in one directory must exist sub directory .\n");
	fprintf(stdout, "                                  required [allinone-dir]/proto/ \n");
	fprintf(stdout, "                                  required [allinone-dir]/excel/ \n");
	fprintf(stdout, "                                  required [allinone-dir]/outer/ \n");
	fprintf(stdout, "                                  optional [allinone-dir]/import/  May be \n");
	fprintf(stdout, "                                  specified multiple times.\n");
	fprintf(stdout, "-p[dir],--proto=[dir]           Sign proto file directory.\n");
	fprintf(stdout, "-x[dir],--excel=[dir]           Sign excel file directory.\n");
	fprintf(stdout, "-o[dir],--outer=[dir]           Sign outer file directory.\n");
	fprintf(stdout, "-i[dir],--import=[dir]          Sign Import file directory.\n");
	fprintf(stdout, "                                May be specified multiple times.");
	fprintf(stdout, "-m[message],--message[message]  Sign only process message full name.\n");
	fprintf(stdout, "-h,--help                       Print out help.\n");
	return 0;
}

//打印所有Tips信息
int print_tips_info(QStringList &tips_ary)
{
	fprintf(stdout, "%s\n", "========================================================");
	fprintf(stdout, "%s\n", "Last Tips info include:");
	for (int i = 0; i < tips_ary.size(); ++i)
	{
		fprintf(stdout,"%s\n",tips_ary[i].toLocal8Bit().toStdString().c_str());
	}
	return 0;
}

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

	QStringList tips_ary;
	const struct option RAPER_LONG_OPTION[]=
	{
		{ "proto", required_argument,NULL, 'p' },
		{ "excel", required_argument, NULL,'x' },
		{ "outer", required_argument,NULL, 'o' },
		{ "import", required_argument,NULL, 'i' },
		{ "message", required_argument,NULL, 'm' },
		{ "newer", required_argument,NULL, 'n' },
		{ "allinone", no_argument,NULL, 'a' },
		{ "readconfg", no_argument,NULL, 'r' },
		{ "savetitle", no_argument,NULL, 's' },
		{ "help", no_argument,NULL, 'h' },
		{ NULL, no_argument, NULL, 0 }
	};

	bool all_in_one = false, read_one_message = false;
	bool save_title = false, read_config = false,print_help = false,read_newer = false;
	QString allinone_dir , messge_full_name, proto_dir, outer_dir, excel_dir;
	QStringList import_list;
	int long_index = 0;
	const char RAPER_OPT_STRING[] = "x:p:o:m:i:a:rsnh";
	int opt = -1;
	while (-1 != (opt = ZCE_LIB::getopt_long(argc, argv,
											 RAPER_OPT_STRING,
											 RAPER_LONG_OPTION,
											 &long_index)))
	{
		switch (opt) 
		{
		case 's':
			save_title = true;
			break;
		case 'r':
			read_config = true;
			break;
		case 'n':
			read_newer = true;
			break;
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
			//如果有多个，就push 多个
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
		case 'h':
		default:
			print_help = true;
			break;
		};
	}

	if (print_help)
	{
		print_cmd_parameter();
		return 0;
	}

#if defined _DEBUG || defined DEBUG
#define DIRECT_DEBUG 1
#if defined DIRECT_DEBUG && DIRECT_DEBUG == 1 
	all_in_one = true;
	read_config = true;
	allinone_dir = "E:\\Courage\\illusion.git\\example\\001";
#endif
#endif

	if (all_in_one)
	{
		if (allinone_dir.isEmpty())
		{
			fprintf(stdout, "Read all in one dir nedd parameter -a[dir] or --allinone[dir].\n");
			fprintf(stdout, "All in one directory must exist sub directory .\n");
			fprintf(stdout, "required [allinone-dir]/proto/ \n");
			fprintf(stdout, "required [allinone-dir]/excel/ \n");
			fprintf(stdout, "required [allinone-dir]/outer/ \n");
			fprintf(stdout, "optional [allinone-dir]/import/  May be specified multiple times.\n");
			fprintf(stdout, "\n");
			fprintf(stdout, "\n");
			print_cmd_parameter();
			return -1;
		}
		ret = Biko_Read_Config::instance()->init_read_all2(allinone_dir,
														   tips_ary);
	}
	else
	{
		if (proto_dir.isEmpty())
		{
			fprintf(stdout, "Process need required parameter proto file directory. -p[dir] or --proto[dir]. \n");
			fprintf(stdout, "Or use all in one parameter to sign all directory. -a[dir] or --allinone[dir]. \n");
			fprintf(stdout, "\n");
			print_cmd_parameter();
			return -1;
		}

		if (excel_dir.isEmpty())
		{
			fprintf(stdout, "Process need required parameter excel file directory. -x[dir] or --excel[dir]. \n");
			fprintf(stdout, "Or use all in one parameter to sign all directory. -a[dir] or --allinone[dir]. \n");
			fprintf(stdout, "\n");
			print_cmd_parameter();
			return -1;
		}

		if (outer_dir.isEmpty())
		{
			fprintf(stdout, "Process need required parameter outer file directory. -o[dir] or --outer[dir]. \n");
			fprintf(stdout, "Or use all in one parameter to sign all directory. -a[dir] or --allinone[dir]. \n");
			fprintf(stdout, "\n");
			print_cmd_parameter();
			return -1;
		}
		//Import list 不是必选参数
		if (import_list.isEmpty())
		{
			fprintf(stdout, "If process need optional parameter import proto file directory. -i[dir] or --import[dir]. \n");
			fprintf(stdout, "Or use all in one parameter to sign all directory. -a[dir] or --allinone[dir]. \n");
		}

		ret = Biko_Read_Config::instance()->init_read_all(proto_dir,
														  excel_dir,
														  outer_dir,
														  import_list,
														  tips_ary);
	}
	if (ret != 0)
	{
		print_tips_info(tips_ary);
		Biko_Read_Config::clean_instance();
		return -1;
	}

	
	if (save_title)
	{
		if (!read_one_message || messge_full_name.isEmpty())
		{
			fprintf(stdout, "To save proto message titile to excel file, need sign message full name.  \n");
			fprintf(stdout, "Use -m[message],--message[message] to sign . \n");
			ret = -1;
		}
		ret = Biko_Read_Config::instance()->save_excel_tablehead(messge_full_name,
																 tips_ary);
	}
	else if (read_config)
	{
		if (read_one_message)
		{
			ret = Biko_Read_Config::instance()->read_one_message(messge_full_name,
																 tips_ary);
		}
		else if (read_newer)
		{
			ret = Biko_Read_Config::instance()->read_newer_message(tips_ary);
		}
		else
		{
			ret = Biko_Read_Config::instance()->read_all_message(tips_ary);
		}
	}
	else
	{
		print_cmd_parameter();
		ret = -1;
	}
	
	Biko_Read_Config::clean_instance();
	if (ret != 0)
	{
		print_tips_info(tips_ary);
		return ret;
	}

	return a.quit();
}

