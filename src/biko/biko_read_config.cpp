#include "biko_predefine.h"
#include "biko_read_config.h"


char Biko_Read_Config::REPEATED_STRING[] = "list_data";

//处理的单子实例
Biko_Read_Config *Biko_Read_Config::instance_ = NULL;

//
Biko_Read_Config::Biko_Read_Config()
{
}


Biko_Read_Config::~Biko_Read_Config()
{
}


Biko_Read_Config *Biko_Read_Config::instance()
{
    if (instance_ == NULL)
    {
        instance_ = new Biko_Read_Config();
    }
    return instance_;
}

void Biko_Read_Config::clean_instance()
{
    if (instance_)
    {
        delete instance_;
        instance_ = NULL;
    }
}

//!所有的目录都在一个目录下的快捷处理方式
int Biko_Read_Config::init_read_all2(const QString &allinone_dir,
										 QString &error_tips)
{
	QString outer_dir = allinone_dir + "/outer";
	return init_read_all(allinone_dir + "/excel",
						 allinone_dir + "/proto",
						 &outer_dir,
						 error_tips);
}

//读取excel_dir目录下所有的EXCEL文件，根据proto_dir目录下的meta文件，反射，转换成位置文件输出到outer_dir目录
int Biko_Read_Config::init_read_all(const QString &excel_dir,
									    const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;
	excel_path_.setPath(excel_dir);
	if (false == excel_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("目录[%1]并不存在，请检查参数。").
			arg(excel_dir);
		return -1;
	}
	//读取.xls , .xlsx 文件
	QStringList filters;
	filters << "*.xls" << "*.xlsx";
	excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (excel_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("目录[%1]下没有任何Excel文件，请检查参数。").
			arg(excel_dir);
		return -1;
	}

	ret = init_read(proto_dir,outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}
    return 0;
}

//初始化，准备读取一个EXCEL文件，转换为配置文件
int Biko_Read_Config::init_read_one(const QString &excel_file,
										const QString *excel_table_name,
										const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;

	QFileInfo file_info(excel_file);
	if (!file_info.exists())
	{
		error_tips = QString::fromLocal8Bit("EXCEL文件[%1]并不存在，请检查。").
			arg(excel_file);
		return -1;
	}
	excel_fileary_ << file_info;
	excel_table_name_.clear();
	if (excel_table_name)
	{
		excel_table_name_ = *excel_table_name;
	}

	ret = init_read(proto_dir, outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}



	return 0;
}

