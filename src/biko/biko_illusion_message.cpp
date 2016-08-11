#include "biko_predefine.h"
#include "biko_protobuf_reflect.h"
#include "biko_illusion_message.h"

Illusion_Message::Illusion_Message()
{

}

Illusion_Message::~Illusion_Message()
{

}

int Illusion_Message::initialize(const QString &proto_file,
								 const google::protobuf::Descriptor *table_msg_desc,
						         QStringList &tips_ary)
{
    int ret = 0;
	proto_file_name_ = proto_file;
    table_msg_desc_ = table_msg_desc;
    const int LIST_MESSAGE_ONE_FIELD = 1;

    const google::protobuf::MessageOptions &mo = table_msg_desc_->options();
    const google::protobuf::FieldDescriptor *line_field_desc = table_msg_desc_->field(0);

	fprintf(stdout,
			"Table message desc error. File[%s] table message[%s] field count [%d](must be ==1)."
			"line message type [%s] label [%5] (must be  repeated message)",
			proto_file_name_.toLocal8Bit().toStdString().c_str(),
			table_msg_desc_->full_name().c_str(),
			table_msg_desc_->field_count(),
			line_field_desc->type_name(),
			line_field_desc->label());

    //List mesage里面只有一个字段，
    //这个字段的名字必须是list_data
    //同时他的类型是必须是Message，(Line Message)
    //而且的label 必须是REPEATED
    if (table_msg_desc_->field_count() != LIST_MESSAGE_ONE_FIELD &&
        line_field_desc->type() != google::protobuf::FieldDescriptor::TYPE_MESSAGE &&
        line_field_desc->label() != google::protobuf::FieldDescriptor::LABEL_REPEATED)
    {
		tips_ary.append(QString::fromLocal8Bit("TABLE Message描述错误, File[%1] 表对应 message[%2] 字段数量为[%3](必须为1) "
											   "行对应 message type [%4] label [%5] ，必须是( repeated message)").
						arg(proto_file_name_).
						arg(table_msg_desc_->full_name().c_str()).
						arg(table_msg_desc_->field_count()).
						arg(line_field_desc->type_name()).
						arg(line_field_desc->label()));

        return -1;
    }

    std::string table_messge_name = table_msg_desc_->full_name();
    std::string line_messge_name = line_field_desc->message_type()->full_name();
    std::string excel_filename = mo.GetExtension(illusion::excel_file);
    std::string excel_sheetname = mo.GetExtension(illusion::excel_sheet);
    int fieldsname_line = mo.GetExtension(illusion::fieldsname_line);
    int fullname_line = mo.GetExtension(illusion::fullname_line);
    int read_data_line = mo.GetExtension(illusion::read_line);
    std::string outer_file_name = mo.GetExtension(illusion::outer_file);
	if (outer_file_name.empty())
	{
		outer_file_name = table_msg_desc_->full_name();
	}

	fprintf(stdout,
			"Table message desc error. File[%s] table message[%s] line message [%s].\n"
			"EXCEL file name[%s] sheet [%s] and field name line[%d] full name line[%d] read start line [%d] ",
			proto_file_name_.toLocal8Bit().toStdString().c_str(),
			table_msg_desc_->full_name().c_str(),
			line_field_desc->message_type()->full_name().c_str(),
			excel_filename.c_str(),
			excel_sheetname.c_str(),
			fieldsname_line,
			fullname_line,
			read_data_line);
	//读取参数检查
    if (excel_filename.empty() ||
        excel_sheetname.empty() ||
        0 > fieldsname_line ||
        0 > fullname_line ||
        0 >= read_data_line )
    {
		tips_ary.append(QString::fromLocal8Bit("TABLE Message描述错误, File[%1] 表对应 message[%2] 行对应Message[%3]").
						arg(proto_file_name_).
						arg(table_msg_desc_->full_name().c_str()).
						arg(line_field_desc->message_type()->full_name().c_str()));
		tips_ary.append(QString::fromLocal8Bit("EXCEL文件名称[%1](must be not null),Sheet名称[%2] "
											   "Sheet 中字段名称行[%3] fullname行[%4] 读取数据起始行[%5] (must be >0)").
						arg(excel_filename.c_str()).
						arg(excel_sheetname.c_str()).
						arg(fieldsname_line).
						arg(fullname_line).
						arg(read_data_line));
        return -2;
    }


	//没有进行本地转码，考虑Message 应该都是ascii
    table_message_name_ = table_messge_name.c_str();
    line_message_name_ = line_messge_name.c_str();
    excel_file_name_ = excel_filename.c_str();
    excel_sheet_name_ = excel_sheetname.c_str();

    fieldsname_line_ = fieldsname_line;
    fullname_line_ = fullname_line;
    read_data_line_ = read_data_line;

    outer_file_name_ = outer_file_name.c_str();

    line_msg_desc_ = line_field_desc->message_type();

    ret = recursive_proto(line_msg_desc_);
    if (0 != ret)
    {
        return ret;
    }

    Q_ASSERT(column_fieldname_ary_.size() == column_field_count_);
    Q_ASSERT(column_fullname_ary_.size() == column_field_count_);
    Q_ASSERT(line_field_desc_ary_.size() == column_field_count_);
    return 0;
}


