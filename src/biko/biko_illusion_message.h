#pragma once

//===========================================================================================
//
class Illusion_Message
{

public:

    Illusion_Message();
    ~Illusion_Message();

	
    /*!
    * @brief      初始化 Illusion Message
    * @return     int 返回值 == 0 标识成功
    * @param      proto_file 存放Message的文件名称
    * @param      table_msg_desc 表结构的PB 描述信息
    * @param      tips_ary  返回的提示信息
    */
    int initialize(const QString &proto_file,
		           const google::protobuf::Descriptor *table_msg_desc,
			       QStringList &tips_ary);


    /*!
    * @brief      New 一个 table message 就是那个有repeated line message 的message
    * @return     int 返回值 == 0 标识成功
    * @param      msg_factory
    * @param      table_msg
    */
    int new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
                         google::protobuf::Message *&table_msg,
						 QStringList &tips_ary) const;

    //!
    int add_line(google::protobuf::Message *table_msg,
                 std::vector<QString> &line_str_ary,
				 int &error_field_no,
				 const google::protobuf::FieldDescriptor *&error_field_desc) const;



protected:

    //!因为一个message可能有子message，必须递归处理
	int recursive_proto(const google::protobuf::Descriptor *msg_desc,
						QStringList &tips_ary);
    //!
    int recursive_msgfield(google::protobuf::Message *msg,
                           std::vector<google::protobuf::Message *> *tb_message_ary) const;

public:

    //!存放所有表数据的结构描述，就是含有repeated  line message 的结构，
    const google::protobuf::Descriptor *table_msg_desc_;
    //!存放一行数据的结构描述
    const google::protobuf::Descriptor *line_msg_desc_;

    //!表格对应的protobuf的message名称
    QString line_message_name_;

    //!对应的repeat line message 结构的名称，
    QString table_message_name_;

	//!对应的PROTO 文件的名称
	QString proto_file_name_;

    //!表格名称
    QString excel_file_name_;
    //!表格名称
    QString excel_sheet_name_;

	//存放protobuf配置数据的的文件名称
	QString outer_file_name_;

	//!是否存在EXCEL 文件
	bool exist_excel_file_ = false;
	//!EXCEL文件配置是更新的
	bool excelcfg_is_newer_ = false;
	

    //!
    int fieldsname_line_ = 1;
    //!表格的第几行描述字段对应的protobuf
    int fullname_line_ = 2;
    //!表格数据从第几行读取
    int read_data_line_ = 3;
    

    //!对应表格的展开的字段数量，包括repeated,sub message的字段,都递归展开，记算
    int column_field_count_ = 0;

    //!对应表格的展开的字段的名称的数组，用于插入EXCEL表格作为标题
    std::vector<QString>  column_fieldname_ary_;
    //!对应表格的展开的字段的fullname数组，用于插入EXCEL表格作为标题2
    std::vector<QString>  column_fullname_ary_;

    //!字段描述注意字段描述和Message 是对应关系，用于设置处理而已，其他地方不用关心
    std::vector<const google::protobuf::FieldDescriptor *> line_field_desc_ary_;


};


//!
typedef std::vector<const Illusion_Message *> ILLUSION_MESSAGE_ARRAY;

//!
typedef std::map <QString, ILLUSION_MESSAGE_ARRAY> QSTRING_2_ILLUSIONARY_MAP;

//!
typedef std::map <QString, Illusion_Message *> QSTRING_2_ILSMSG_MAP;