//读取EXCEL导表进行配置
int Biko_Read_Config::init_read(const QString &proto_dir, 
									const QString *outer_dir,
									QString &error_tips)
{
	int ret = 0;
	//
	ret = init_protodir(proto_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	ret = init_outdir(outer_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	bool bret = ils_excel_file_.initialize(false);
	if (false == bret)
	{
		error_tips = QString::fromLocal8Bit("OLE不能启动EXCEL，实用OLE读取EXCEL必须安装了EXCEL。");
		return -1;
	}


	return 0;
}

//初始化.proto文件目录，读取里面所有的proto文件
int Biko_Read_Config::init_protodir(const QString &proto_dir,
									QString &error_tips)
{
	int ret = 0;
	proto_path_.setPath(proto_dir);;
	if (false == proto_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("目录[%1]并不存在，请检查参数。").
			arg(proto_dir);
		return -1;
	}
	//读取.proto 文件
	QStringList filters;
	filters << "*.proto";
	proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (proto_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("目录[%1]下没有任何protobuf meta(.proto)文件，请检查参数。").
			arg(proto_dir);
		return -1;
	}
	ils_proto_reflect_.map_path(proto_path_.path().toStdString());

	//加载所有的.proto 文件
	for (int i = 0; i < proto_fileary_.size(); ++i)
	{

	}
	return 0;
}

int Biko_Read_Config::init_outdir(const QString *outer_dir,
									  QString &error_tips)
{
	QString out_path,path_str;
	if (outer_dir)
	{
		out_path = *outer_dir;
	}
	else
	{
		out_path = ".";
	}
	path_str = out_path;
	path_str += "/log";
	out_log_path_.setPath(path_str);
	if (false == out_log_path_.exists())
	{
		if (false == out_log_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//db3的路径没有可以创建
	path_str = out_path;
	path_str += "/db3";
	out_db3_path_.setPath(path_str);
	if (false == out_db3_path_.exists())
	{
		if (false == out_db3_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//pbc的路径没有可以创建
	path_str = out_path;
	path_str += "/pbc";
	out_pbc_path_.setPath(path_str);
	if (false == out_pbc_path_.exists())
	{
		if (false == out_pbc_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	return 0;
}

//
void Biko_Read_Config::finalize()
{
    clear();

    if (ils_excel_file_.is_open())
    {
        ils_excel_file_.close();
        ils_excel_file_.finalize();
    }

    return;
}

//清理所有的读取数据
void Biko_Read_Config::clear()
{
    file_cfg_map_.clear();
}

//把扫描或者参数的EXCEL文件都进行一次读取
int Biko_Read_Config::read_excel(QString &error_tips)
{
	//
	int ret = 0;
	if (!excel_table_name_.isEmpty() && excel_fileary_.size()== 1)
	{
		return read_one_excel(excel_fileary_[0].filePath(),
							  &excel_table_name_,
							  error_tips);
	}
	else
	{
		//代码写错了？
		Q_ASSERT(excel_table_name_.isEmpty());
		for (int i = 0; i < excel_fileary_.size(); ++i)
		{
			ret = read_one_excel(excel_fileary_[i].filePath(),
								 NULL,
								 error_tips);
			if (0 != ret)
			{
				return ret;
			}
		}
	}
	return 0;
}

//读取一个EXCEL文件，如果制定了表格，只读取特定表格
int Biko_Read_Config::read_one_excel(const QString &open_file,
									     const QString *excel_table_name,
									     QString &error_tips)
{
    clear();

    bool bret = ils_excel_file_.open(open_file);
    //Excel文件打开失败
    if (bret != true)
    {
        return -1;
    }
    //
    qDebug() << "Dream excecl file have sheet num["
             << ils_excel_file_.sheetsCount()
             << "].\n";

    //表格错误
    if (ils_excel_file_.hasSheet("TABLE_CONFIG") == false ||
        ils_excel_file_.hasSheet("ENUM_CONFIG") == false)
    {
        //
        error_tips = QString::fromLocal8Bit("你选择的配置EXCEL不是能读取的配置表[TABLE_CONFIG]"
											" or [ENUM_CONFIG]"
											"，请重现检查后打开。!");
        return -1;
    }

    //file_cfg_map_[open_file] = excel_data;
    EXCEL_FILE_DATA excel_data;
    auto result = file_cfg_map_.insert(std::make_pair(open_file, excel_data));
    if (!result.second)
    {
        return -1;
    }

    //
    EXCEL_FILE_DATA &xls_data = (*result.first).second;
    int ret = read_table_enum(xls_data);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("你选择的配置EXCEL文件中的[ENUM_CONFIG]表不正确，请重现检查后打开。!");
        return ret;
    }

    //
    ret = read_table_config(xls_data, error_tips);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("你选择的配置EXCEL文件中的TABLE_CONFIG表不正确，请重现检查后打开。!");
        return ret;
    }

	if (excel_table_name && ils_excel_file_.hasSheet(*excel_table_name) == false)
	{
		error_tips = QString::fromLocal8Bit("没有一张表格被读取了!您设置的读取表格[%1]应该不存在").
			arg(*excel_table_name);
		return -1;
	}

	bool already_read = false;
    auto iter_tmp = xls_data.xls_table_cfg_.begin();
    for (; iter_tmp != xls_data.xls_table_cfg_.end(); ++iter_tmp)
    {
		//如果标识了table name，只读取这个table
		if (excel_table_name && *excel_table_name != iter_tmp->second.excel_table_name_)
		{
			continue;
		}
		already_read = true;

		if (iter_tmp->second.save_pb_config_.isEmpty() == false)
		{
			google::protobuf::Message *list_msg = NULL;
			ret = read_sheet_pbcdata(iter_tmp->second, list_msg, error_tips);
			if (0 != ret)
			{
				return ret;
			}

			ret = save_to_protocfg(iter_tmp->second, list_msg, error_tips);
			if (0 != ret)
			{
				return ret;
			}
		}
    }
	//如果没有读取,理论上前面检查了表格是否存在
	if (!already_read)
	{
		error_tips = QString::fromLocal8Bit("没有一张表格被读取了!原因不明，我的确不明。");
		return -1;
	}
    return 0;
}

int Biko_Read_Config::read_proto_file(const QString & proto_file,
									  QStringList & tips_info)
{
	int ret = 0;
	const google::protobuf::FileDescriptor *file_desc = NULL;
	ret = ils_proto_reflect_.import_file(proto_file.toStdString(), 
										 file_desc);
	if (ret != 0 || NULL == file_desc )
	{
		return -1;
	}



	//处理所有的Message，找出我们要处理的，
	int msg_count =file_desc->message_type_count();
	for (int i = 0; i < msg_count; ++i)
	{
		const google::protobuf::Descriptor *listmsg_desc = file_desc->message_type(i);
		
		bool illusion_msg = mo.GetExtension(illusion::illusion_message);
		if (illusion_msg && listmsg_desc->field_count() > 0)
		{
			
			
		}
		
	}

	return 0;
}




//读取所有的枚举值
int Biko_Read_Config::read_table_enum(EXCEL_FILE_DATA &file_cfg_data)
{
    //前面检查过了
    bool bret =  ils_excel_file_.loadSheet("ENUM_CONFIG");
    if (bret == false)
    {
        return -1;
    }

    //答应行列
    int row_count = ils_excel_file_.row_count();
    int col_count = ils_excel_file_.column_count();
    qDebug() << "ENUM_CONFIG table have col_count = "
             << col_count
             << " row_count ="
             << row_count
             << "\n";

    //注意行列的下标都是从1开始。
    const long COL_ENUM_KEY = 1;
    const long COL_ENUM_VALUE = 2;
    const QChar ENUM_FIRST_CHAR = '[';

    size_t read_enum = 0;
    //读取所有的行
    for (long i = 1; i <= row_count; ++i)
    {

        long   row_no = i;
        //字符串
        QString enum_key = ils_excel_file_.get_cell(row_no, COL_ENUM_KEY).toString();

        //如果第一个字符是[
        if (enum_key[0] == ENUM_FIRST_CHAR )
        {
            QString enum_vlaue = ils_excel_file_.get_cell(row_no, COL_ENUM_VALUE).toString();
            file_cfg_data.xls_enum_[enum_key] = enum_vlaue;

            ++read_enum;
        }
    }
    return 0;
}


//读取表格配置
int Biko_Read_Config::read_table_config(EXCEL_FILE_DATA &file_cfg_data,
											QString &error_tips)
{
    //前面检查过了
    bool bret = ils_excel_file_.loadSheet("TABLE_CONFIG");
    if (bret == false)
    {
		error_tips = QString::fromLocal8Bit("你选择的配置EXCEL不是能读取的配置表[TABLE_CONFIG]"
											"，请重现检查后打开。!");
        return -1;
    }

    long row_count = ils_excel_file_.row_count();
    long col_count = ils_excel_file_.column_count();
    qDebug() << "TABLE_CONFIG table have col_count = " << col_count << " row_count =" << row_count << "\n";

    //注意行列的下标都是从1开始。
    const int COL_TC_KEY = 1;
    const int COL_TC_VALUE = 2;
	bool info_imperfect = false;
	QString cur_process_sheet;

    for (int row_no = 1; row_no <= row_count; ++row_no)
    {

        QString tc_key = ils_excel_file_.get_cell(row_no, COL_TC_KEY).toString();

        QString temp_value;
        TABLE_CONFIG tc_data;
		
		//发现一个这个名称，就读取一排数据
        if (tc_key == QString::fromLocal8Bit("读取的SHEET名称"))
        {
            tc_data.excel_table_name_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.excel_table_name_.isEmpty())
            {
                continue;
            }
			cur_process_sheet = tc_data.excel_table_name_;
			//如果剩余的数据不够，认为发生错误
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
                break;
            }

            tc_data.read_data_start_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.read_data_start_ <= 0)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]的起始读取行字段错误，行号从1开始。").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

            tc_data.pb_line_message_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.pb_line_message_.isEmpty())
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]的对应的Protobuf 对应的行Message必须填写。").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

            tc_data.pb_fieldname_line_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.pb_fieldname_line_ <= 0)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]的对应的Protobuf 对应的行Message必须填写。").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

			//PB 的配置文件部分，可以为空
			tc_data.save_pb_config_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
			if (tc_data.save_pb_config_.isEmpty())
			{
				error_tips = QString::fromLocal8Bit("SHEET[%1]必须导成导成PB编码文件，必选其一。").
					arg(cur_process_sheet);
				return -1;
			}
			++row_no;
			if (row_no > row_count)
			{
				info_imperfect = true;
				break;
			}
			tc_data.pb_list_message_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
			if (false ==tc_data.save_pb_config_.isEmpty() && true == tc_data.pb_list_message_.isEmpty())
			{
				error_tips = QString::fromLocal8Bit("SHEET[%1]如果要求导出Protobuf文件，必须填写对应的List Message结构名称").
					arg(tc_data.excel_table_name_);
				return -1;
			}
			++row_no;
			//最后一行可以不做检查
			//if (row_no > row_count)
		
            auto result = file_cfg_data.xls_table_cfg_.insert(std::make_pair(tc_data.excel_table_name_, tc_data));
            if (false == result.second)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]的描述重复，请检查").
					arg(cur_process_sheet);
                return -2;
            }
        }
        else
        {
            continue;
        }
    }

	if (info_imperfect)
	{
		error_tips = QString::fromLocal8Bit("SHEET[%1]的描述信息缺失，请检查").
			arg(cur_process_sheet);
		return -3;
	}
	
    return 0;
}