int Illusion_Message::recursive_proto(const google::protobuf::Descriptor *msg_desc)
{
    int ret = 0;
    for (int j = 0; j < msg_desc->field_count(); ++j)
    {
        const google::protobuf::FieldDescriptor *field_desc =
            msg_desc->field(j);
        const google::protobuf::FieldOptions &fo = field_desc->options();

        //如果这个域标识了不读取，跳过
        bool is_read_filed = fo.GetExtension(illusion::cfg_field);
        if (!is_read_filed)
        {
            continue;
        }

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
            //tb_field_desc_ary_ 是保持和读取的EXCEL一样的字段描述的，用于插入，

            //Message需要递归处理
            if (field_desc->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
            {
                const google::protobuf::Descriptor *sub_msg_desc = field_desc->message_type();
                ret = recursive_proto(sub_msg_desc);
                if (0 != ret)
                {
                    return ret;
                }
            }
            else
            {
                QString field_name = QString("%1-%2").arg(fields_name.c_str()).arg(k + 1);
                column_fieldname_ary_.push_back(field_name);
                column_fullname_ary_.push_back(field_desc->full_name().c_str());
                line_field_desc_ary_.push_back(field_desc);
                ++column_field_count_;
            }
        }
    }
    return 0;
}

//
int Illusion_Message::new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
                                       google::protobuf::Message *&table_msg) const
{
    // build a dynamic message by "table_msg_desc_" proto
    const google::protobuf::Message *message = msg_factory->GetPrototype(table_msg_desc_);
    if (!message)
    {
        fprintf(stderr, "DynamicMessageFactory GetPrototype by name [%s] fail.",
                table_msg_desc_->full_name().c_str());
        return -1;
    }

    table_msg = message->New();

    return 0;
}

//!
int Illusion_Message::add_line(google::protobuf::Message *table_msg,
                               std::vector<QString> &line_str_ary,
							   int &error_field_no,
							   const google::protobuf::FieldDescriptor *&error_field_desc) const
{
    int ret = 0;
    google::protobuf::Message *line_message = NULL;
    const google::protobuf::FieldDescriptor *list_field_desc = table_msg_desc_->field(0);
    ret = Protobuf_Reflect_AUX::locate_msgfield(table_msg,
                                                list_field_desc,
                                                line_message,
                                                true);
    if (0 != ret)
    {
        return ret;
    }

    //!每个字段对应的Message，用于方便插入操作处理的临时数据而已(每次都必须更新)，其他地方不用关心
    std::vector<google::protobuf::Message *> line_fieldmsg_ary;
    ret = recursive_msgfield(line_message, &line_fieldmsg_ary);
    if (0 != ret)
    {
        return ret;
    }

    Q_ASSERT(line_str_ary.size() == column_field_count_);

    for (int i = 0; i < column_field_count_; i++)
    {
        ret = Protobuf_Reflect_AUX::set_fielddata(line_fieldmsg_ary[i],
                                                  line_field_desc_ary_[i],
                                                  line_str_ary[i]);
        if (0 != ret)
        {
			error_field_no = i;
			error_field_desc = line_field_desc_ary_[i];
            return ret;
        }
    }

    return 0;
}

//!
int Illusion_Message::recursive_msgfield(google::protobuf::Message *msg,
                                         std::vector<google::protobuf::Message *> *tb_message_ary) const
{
    int ret = 0;
    const google::protobuf::Descriptor *msg_desc = msg->GetDescriptor();
    for (int j = 0; j < msg_desc->field_count(); ++j)
    {

        const google::protobuf::FieldDescriptor *field_desc =
            msg_desc->field(j);
        const google::protobuf::FieldOptions &fo = field_desc->options();

        //如果这个域标识了不读取，跳过
        bool is_read_filed = fo.GetExtension(illusion::cfg_field);
        if (!is_read_filed)
        {
            continue;
        }

        //
        int repeated_num = 1;
        if (field_desc->label() == google::protobuf::FieldDescriptor::LABEL_REPEATED)
        {
            repeated_num = fo.GetExtension(illusion::repeat_size);
			Q_ASSERT(repeated_num > 0);
            if (repeated_num <= 0)
            {
				//理论上不会到这儿，因为前面有过检查
                return -1;
            }
			
        }
        for (int k = 0; k < repeated_num; ++k)
        {
            if (field_desc->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
            {
                google::protobuf::Message *field_message = NULL;
                ret = Protobuf_Reflect_AUX::locate_msgfield(msg,
                                                            field_desc,
                                                            field_message,
                                                            true);
                ret = recursive_msgfield(field_message, tb_message_ary);
                if (ret != 0)
                {
                    return ret;
                }
            }
            else
            {
                tb_message_ary->push_back(msg);
            }
        }
    }
    return 0;
}


