#pragma once


#include "biko_qt_excel_engine.h"
#include "biko_illusion_message.h"



//===========================================================================================
//
//
struct ZCE_PROTO_ERROR
{
    std::string file_name_;
    int line_;
    int column_;
    std::string message_;
};

typedef std::vector<ZCE_PROTO_ERROR> PROTO_ERROR_ARRAY;

//错误收集
class ZCE_Error_Collector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
    ZCE_Error_Collector();
    virtual ~ZCE_Error_Collector();

public:

    virtual void AddError(const std::string &filename,
                          int line,
                          int column,
                          const std::string &message);

    void clear_error();

public:
    //
    PROTO_ERROR_ARRAY error_array_;
};

//===========================================================================================
class Illusion_Message;


class Biko_Read_Config
{
protected:


    //!枚举值的对应关系表
    typedef std::map <QString, QString >  MAP_QSTRING_TO_QSTRING;

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
	* @param[in]  proto_dir 根据proto_dir目录下的meta文件反射，
    * @param[in]  excel_dir 读取excel_dir目录下所有的EXCEL文件
    * @param[in]  outer_dir 转换成位置文件输出到outer_dir目录，如果为NULL，则表示用当前目录输出
    */
    int init_read_all(const QString &proto_dir,
                      const QString &excel_dir, 
                      const QString &outer_dir,
                      QStringList &error_tips);

    //!所有的目录都在一个目录下的快捷处理方式
    int init_read_all2(const QString &allinone_dir,
                       QStringList &error_tips);

    //!12312
    void finalize();


	int read_all_message(QStringList &tips_info);


	int read_one_message(const QString &messge_full_name,
						 QStringList &error_tips);

    //清理所有的读取数据
    void clear();


protected:

    //读枚举值
    int read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map);

    /*!
    * @brief
    * @return     int
    * @param      outer_dir
    * @param[out] error_tips 错误信息，
    */
    int init_outdir(const QString &outer_dir,
                    QStringList &error_tips);


    /*!
    * @brief      map proto 文件的目录，同时加载里面所有的.proto文件
    * @return     int
    * @param      proto_dir
    * @param[out] error_tips 错误信息，
    */
    int init_protodir(const QString &proto_dir,
                      QStringList &error_tips);


	int init_exceldir(const QString &excel_dir,
					  QStringList &error_tips);




    //!
    int read_proto_file(const QFileInfo &proto_file,
                        QStringList &error_tips);

	//!
	int open_illusion_excel(const QString &excel_file_name,
							bool read_enum_sheet,
						    QStringList &error_tips);

	//!
	int read_illusion_excel(const Illusion_Message *ils_msg,
							QStringList &error_tips);

protected:

    //单子实例
    static Biko_Read_Config  *instance_;

    //
    static char REPEATED_STRING[];
protected:

	//!proto文件存放的路径
	QDir proto_path_;

    //!EXCEL配置存放的目录
    QDir excel_path_;

    //!日志输出的目录
    QDir out_log_path_;

    //!PBC文件输出的目录路径，PBC protobuf config
    QDir out_pbc_path_;


    //!Excel的处理对象,EXCEL的处理类
    QtAxExcelEngine ils_excel_file_;


    //!
    google::protobuf::compiler::Importer *protobuf_importer_ = NULL;
    //!
    google::protobuf::compiler::DiskSourceTree *source_tree_ = NULL;
    //
    google::protobuf::DynamicMessageFactory *msg_factory_ = NULL;
    //
    ZCE_Error_Collector error_collector_;

    //!每个分析的Message分析得到Illusion_Message，在这儿保存他们
    std::vector <const Illusion_Message *> illusion_msg_ary_;
	//!outer输出文件对用的proto配置的信息
	std::map <QString, const Illusion_Message *> msgname_2_illusion_map_;
    
    //!proto 名称对应
    std::map <QString, ILLUSION_MESSAGE_ARRAY> proto_cfg_map_;
	//!EXCEL 文件名称对应proto配置的信息
	std::map <QString, ILLUSION_MESSAGE_ARRAY> excel_cfg_map_;

    //!outer输出文件对用的proto配置的信息
    std::map <QString, const Illusion_Message *> outer_cfg_map_;

    //!EXCEL文件列表
    QFileInfoList excel_fileary_;

    //!proto meta文件列表
    QFileInfoList proto_fileary_;


};