int Biko_Read_Config::read_sheet_pbcdata(TABLE_CONFIG &tc_data, 
											 google::protobuf::Message *&list_msg, 
											 QString & error_tips)
{
	int ret = 0;
	//检查EXCEL文件中是否有这个表格
	if (ils_excel_file_.loadSheet(tc_data.excel_table_name_) == false)
	{
		return -3;
	}

	ret = ils_proto_reflect_.new_mesage(tc_data.pb_list_message_.toStdString(), list_msg);
	if (ret != 0)
	{
		return ret;
	}


	int line_count = ils_excel_file_.row_count();
	int col_count = ils_excel_file_.column_count();
	qDebug() << tc_data.excel_table_name_ <<
		" table have col_count = "
		<< col_count
		<< " row_count ="
		<< line_count
		<< "\n";


	//如果标识了pb字段行等，但其实没有那么多行
	if (tc_data.pb_fieldname_line_ > line_count || tc_data.read_data_start_ > line_count)
	{
		return -4;
	}

	QString field_name_string;
	for (int col_no = 1; col_no <= col_count; ++col_no)
	{
		field_name_string = ils_excel_file_.get_cell(tc_data.pb_fieldname_line_, col_no).toString();
		tc_data.proto_field_ary_.push_back(field_name_string);

		//这段到底在干嘛，我自己也只能说个大概了，因为repeated 的字段需要先add message，
		//所以就必须把一次出现的地方找出来
		int find_pos = tc_data.proto_field_ary_[col_no - 1].lastIndexOf('.');
		if (find_pos != -1)
		{
			if (tc_data.firstshow_field_ == field_name_string)
			{
				tc_data.item_msg_firstshow_.push_back(true);
			}
			else
			{
				if (tc_data.firstshow_msg_.length() > 0 &&
					true == field_name_string.startsWith(tc_data.firstshow_msg_))
				{
					tc_data.item_msg_firstshow_.push_back(false);
				}
				else
				{
					tc_data.firstshow_field_ = field_name_string;
					tc_data.firstshow_msg_.append(field_name_string.constData(),
												  find_pos + 1);
					tc_data.item_msg_firstshow_.push_back(true);
				}
			}
		}
		else
		{
			tc_data.item_msg_firstshow_.push_back(false);
		}

	}

	std::vector<google::protobuf::Message *> field_msg_ary;
	std::vector<const google::protobuf::FieldDescriptor *> field_desc_ary;

	//吧啦吧啦吧啦吧啦吧啦吧啦吧啦，这段啰嗦的代码只是为了搞个日志的名字,EXCEFILENAE_TABLENAME.log
	QString xls_file_name;
	xls_file_name = ils_excel_file_.open_filename();
	QFileInfo xls_fileinfo(xls_file_name);
	QString file_basename = xls_fileinfo.baseName();

	QString log_file_name = file_basename;
	log_file_name += "_";
	log_file_name += tc_data.excel_table_name_;
	log_file_name += ".log";
	QString outlog_filename = out_log_path_.path();
	outlog_filename += "/";
	outlog_filename += log_file_name;

	QFile read_table_log(outlog_filename);
	read_table_log.open(QIODevice::ReadWrite);
	if (!read_table_log.isWritable())
	{
		fprintf(stderr, "Read excel file data log file [%s] open fail.",
				outlog_filename.toStdString().c_str());
		return -1;
	}
	std::stringstream sstr_stream;

	//什么？为啥不用google pb 的debugstring直接输出？为啥，自己考虑
	sstr_stream << "Read excel file:" << xls_file_name.toStdString().c_str() << " line count" << line_count
		<< "column count " << col_count << std::endl;
	sstr_stream << "Read table:" << tc_data.excel_table_name_.toStdString().c_str() << std::endl;

	fprintf(stderr, "Read excel file:%s table :%s start. line count %u column %u.",
			xls_file_name.toStdString().c_str(),
			tc_data.excel_table_name_.toStdString().c_str(),
			line_count,
			col_count);

	QString read_data;
	std::string set_data;

	for (int line_no = tc_data.read_data_start_; line_no <= line_count; ++line_no)
	{
		google::protobuf::Message *line_msg = NULL;
		Illusion_Protobuf_Reflect::locate_sub_msg(list_msg,
												  "list_data",
												  true,
												  line_msg);

		google::protobuf::Message *field_msg = NULL;
		const google::protobuf::FieldDescriptor *field_desc = NULL;
		field_msg_ary.clear();
		field_desc_ary.clear();
		for (int col_no = 1; col_no <= col_count; ++col_no)
		{
			//如果为空表示不需要关注这列
			if (tc_data.proto_field_ary_[col_no - 1].length() == 0)
			{
				field_msg_ary.push_back(NULL);
				field_desc_ary.push_back(NULL);
				continue;
			}

			//取得字段的描述
			ret = Illusion_Protobuf_Reflect::get_fielddesc(line_msg,
														   tc_data.proto_field_ary_[col_no - 1].toStdString(),
														   tc_data.item_msg_firstshow_[col_no - 1] == 1 ? true : false,
														   field_msg,
														   field_desc);
			if (0 != ret)
			{
				fprintf(stderr, "Message [%s] don't find field_desc [%s] field_desc name define in Line/Column[%d/%d(%s)]",
						tc_data.pb_line_message_.toStdString().c_str(),
						tc_data.proto_field_ary_[col_no - 1].toStdString().c_str(),
						tc_data.pb_fieldname_line_,
						col_no,
						BikoQtExcelEngine::column_name(col_no)
				);
				return ret;
			}
			field_msg_ary.push_back(field_msg);
			field_desc_ary.push_back(field_desc);
		}
		fprintf(stderr, "Read line [%d] ", line_no);
		sstr_stream << "Read line:" << line_no << std::endl << "{" << std::endl;

		for (long col_no = 1; col_no <= col_count; ++col_no)
		{
			//如果为空表示不需要关注这列
			if (tc_data.proto_field_ary_[col_no - 1].length() == 0)
			{
				continue;
			}

			//读出EXCEL数据，注意这个地方是根据MFC的编码决定CString数据的编码
			read_data = ils_excel_file_.get_cell(line_no, col_no).toString();

			//取得字段的描述
			field_msg = field_msg_ary[col_no - 1];
			field_desc = field_desc_ary[col_no - 1];

			//如果是string 类型，Google PB之支持UTF8
			if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
			{
				set_data = read_data.toStdString();
			}
			//对于BYTES，
			else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
			{
				set_data = read_data.toLocal8Bit();
			}
			//其他字段类型统一转换为UTF8的编码
			else
			{
				set_data = read_data.toStdString();
			}
			//根据描述，设置字段的数据
			ret = Illusion_Protobuf_Reflect::set_fielddata(field_msg, field_desc, set_data);
			if (0 != ret)
			{
				fprintf(stderr, "Message [%s] field_desc [%s] type [%d][%s] set_fielddata fail. Line,Colmn[%d|%d(%s)]",
						tc_data.pb_line_message_.toStdString().c_str(),
						field_desc->full_name().c_str(),
						field_desc->type(),
						field_desc->type_name(),
						line_no,
						col_no,
						BikoQtExcelEngine::column_name(col_no)
				);
				return ret;
			}

			sstr_stream << "\t" << tc_data.proto_field_ary_[col_no - 1].toStdString().c_str() << ":" << set_data.c_str()
				<< std::endl;
		}

		//如果没有初始化
		if (!line_msg->IsInitialized())
		{
			fprintf(stderr, "Read line [%d] message [%s] is not IsInitialized, please check your excel or proto file.",
					line_no,
					tc_data.pb_line_message_.toStdString().c_str());

			fprintf(stderr, "Read line [%d] message [%s] InitializationErrorString :%s;",
					line_no,
					tc_data.pb_line_message_.toStdString().c_str(),
					line_msg->InitializationErrorString().c_str());
			return -1;
		}

		std::cout << line_msg->DebugString() << std::endl;
	}

	std::string out_string;
	out_string.reserve(64 * 1024 * 1024);
	out_string = sstr_stream.str();

	fprintf(stderr, "\n%s", out_string.c_str());
	read_table_log.write(out_string.c_str(), out_string.length());
	read_table_log.close();
	fprintf(stderr, "Read excel file:%s table :%s end.",
			xls_file_name.toStdString().c_str(),
			tc_data.excel_table_name_.toStdString().c_str());
	return 0;
}



