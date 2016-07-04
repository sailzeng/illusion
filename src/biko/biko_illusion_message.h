#pragma once

//===========================================================================================
//
class Illusion_Message
{

public:

	Illusion_Message();
	~Illusion_Message();


	int init(const google::protobuf::Descriptor *table_msg_desc);


	//!New 一个 table message 就是那个有repeated line message 的message
	int new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
		                 google::protobuf::Message *&table_msg);

	//!
	int add_line(google::protobuf::Message *table_msg,
				 std::vector<std::string> &line_str_ary);

protected:

	//!因为一个message可能有子message，必须递归处理
	int recursive_proto(const google::protobuf::Descriptor *msg_desc);
	//!
	int recursive_msgfield(google::protobuf::Message *msg);
	
protected:

	//!
	const google::protobuf::Descriptor *table_msg_desc_;
	//!
	const google::protobuf::Descriptor *line_msg_desc_;

	//!表格对应的protobuf的message名称
	QString line_messge_name_;

	//!对应的repeat line message 结构的名称，
	QString list_messge_name_;

	//!表格名称
	QString excel_filename_;
	//!表格名称
	QString excel_sheetname_;

	//!
	int fieldsname_line_ = 1;
	//!表格的第几行描述字段对应的protobuf
	int fullname_line_ = 2;
	//!表格数据从第几行读取
	int read_data_line_ = 3;

	

	//存放protobuf配置数据的的文件名称
	QString outer_file_name_;

	//!对应表格的展开的字段数量，包括repeated,sub message的字段,都递归展开，记算
	size_t tb_field_count_ = 0;

	//!对应表格的展开的字段的名称的数组，用于插入EXCEL表格作为标题
	std::vector<QString>  tb_fieldname_ary_;
	//!对应表格的展开的字段的fullname数组，用于插入EXCEL表格作为标题2
	std::vector<QString>  tb_fullname_ary_;

	//!字段描述注意字段描述和Message 是对应关系，用于设置处理而已，其他地方不用关心
	std::vector<const google::protobuf::FieldDescriptor *> tb_field_desc_ary_;
	//!每个字段对应的Message，用于方便插入操作处理的临时数据而已(每次都必须更新)，其他地方不用关心
	std::vector<google::protobuf::Message *> tb_message_ary_;

};


