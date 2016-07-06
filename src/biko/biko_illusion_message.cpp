#include "biko_predefine.h"
#include "biko_protobuf_reflect.h"
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

    const google::protobuf::MessageOptions &mo = table_msg_desc_->options();

    const google::protobuf::FieldDescriptor *list_field_desc = table_msg_desc_->field(0);
    //List mesage里面只有一个字段，
    //这个字段的名字必须是list_data
    //同时他的类型是必须是Message，(Line Message)
    //而且的label 必须是REPEATED
    if (table_msg_desc_->field_count() != LIST_MESSAGE_ONE_FIELD &&
        list_field_desc->type() != google::protobuf::FieldDescriptor::TYPE_MESSAGE &&
        list_field_desc->label() != google::protobuf::FieldDescriptor::LABEL_REPEATED)
    {
        //有错误
        return -1;
    }

    std::string table_messge_name = table_msg_desc_->full_name();
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

    table_messge_name_ = table_messge_name.c_str();
    line_messge_name_ = line_messge_name.c_str();
    excel_file_name_ = excel_filename.c_str();
    excel_sheetname_ = excel_sheetname.c_str();

    fieldsname_line_ = fieldsname_line;
    fullname_line_ = fullname_line;
    read_data_line_ = read_data_line;

    outer_file_name_ = outer_file_name.c_str();

    line_msg_desc_ = list_field_desc->message_type();

    ret = recursive_proto(line_msg_desc_);
    if (0 != ret)
    {
        return ret;
    }

    Q_ASSERT(tb_fieldname_ary_.size() == tb_field_count_);
    Q_ASSERT(tb_fullname_ary_.size() == tb_field_count_);
    Q_ASSERT(tb_field_desc_ary_.size() == tb_field_count_);
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
        bool is_read_filed = fo.GetExtension(illusion::illusion_field);
        if (is_read_filed)
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
                tb_fieldname_ary_.push_back(field_name);
                tb_fullname_ary_.push_back(field_desc->full_name().c_str());
                tb_field_desc_ary_.push_back(field_desc);
                ++tb_field_count_;
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
                               std::vector<std::string> &line_str_ary) const
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
    std::vector<google::protobuf::Message *> tb_message_ary;
    ret = recursive_msgfield(line_message, &tb_message_ary);
    if (0 != ret)
    {
        return ret;
    }

    Q_ASSERT(line_str_ary.size() == tb_field_count_);

    for (size_t i = 0; i < tb_field_count_; i++)
    {
        ret = Protobuf_Reflect_AUX::set_fielddata(tb_message_ary[i],
                                                  tb_field_desc_ary_[i],
                                                  line_str_ary[i]);
        if (0 != ret)
        {
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
        bool is_read_filed = fo.GetExtension(illusion::illusion_field);
        if (is_read_filed)
        {
            continue;
        }

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


int Illusion_Message::save_to_protocfg(const google::protobuf::Message *table_msg,
                                       const QDir &out_pbc_path,
                                       QString &error_tips) const
{
    QString pbc_file = out_pbc_path.path() + "/";
    pbc_file += outer_file_name_;
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
    if (!table_msg->IsInitialized())
    {
        fprintf(stderr, "class [%s] protobuf encode fail, IsInitialized return false.error string [%s].",
                table_msg->GetTypeName().c_str(),
                table_msg->InitializationErrorString().c_str());
        return -1;
    }
    std::string bin_string, txt_string;
    bool succ = table_msg->SerializeToString(&bin_string);
    if (!succ)
    {
        return -2;
    }
    succ = google::protobuf::TextFormat::PrintToString(*table_msg, &txt_string);
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