int Biko_Read_Config::save_to_protocfg(const TABLE_CONFIG & table_cfg, 
										   const google::protobuf::Message * line_msg, 
										   QString & error_tips)
{
	QString pbc_file = out_pbc_path_.path() + "/";
	pbc_file += table_cfg.save_pb_config_;
	QString txt_file = pbc_file + ".txt";
	QFile pbc_config(pbc_file);
	pbc_config.open(QIODevice::ReadWrite);
	if (!pbc_config.isWritable())
	{
		return -1;
	}
	QFile txt_config(txt_file);
	txt_config.open(QIODevice::ReadWrite);
	if (!txt_config.isWritable())
	{
		return -1;
	}
	if (!line_msg->IsInitialized())
	{
		fprintf(stderr, "class [%s] protobuf encode fail, IsInitialized return false.error string [%s].",
				line_msg->GetTypeName().c_str(),
				line_msg->InitializationErrorString().c_str());
		return -1;
	}
	std::string bin_string, txt_string;
	bool succ = line_msg->SerializeToString(&bin_string);
	if (!succ)
	{
		return -2;
	}
	succ = google::protobuf::TextFormat::PrintToString(*line_msg, &txt_string);
	if (!succ)
	{
		return -2;
	}
	qint64 wt_len = pbc_config.write(bin_string.c_str(), bin_string.length());
	if (wt_len < 0)
	{
		return -3;
	}
	pbc_config.close();

	wt_len = txt_config.write(txt_string.c_str(), txt_string.length());
	if (wt_len < 0)
	{
		return -4;
	}
	txt_config.close();
	return 0;
}


