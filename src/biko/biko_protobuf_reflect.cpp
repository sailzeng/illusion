#include "biko_predefine.h"
#include "biko_protobuf_reflect.h"




//======================================================================================





////
//void Illusion_Protobuf_Reflect::error_info(PROTO_ERROR_ARRAY &error_ary)
//{
//    error_ary = error_collector_.error_array_;
//}

//
//int Illusion_Protobuf_Reflect::new_mesage(const std::string &type_name,
//                                          google::protobuf::Message *&new_msg)
//{
//
//    //根据名称得到结构描述
//    const google::protobuf::Descriptor *proc_msg_desc =
//        protobuf_importer_->pool()->FindMessageTypeByName(type_name);
//    if (!proc_msg_desc)
//    {
//		fprintf(stderr, "Importer DescriptorPool FindMessageTypeByName by name [%s] fail.",
//                type_name.c_str());
//        return -1;
//    }
//
//    // build a dynamic message by "proc_msg_desc" proto
//    const google::protobuf::Message *message = msg_factory_->GetPrototype(proc_msg_desc);
//    if (!message)
//    {
//		fprintf(stderr, "DynamicMessageFactory GetPrototype by name [%s] fail.",
//                type_name.c_str());
//        return -1;
//    }
//
//    new_msg = message->New();
//
//    return 0;
//}




