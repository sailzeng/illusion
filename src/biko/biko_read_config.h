#pragma once

#include "biko_protobuf_reflect.h"
#include "biko_qt_excel_engine.h"

class Biko_Read_Config
{
public:


    struct TABLE_CONFIG
    {
        //!表格名称
        QString excel_table_name_;

        //!表格数据从第几行读取
        int read_data_start_ = 3;

        //!表格对应的protobuf的message名称
        QString pb_line_message_;

        //!表格的第几行描述字段对应的protobuf
        int pb_fieldname_line_ = 2;
		
		//存放protobuf配置数据的的文件名称
		QString save_pb_config_;
		//!对应的repeat line message 结构的名称，
		QString pb_list_message_;

        //!Protobuf item定义的数据
        std::vector<QString>  proto_field_ary_;

        //!假设结构如下，record是一个repeated 的message，
        //!phonebook.master
        //!phonebook.record.name
        //!phonebook.record.tele_number
        //!phonebook.record.email
        //!phonebook.record.name
        //!phonebook.record.tele_number
        //!phonebook.record.email
        //!那么phonebook.record.name出现的位置会被标识为item_msg_firstshow_ 为1
        std::vector<int> item_msg_firstshow_;

        //!在上面的例子  会被记录为phonebook.record.name
        QString firstshow_field_;
        //!在上面的例子 会被记录为phonebook.record
        QString firstshow_msg_;

    };

    //!枚举值的对应关系表
    typedef std::map <QString, QString >  MAP_QSTRING_TO_QSTRING;

    //!
    typedef std::map <QString, TABLE_CONFIG> MAP_TABLE_TO_CONFIG;

    //!
    struct EXCEL_FILE_DATA
    {
        MAP_QSTRING_TO_QSTRING  xls_enum_;

        MAP_TABLE_TO_CONFIG  xls_table_cfg_;
    };

    typedef std::map <QString, EXCEL_FILE_DATA> MAP_FNAME_TO_CFGDATA;

protected: // 仅从序列化创建
    Biko_Read_Config();
protected:
    virtual ~Biko_Read_Config();


public:

    //!
    static Biko_Read_Config *instance();

    //!
    static void clean_instance();

public:

    /*!
    * @brief      初始化读取操作，准备进行目录的批量转换
	* @return     int 
    * @param[in]  excel_dir 读取excel_dir目录下所有的EXCEL文件
    * @param[in]  proto_dir 根据proto_dir目录下的meta文件反射，
    * @param[in]  outer_dir 转换成位置文件输出到outer_dir目录，如果为NULL，则表示用当前目录输出
    */
    int init_read_all(const QString &excel_dir,
                      const QString &proto_dir,
                      const QString *outer_dir,
					  QString &error_tips);

    //!所有的目录都在一个目录下的快捷处理方式
	int init_read_all2(const QString &allinone_dir,
					   QString &error_tips);

	/*!
	* @brief      初始化，准备读取一个EXCEL文件，转换为配置文件
	* @return     int
	* @param      excel_file
	* @param      proto_dir 根据proto_dir目录下的meta文件反射，
	* @param      outer_dir 转换成位置文件输出到outer_dir目录
	* @param      table_name 表名，为NULL标识，全文件进行转换
	* @param[out] error_tips 错误信息，输出参数
	*/
	int init_read_one(const QString &excel_file,
					  const QString *excel_table_name,
					  const QString &proto_dir,
					  const QString *outer_dir,
					  QString &error_tips);

	

    bool init_print_pbc(const QString &pbc_file,
                        const QString &meta_struct_name,
                        const QString &proto_dir,
                        const QString *outer_dir);

    //
    void finalize();


	int read_excel(QString &error_tips);



    //清理所有的读取数据
    void clear();



    //!从DB3文件里面读取某个配置表的配置
    int read_db3_conftable(const std::string &db3_fname,
                           const std::string &conf_message_name,
                           unsigned int table_id,
                           unsigned int index_1 = 0,
                           unsigned int index_2 = 0);

protected:

    //读枚举值
    int read_table_enum(EXCEL_FILE_DATA &file_cfg_data);

    //!
	/*!
	* @brief      读取sheet [TABLE_CONFIG] 的配置
	* @return     int 返回成功与否 == 0标识成功
	* @param      file_cfg_data
	* @param      error_tips
	* @note       
	*/
	int read_table_config(EXCEL_FILE_DATA &file_cfg_data,
						  QString &error_tips);


	int read_sheet_pbcdata(TABLE_CONFIG &table_cfg,
						   google::protobuf::Message *&list_msg,
						   QString &error_tips);


    
	/*!
	* @brief      将数据保存到Proto buf config 配置文件里面
	* @return     int
	* @param      table_cfg
	* @param      line_msg
	* @param      error_tips
	* @note       
	*/
	int save_to_protocfg(const TABLE_CONFIG &table_cfg,
						 const google::protobuf::Message *line_msg,
						 QString &error_tips);

	/*!
	* @brief      读取EXCEL初始化的内部实现，对接几个读取初始化接口
	* @return     int
	*/
	int init_read(const QString &proto_dir,
				  const QString *outer_dir,
				  QString &error_tips);


	/*!
	* @brief      
	* @return     int
	* @param      outer_dir
	* @param[out] error_tips 错误信息，输出参数
	*/
	int init_outdir(const QString *outer_dir,
					QString &error_tips);


	/*!
	* @brief      map proto 文件的目录，同时加载里面所有的.proto文件
	* @return     int
	* @param      proto_dir
	* @param[out] error_tips 错误信息，输出参数
	*/
	int init_protodir(const QString &proto_dir,
					  QString &error_tips);


	/*!
	* @brief
	* @return     int
	* @param      open_file 打开的EXCEL文件名称，
	* @param[out] error_tips 错误信息，输出参数
	*/
	int read_one_excel(const QString &open_file,
					   const QString *excel_table_name,
					   QString &error_tips);

protected:

    //单子实例
    static Biko_Read_Config  *instance_;
	
	//
	static char REPEATED_STRING[];
protected:

	//!EXCEL配置存放的目录
	QDir excel_path_;

	//!proto文件存放的路径
	QDir proto_path_;

    //!日志输出的目录
    QDir out_log_path_;	
	//!PBC文件输出的目录路径，PBC protobuf config
	QDir out_pbc_path_;
    //!DB3文件输出的目录路径，
    QDir out_db3_path_;


    //!Excel的处理对象,EXCEL的处理类
    BikoQtExcelEngine ils_excel_file_;

    //!
    Illusion_Protobuf_Reflect ils_proto_reflect_;

    //!文件对应的配置数据，用于我的查询
    MAP_FNAME_TO_CFGDATA   file_cfg_map_;


	//!EXCEL文件列表
	QFileInfoList excel_fileary_;

	//!proto meta文件列表
	QFileInfoList proto_fileary_;
	
	//!EXCEL表格的名称
	QString excel_table_name_;

};


