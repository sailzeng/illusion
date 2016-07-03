#include "biko_predefine.h"
#include "biko_illusion_message.h"

Illusion_Message::Illusion_Message()
{

}

Illusion_Message::~Illusion_Message()
{

}

int Illusion_Message::init(const google::protobuf::Descriptor *table_msg_desc)
{
	int ret = 0;
	table_msg_desc_ = table_msg_desc;
	const int LIST_MESSAGE_ONE_FIELD = 1;
	const std::string LIST_MESSAGE_FIELD_NAME = "list_data";

	const google::protobuf::MessageOptions& mo = table_msg_desc_->options();

	const google::protobuf::FieldDescriptor *list_field_desc = table_msg_desc_->field(0);
	//List mesage里面只有一个字段，
	//这个字段的名字必须是list_data
	//同时他的类型是必须是Message，(Line Message)
	//而且的label 必须是REPEATED
	if (table_msg_desc_->field_count() != LIST_MESSAGE_ONE_FIELD &&
		list_field_desc->name() != LIST_MESSAGE_FIELD_NAME &&
		list_field_desc->type() != google::protobuf::FieldDescriptor::TYPE_MESSAGE &&
		list_field_desc->label() != google::protobuf::FieldDescriptor::LABEL_REPEATED)
	{
		//有错误
		return -1;
	}

	std::string list_messge_name = table_msg_desc_->full_name();
	std::string line_messge_name = list_field_desc->message_type()->full_name();
	std::string excel_filename = mo.GetExtension(illusion::excel_file);
	std::string excel_sheetname = mo.GetExtension(illusion::excel_sheet);
	int fieldsname_line = mo.GetExtension(illusion::fieldsname_line);
	int fullname_line = mo.GetExtension(illusion::fullname_line);
	int read_data_line = mo.GetExtension(illusion::read_line);
	std::string outer_file_name = mo.GetExtension(illusion::outer_file);

	if (excel_filename.empty() ||
		excel_sheetname.empty() ||
		0 >= fieldsname_line ||
		0 >= fullname_line ||
		0 >= read_data_line ||
		fieldsname_line == fullname_line ||
		fieldsname_line == read_data_line ||
		fullname_line == read_data_line)
	{
		//有错误
		return -2;
	}
	if (outer_file_name.empty())
	{
		outer_file_name = table_msg_desc_->full_name();
	}
	
	list_messge_name_ = list_messge_name.c_str();
	line_messge_name_ = line_messge_name.c_str();
	excel_filename_ = excel_filename.c_str();
	excel_sheetname_ = excel_sheetname.c_str();

	fieldsname_line_ = fieldsname_line;
	fullname_line_ = fullname_line;
	read_data_line_ = read_data_line;

	outer_file_name_ = outer_file_name.c_str();

	line_msg_desc_ = list_field_desc->message_type();

	ret = recursive_proto(line_msg_desc_, tb_field_count_);
	if (0 != ret)
	{
		return ret;
	}
	return 0;
}


int Illusion_Message::recursive_proto(const google::protobuf::Descriptor *msg_desc,
									  int &field_count)
{
	int ret = 0;
	for (int j = 0; j < msg_desc->field_count(); ++j)
	{
		const google::protobuf::FieldDescriptor *field_desc =
			msg_desc->field(j);
		const google::protobuf::FieldOptions& fo = field_desc->options();

		std::string fields_name = fo.GetExtension(illusion::fields_name);
		
		//
		int repeated_num = 1;
		if (field_desc->label() == google::protobuf::FieldDescriptor::LABEL_REPEATED)
		{
			repeated_num = fo.GetExtension(illusion::repeat_size);
			if (repeated_num <= 0)
			{
				return -1;
			}
		}

		for (int k = 0; k < repeated_num; ++k)
		{
			//
			int filed_num = 1;
			//
			if (field_desc->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
			{
				int sub_field_count = 0;
				const google::protobuf::Descriptor *sub_msg_desc = field_desc->message_type();
				ret = recursive_proto(sub_msg_desc,
									  sub_field_count);
				if (0 != ret || 0 == sub_field_count)
				{
					return ret;
				}
				filed_num = sub_field_count;
			}
			else
			{
				QString field_name = QString("%1-%2").arg(fields_name.c_str()).arg(k+1);
				tb_fieldname_ary_.push_back(field_name);
				tb_fieldname_ary_.push_back(field_desc->full_name().c_str());
				tb_field_desc_ary_.push_back(field_desc);
			}
			field_count += filed_num;
		}
	}
	return 0;
}

//!
int Illusion_Message::new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
									   google::protobuf::Message *&table_msg)
{
	return 0;
}