int Protobuf_Reflect_AUX::set_fielddata(google::protobuf::Message *msg,
                                             const google::protobuf::FieldDescriptor *field,
                                             const std::string &set_data)
{
    assert(field);

    const google::protobuf::Reflection *reflection = msg->GetReflection();

    //如果没有设置数据，看看是否是必要字段，而且是否有默认值
    if (set_data.length() == 0)
    {
        if (field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED)
        {
			fprintf(stderr, "set_data is null but field name [%s] label is REQUIRED.",
                    field->full_name().c_str());
            return -1;
        }
        else
        {
            return 0;
        }
    }

    if (field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED ||
        field->label() == google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
    {
        // double, exactly eight bytes on the wire.
        if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
        {
            reflection->SetDouble(msg, field, std::stod(set_data));
        }
        // float, exactly four bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
        {
            reflection->SetFloat(msg, field, std::stof(set_data));
        }
        // int64, varint on the wire.  Negative numbers take 10 bytes.  Use TYPE_SINT32 if negative values are likely.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
        {
            reflection->SetInt64(msg, field, std::stoll(set_data));
        }
        // uint64, exactly eight bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
        {
            reflection->SetUInt64(msg, field, std::stoull(set_data));
        }
        //int32, varint on the wire.Negative numbers take 10 bytes.  Use TYPE_SINT32 if negative values are likely.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
        {
            reflection->SetInt32(msg, field, std::stoi(set_data));
        }
        //
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
        {
            reflection->SetUInt64(msg, field, std::stoull(set_data));
        }
        // uint32, exactly four bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
        {
            reflection->SetUInt32(msg, field, std::stoul(set_data));
        }
        // bool, varint on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
        {
            reflection->SetBool(msg, field, string_to_bool(set_data));
        }
        // UTF-8 text.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
        {
            reflection->SetString(msg, field, set_data);
        }
        // Arbitrary byte array.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
        {
            reflection->SetString(msg, field, set_data);
        }
        // uint32, varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
        {
            reflection->SetUInt32(msg, field, 0);
        }
        // Enum, varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
        {
            const google::protobuf::EnumDescriptor *enum_desc =
                field->enum_type();
            const google::protobuf::EnumValueDescriptor *evalue_desc = enum_desc->FindValueByName(set_data);
            if (evalue_desc)
            {
                reflection->SetEnum(msg, field, evalue_desc);
            }
            else
            {
				fprintf(stderr, "Don't find EnumValueDescriptor by name [%s] in enum [%s].",
                        set_data.c_str(),
                        enum_desc->full_name().c_str());
                return -1;
            }

        }
        // int32, exactly four bytes on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
        {
            reflection->SetInt32(msg, field, std::stoi(set_data));
        }
        // int64, exactly eight bytes on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
        {
            reflection->SetInt64(msg, field, std::stoll(set_data));
        }
        // int32, ZigZag-encoded varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
        {
            reflection->SetInt32(msg, field, std::stoi(set_data));
        }
        // int64, ZigZag-encoded varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
        {
            reflection->SetInt64(msg, field, std::stoll(set_data));
        }
        else
        {
            //不支持的类型，这个地方如果出现TYPE_MESSAGE。也是不正常的。
			fprintf(stderr, "I don't field [%s] support this type.%d %s",
                    field->full_name().c_str(),
                    field->type(),
                    field->type_name());
            assert(false);
            return -1;
        }
    }
    //数组
    else if (field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REPEATED)
    {
        // double, exactly eight bytes on the wire.
        if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
        {
            reflection->AddDouble(msg, field, std::stod(set_data));
        }
        // float, exactly four bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
        {
            reflection->AddFloat(msg, field, std::stof(set_data));
        }
        // int64, varint on the wire.  Negative numbers take 10 bytes.  Use TYPE_SINT32 if negative values are likely.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
        {
            reflection->AddInt64(msg, field, std::stoll(set_data));
        }
        // uint64, exactly eight bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
        {
            reflection->AddUInt64(msg, field, std::stoull(set_data));
        }
        //int32, varint on the wire.Negative numbers take 10 bytes.  Use TYPE_SINT32 if negative values are likely.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
        {
            reflection->AddInt32(msg, field, std::stoi(set_data));
        }
        //
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
        {
            reflection->AddUInt64(msg, field, std::stoull(set_data));
        }
        // uint32, exactly four bytes on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
        {
            reflection->AddUInt32(msg, field, std::stoul(set_data));
        }
        // bool, varint on the wire.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
        {
            reflection->AddBool(msg, field, string_to_bool(set_data));
        }
        // UTF-8 text.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
        {
            reflection->AddString(msg, field, set_data);
        }
        // Arbitrary byte array.
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
        {
            reflection->AddString(msg, field, set_data);
        }
        // uint32, varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
        {
            reflection->AddUInt32(msg, field, 0);
        }
        // Enum, varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
        {
            const google::protobuf::EnumDescriptor *enum_desc =
                field->enum_type();
            const google::protobuf::EnumValueDescriptor *evalue_desc = enum_desc->FindValueByName(set_data);
            if (evalue_desc)
            {
                reflection->AddEnum(msg, field, evalue_desc);
            }
            else
            {
				fprintf(stderr, "Don't find EnumValueDescriptor by name [%s] in enum [%s].",
                        set_data.c_str(),
                        enum_desc->full_name().c_str());
                return -1;
            }

        }
        // int32, exactly four bytes on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
        {
            reflection->AddInt32(msg, field, std::stoi(set_data));
        }
        // int64, exactly eight bytes on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
        {
            reflection->AddInt64(msg, field, std::stoll(set_data));
        }
        // int32, ZigZag-encoded varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
        {
            reflection->AddInt32(msg, field, std::stoi(set_data));
        }
        // int64, ZigZag-encoded varint on the wire
        else if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
        {
            reflection->AddInt64(msg, field, std::stoll(set_data));
        }
        else
        {
			fprintf(stderr, "I don't field [%s] support this type.%d %s",
                    field->full_name().c_str(),
                    field->type(),
                    field->type_name());
            assert(false);
            return -1;
        }
    }
    else
    {
		fprintf(stderr, "I don't field [%s] support this type.%d %s",
                field->full_name().c_str(),
                field->type(),
                field->type_name());
        assert(false);
        return -1;
    }

    return 0;
}

//定位一个子结构
int Protobuf_Reflect_AUX::locate_sub_msg(google::protobuf::Message *msg,
                                              const std::string &submsg_field_name,
                                              bool message_add,
                                              google::protobuf::Message *&sub_msg)
{
    //得到结构的描述和反射
    const google::protobuf::Reflection *reflection = msg->GetReflection();
    const google::protobuf::Descriptor *msg_desc = msg->GetDescriptor();

    const google::protobuf::FieldDescriptor *field = NULL;
    field = msg_desc->FindFieldByName(submsg_field_name);

    //没有找到对应的字段描述
    if (!field)
    {
        return -1;
    }

    if (field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED ||
        field->label() == google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
    {
        //如果是一个子结构
        if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
        {
            sub_msg = reflection->MutableMessage(msg, field, 0);
            if (!sub_msg)
            {
                return -1;
            }
        }
    }
    else if (field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REPEATED)
    {
        // Length-delimited message.
        if (field->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
        {
            //如果要增加一个repeated的数据
            if (message_add)
            {
                sub_msg = reflection->AddMessage(msg, field, NULL);
                if (!sub_msg)
                {
                    return -1;
                }
            }
            //如果不用增加，使用repeated 数组的最后一个数据，
            else
            {
                int ary_size = reflection->FieldSize(*msg, field);
                if (ary_size == 0)
                {
                    return -1;
                }
                sub_msg = reflection->MutableRepeatedMessage(msg, field, ary_size - 1);
                if (!sub_msg)
                {
                    return -1;
                }
            }
        }
    }
    else
    {
        assert(false);
        return -1;
    }

    return 0;
}

int Protobuf_Reflect_AUX::locate_msgfield(google::protobuf::Message * msg, 
											   const google::protobuf::FieldDescriptor * msg_field, 
											   google::protobuf::Message *& sub_msg,
											   bool message_add)
{
	//得到结构的描述和反射
	const google::protobuf::Reflection *reflection = msg->GetReflection();
	const google::protobuf::Descriptor *msg_desc = msg->GetDescriptor();

	//没有找到对应的字段描述
	if (!msg_field)
	{
		return -1;
	}

	if (msg_field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED ||
		msg_field->label() == google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
	{
		//如果是字段是一个普通结构
		if (msg_field->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
		{
			sub_msg = reflection->MutableMessage(msg, msg_field, NULL);
			if (!sub_msg)
			{
				return -1;
			}
		}
	}
	else if (msg_field->label() == google::protobuf::FieldDescriptor::Label::LABEL_REPEATED)
	{
		// Length-delimited message.
		if (msg_field->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
		{
			//如果要增加一个repeated的数据
			if (message_add)
			{
				sub_msg = reflection->AddMessage(msg, msg_field, NULL);
				if (!sub_msg)
				{
					return -1;
				}
			}
			//如果不用增加，使用repeated 数组的最后一个数据，
			else
			{
				int ary_size = reflection->FieldSize(*msg, msg_field);
				if (ary_size == 0)
				{
					return -1;
				}
				sub_msg = reflection->MutableRepeatedMessage(msg, msg_field, ary_size - 1);
				if (!sub_msg)
				{
					return -1;
				}
			}
		}
	}
	else
	{
		assert(false);
		return -1;
	}

	return 0;
}


int Protobuf_Reflect_AUX::get_fielddesc(google::protobuf::Message *msg,
                                             const std::string &full_name,
                                             bool message_add,
                                             google::protobuf::Message *&field_msg,
                                             const google::protobuf::FieldDescriptor *&field_desc)
{
    int ret = 0;

    field_desc = NULL;

    google::protobuf::Message *src_msg = msg;
    field_msg = msg;
    const char *FIELD_SEPARATOR = ".";
    std::vector<std::string> v;
    string_split(full_name, FIELD_SEPARATOR, v);

    size_t level_num = v.size();
    if (level_num > 1)
    {
        for (size_t i = 0; i < level_num - 1; ++i)
        {
            ret = Protobuf_Reflect_AUX::locate_sub_msg(src_msg,
                                                            v[i],
                                                            message_add,
                                                            field_msg);
            if (0 != ret)
            {
                return ret;
            }
            src_msg = field_msg;
        }
    }

    const google::protobuf::Descriptor *msg_desc = field_msg->GetDescriptor();

    field_desc = msg_desc->FindFieldByName(v[level_num - 1]);

    //没有找到对应的字段描述
    if (!field_desc)
    {
        return -1;
    }
    return 0;
}

//根据fullname，也就是 phone_book.number 设置一个Message的field
int Protobuf_Reflect_AUX::set_field(google::protobuf::Message *msg,
                                         const std::string &full_name,
                                         const std::string &set_data,
                                         bool repeated_add)
{
    int ret = 0;
    const google::protobuf::FieldDescriptor *field = NULL;
    google::protobuf::Message *sub_msg = NULL;

    //取得字段的描述
    ret = Protobuf_Reflect_AUX::get_fielddesc(msg,
                                                   full_name,
                                                   repeated_add,
                                                   sub_msg,
                                                   field);
    if (0 != ret)
    {
        return ret;
    }

    //根据描述，设置字段的数据
    ret = Protobuf_Reflect_AUX::set_fielddata(sub_msg, field, set_data);
    if (0 != ret)
    {
        return ret;
    }

    return 0;
}

//用UTF8 的coding 编码方式输出一个MESSAGE的信息，方便查看。
void Protobuf_Reflect_AUX::protobuf_output(const google::protobuf::Message *msg,
                                                std::ostream *out)
{
    const google::protobuf::Descriptor *msg_desc = msg->GetDescriptor();
    const google::protobuf::Reflection *reflection = msg->GetReflection();
    for (int i = 0; i < msg_desc->field_count(); ++i)
    {
        const google::protobuf::FieldDescriptor *field_desc =
            msg_desc->field(i);


        if (field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED ||
            field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
        {
            *out << "\t" << field_desc->full_name() << ":";
            if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
            {
                *out << reflection->GetDouble(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
            {
                *out << reflection->GetFloat(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
            {
                *out << reflection->GetInt64(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
            {
                *out << reflection->GetUInt64(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            {
                *out << reflection->GetInt32(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
            {
                *out << reflection->GetUInt64(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
            {
                *out << reflection->GetUInt32(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            {
                *out << reflection->GetBool(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            {
                std::string utf8_string = reflection->GetString(*msg, field_desc);
                *out << utf8_string << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            {
                const google::protobuf::Message &sub_msg = reflection->GetMessage(*msg, field_desc);
                protobuf_output(&sub_msg, out);
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
            {
                *out << reflection->GetString(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
            {
                *out << reflection->GetUInt32(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            {
                const google::protobuf::EnumValueDescriptor *enum_vale =
                    reflection->GetEnum(*msg, field_desc);

                *out << enum_vale->full_name() << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
            {
                *out << reflection->GetInt32(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
            {
                *out << reflection->GetInt64(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
            {
                *out << reflection->GetInt32(*msg, field_desc) << std::endl;
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
            {
                *out << reflection->GetInt64(*msg, field_desc) << std::endl;
            }
            else
            {
				fprintf(stderr, "I don't field_desc [%s] support this type.%d %s",
                        field_desc->full_name().c_str(),
                        field_desc->type(),
                        field_desc->type_name());
                assert(false);
            }
        }
        else if (field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_REPEATED)
        {
            int field_size = reflection->FieldSize(*msg, field_desc);
            if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedDouble(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedFloat(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt64(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedUInt64(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt32(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedUInt64(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedUInt32(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedBool(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    std::string utf8_string = reflection->GetRepeatedString(*msg, field_desc, j);

                    *out << "\t" << field_desc->full_name() << ":" <<
                        utf8_string << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    const google::protobuf::Message &sub_msg = reflection->GetRepeatedMessage(*msg, field_desc, j);
                    protobuf_output(&sub_msg, out);
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedString(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedUInt32(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    const google::protobuf::EnumValueDescriptor *enum_vale =
                        reflection->GetRepeatedEnum(*msg, field_desc, j);
                    *out << "\t" << field_desc->full_name() << ":" << enum_vale->full_name();
                }

            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt32(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt64(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt32(*msg, field_desc, j) << std::endl;
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    *out << "\t" << field_desc->full_name() << ":" <<
                         reflection->GetRepeatedInt64(*msg, field_desc, j) << std::endl;
                }
            }
            else
            {
				fprintf(stderr, "I don't field_desc [%s] support this type.%d %s",
                        field_desc->full_name().c_str(),
                        field_desc->type(),
                        field_desc->type_name());
                assert(false);
                return;
            }
        }
        else
        {
			assert(false);
        }
    }
    return;
}


void Protobuf_Reflect_AUX::message_set_default(google::protobuf::Message *msg)
{
    const google::protobuf::Descriptor *msg_desc = msg->GetDescriptor();
    const google::protobuf::Reflection *reflection = msg->GetReflection();
    for (int i = 0; i < msg_desc->field_count(); ++i)
    {
        const google::protobuf::FieldDescriptor *field_desc =
            msg_desc->field(i);


        if (field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_REQUIRED ||
            field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
        {
            if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
            {
                reflection->SetDouble(msg, field_desc, field_desc->default_value_double());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
            {
                reflection->SetFloat(msg, field_desc, field_desc->default_value_float());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
            {
                reflection->SetInt64(msg, field_desc, field_desc->default_value_int64());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
            {
                reflection->SetUInt64(msg, field_desc, field_desc->default_value_uint64());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            {
                reflection->SetInt32(msg, field_desc, field_desc->default_value_int32());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
            {
                reflection->SetUInt64(msg, field_desc, field_desc->default_value_uint64());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
            {
                reflection->SetUInt32(msg, field_desc, field_desc->default_value_uint32());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            {
                reflection->SetBool(msg, field_desc, field_desc->default_value_bool());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            {
                reflection->SetString(msg, field_desc, field_desc->default_value_string());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            {
                google::protobuf::Message *sub_msg = reflection->MutableMessage(msg, field_desc, NULL);
                message_set_default(sub_msg);
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
            {
                reflection->SetString(msg, field_desc, field_desc->default_value_string());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
            {
                reflection->SetUInt32(msg, field_desc, field_desc->default_value_uint32());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            {
                reflection->SetEnum(msg, field_desc, field_desc->default_value_enum());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
            {
                reflection->SetInt32(msg, field_desc, field_desc->default_value_int32());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
            {
                reflection->SetInt64(msg, field_desc, field_desc->default_value_int64());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
            {
                reflection->SetInt32(msg, field_desc, field_desc->default_value_int32());
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
            {
                reflection->SetInt64(msg, field_desc, field_desc->default_value_int64());
            }
            else
            {
				fprintf(stderr, "I don't field_desc [%s] support this type.%d %s",
                        field_desc->full_name().c_str(),
                        field_desc->type(),
                        field_desc->type_name());
                assert(false);
            }
        }
        else if (field_desc->label() == google::protobuf::FieldDescriptor::Label::LABEL_REPEATED)
        {
            int field_size = reflection->FieldSize(*msg, field_desc);
            if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedDouble(msg, field_desc, j, field_desc->default_value_double());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FLOAT)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedFloat(msg, field_desc, j, field_desc->default_value_float());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt64(msg, field_desc, j, field_desc->default_value_int64());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedUInt64(msg, field_desc, j, field_desc->default_value_uint64());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_INT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt32(msg, field_desc, j, field_desc->default_value_int32());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedUInt64(msg, field_desc, j, field_desc->default_value_uint64());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_FIXED32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedUInt32(msg, field_desc, j, field_desc->default_value_uint32());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BOOL)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedBool(msg, field_desc, j, field_desc->default_value_bool());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedString(msg, field_desc, j, field_desc->default_value_string());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    google::protobuf::Message *sub_msg = reflection->MutableRepeatedMessage(msg, field_desc, j);
                    message_set_default(sub_msg);
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedString(msg, field_desc, j, field_desc->default_value_string());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedUInt32(msg, field_desc, j, field_desc->default_value_uint32());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_ENUM)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedEnum(msg, field_desc, j, field_desc->default_value_enum());
                }

            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt32(msg, field_desc, j, field_desc->default_value_int32());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt64(msg, field_desc, j, field_desc->default_value_int64());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT32)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt32(msg, field_desc, j, field_desc->default_value_int32());
                }
            }
            else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_SINT64)
            {
                for (int j = 0; j < field_size; ++j)
                {
                    reflection->SetRepeatedInt64(msg, field_desc, j, field_desc->default_value_int64());
                }
            }
            else
            {
				fprintf(stderr, "I don't field_desc [%s] support this type.%d %s",
                        field_desc->full_name().c_str(),
                        field_desc->type(),
                        field_desc->type_name());
                assert(false);
                return;
            }
        }
        else
        {
            assert(false);
        }
    }
    return;
}


bool Protobuf_Reflect_AUX::string_to_bool(const std::string& str)
{

	if (0 == stricmp("TRUE", str.c_str()))
	{
		return true;
	}
	else if (1 == ::atoi(str.c_str()))
	{
		return true;
	}
	return false;
}

void Protobuf_Reflect_AUX::string_split(const std::string &source_str,
						 const std::string &separator,
						 std::vector<std::string> &v)
{

	std::string::const_iterator fs = source_str.begin();
	std::string::const_iterator ls = source_str.end();
	std::string::const_iterator fo = separator.begin();
	std::string::const_iterator lo = separator.end();

	//使用尾部插入的迭代器
	std::back_insert_iterator<std::vector<std::string> > o = std::back_inserter(v);
	if (fo == lo)
	{
		*o = std::vector<std::string>::value_type(fs, ls);
		return;
	}

	// current position old_str in str
	std::string::const_iterator  pos = std::search(fs, ls, fo, lo,
								 std::equal_to<std::string::value_type>());

	if (pos == ls)
	{
		if (fs < ls)
		{
			*o = std::vector<std::string>::value_type(fs, ls);
		}

		return;
	}

	size_t old_size = std::distance(fo, lo);

	for (; pos != ls;)
	{
		// append src string
		*o = std::vector<std::string>::value_type(fs, pos);
		// ignore old_str
		fs = pos + old_size;
		// find next
		pos = std::search(fs, ls, fo, lo, std::equal_to<std::string::value_type>());
	}

	if (fs != ls)
	{
		*o = std::vector<std::string>::value_type(fs, ls);
	}

	return;
}