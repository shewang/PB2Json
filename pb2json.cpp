#include <stdlib.h>
#include <stdio.h>
#include "pb2json.h"

namespace PB2Json
{
    int ToJson(const Message& message, const FieldDescriptor* pFieldDescriptor, Json::Value& value, string& name_str, map<string, string>& key_map)
    {
        const Reflection* pReflection = message.GetReflection();
        int ret = 0;

        switch (pFieldDescriptor->cpp_type())
        {
            case FieldDescriptor::CPPTYPE_INT32:
            {
                value[name_str] = pReflection->GetInt32(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT32:
            {
                value[name_str] = pReflection->GetUInt32(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_INT64:
            {
                value[name_str] = (Json::Int64)pReflection->GetInt64(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT64:
            {
                value[name_str] = (Json::UInt64)pReflection->GetUInt64(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_STRING:
            {
                value[name_str] = pReflection->GetString(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_BOOL:
            {
                value[name_str] = pReflection->GetBool(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_ENUM:
            {
                const EnumValueDescriptor* pEnumValueDes = NULL;
                pEnumValueDes = (EnumValueDescriptor*)pReflection->GetEnum(message, pFieldDescriptor);
                if (NULL != pEnumValueDes)
                {
                    value[name_str] = pEnumValueDes->number(); 
                }
                else
                {
                    ret = 1;
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_DOUBLE:
            {
                value[name_str] = pReflection->GetDouble(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_FLOAT:
            {
                value[name_str] = pReflection->GetFloat(message, pFieldDescriptor);
                break;
            }
            case FieldDescriptor::CPPTYPE_MESSAGE:
            {
                const Message& tmp = pReflection->GetMessage(message, pFieldDescriptor);
                Json::Value tmp_value;
                if (key_map.size() == 0)
                {
                    ret = ToJsonObj(tmp, tmp_value);
                }
                else
                {
                    ret = ToJsonObjMap(tmp, tmp_value, key_map);
                }

                value[name_str] = tmp_value;
                break;
            }
            default:
            {
                ret = 1;
                break;
            }
        }

        return ret;
    }

    int ToJsonArray(const Message& message, const FieldDescriptor* pFieldDescriptor, Json::Value& value, string& name_str, map<string, string>& key_map)
    {
        const Reflection* pReflection = message.GetReflection();
        Json::Value tmp_value = Json::arrayValue;
        int ret = 0;
        for (int FieldNum = 0; FieldNum < pReflection->FieldSize(message, pFieldDescriptor); FieldNum++)
        {
            switch (pFieldDescriptor->cpp_type())
            {
                case FieldDescriptor::CPPTYPE_INT32:
                {
                    tmp_value.push_back(pReflection->GetRepeatedInt32(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_UINT32:
                {
                    tmp_value.push_back(pReflection->GetRepeatedUInt32(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_INT64:
                {
                    tmp_value.push_back((Json::Int64)pReflection->GetRepeatedInt64(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_UINT64:
                {
                    tmp_value.push_back((Json::UInt64)pReflection->GetRepeatedUInt64(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_STRING:
                {
                    tmp_value.push_back(pReflection->GetRepeatedString(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_BOOL:
                {
                    tmp_value.push_back(pReflection->GetRepeatedBool(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_ENUM:
                {
                    const EnumValueDescriptor* pEnumValueDes = NULL;
                    pEnumValueDes = (EnumValueDescriptor*)pReflection->GetRepeatedEnum(message, pFieldDescriptor, FieldNum);
                    if (NULL == pEnumValueDes)
                    {
                        return 1;
                    }
                    tmp_value.push_back(pEnumValueDes->number()); 
                    break;
                }
                case FieldDescriptor::CPPTYPE_DOUBLE:
                {
                    tmp_value.push_back(pReflection->GetRepeatedDouble(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_FLOAT:
                {
                    tmp_value.push_back(pReflection->GetRepeatedFloat(message, pFieldDescriptor, FieldNum));
                    break;
                }
                case FieldDescriptor::CPPTYPE_MESSAGE:
                {
                    const Message& tmp = pReflection->GetRepeatedMessage(message, pFieldDescriptor, FieldNum);
                    Json::Value t_value;
                    if (key_map.size() == 0)
                    {
                       ret = ToJsonObj(tmp, t_value);
                    }
                    else
                    {
                        ret = ToJsonObjMap(tmp, t_value, key_map);
                    }
                    tmp_value.push_back(t_value);
                    break;
                }
                default:
                {
                    return 1;
                }
            }
            value[name_str] = tmp_value;
        }

        return ret;
    }

    int ToJsonObj(const Message& message, Json::Value& value)
    {
        const Reflection* pReflection = message.GetReflection();
        const FieldDescriptor* pFieldDescriptor = NULL;
        bool bRepeated = false;

        std::vector<const FieldDescriptor*> fields;
        pReflection->ListFields(message, &fields);
        int ret = 0;

        for (size_t i = 0; i < fields.size(); i++)
        {
            pFieldDescriptor = fields[i];
            bRepeated = pFieldDescriptor->is_repeated();
            string name_str = pFieldDescriptor->name();
            map<string, string> key_map;

            if (bRepeated)
            {
                ret += ToJsonArray(message, pFieldDescriptor, value, name_str, key_map);
                continue;
            }

            ret += ToJson(message, pFieldDescriptor, value, name_str, key_map);
        }

        return ret;
    }

    int ToJsonObjMap(const Message& message, Json::Value& value, map<string, string>& key_map)
    {
        const Reflection* pReflection = message.GetReflection();
        const FieldDescriptor* pFieldDescriptor = NULL;
        bool bRepeated = false;

        std::vector<const FieldDescriptor*> fields;
        pReflection->ListFields(message, &fields);
        int ret = 0;

        for (size_t i = 0; i < fields.size(); i++)
        {
            pFieldDescriptor = fields[i];
            bRepeated = pFieldDescriptor->is_repeated();
            string name_str = pFieldDescriptor->containing_type()->name() + "." + pFieldDescriptor->name();

            map<string, string>::iterator it = key_map.find(name_str);
            if (it != key_map.end())
            {
                name_str = it->second;
            }
            else
            {
                name_str = pFieldDescriptor->name();
            }

            if (bRepeated)
            {
                ret += ToJsonArray(message, pFieldDescriptor, value, name_str, key_map);
                continue;
            }

            ret += ToJson(message, pFieldDescriptor, value, name_str, key_map);
        }

        return ret;
    }

    bool check_type(Json::ValueType value_type, FieldDescriptor::CppType type)
    {
        if (value_type == Json::objectValue)
        {
            if (FieldDescriptor::CPPTYPE_MESSAGE == type)
            {
                return true;
            }

            return false;
        }

        if ((value_type == Json::intValue) || (value_type == Json::uintValue))
        {
            if (type <= FieldDescriptor::CPPTYPE_UINT64)
            {
                return true;
            }

            if (type == FieldDescriptor::CPPTYPE_ENUM)
            {
                return true;
            }

            return false;
        }

        if (value_type == Json::stringValue)
        {
            if (type == FieldDescriptor::CPPTYPE_STRING)
            {
                return true;
            }

            return false;
        }

        if (value_type == Json::realValue)
        {
            if ((FieldDescriptor::CPPTYPE_DOUBLE == type) || (FieldDescriptor::CPPTYPE_FLOAT == type))
            {
                return true;
            }

            return false;
        }

        if (value_type == Json::booleanValue)
        {
            if (FieldDescriptor::CPPTYPE_BOOL == type)
            {
                return true;
            }

            return false;
        }

        return false;
    }

    int ToPbRepeated(const Json::Value &value, const FieldDescriptor *pFieldDescriptor, Message &message, map<string, string>& key_map)
    {
        if (!check_type(value[0].type(), pFieldDescriptor->cpp_type()))
        {
            return 1;
        }

        const Reflection *pReflection = message.GetReflection();
        EnumDescriptor      *pEnumDes = NULL;
        EnumValueDescriptor *pEnumValueDes = NULL;
        int ret = 0;
        for(uint32_t i = 0; i < value.size(); i++)
        {
            switch(pFieldDescriptor->cpp_type())
            {
                case FieldDescriptor::CPPTYPE_INT32:
                {
                    pReflection->AddInt32(&message, pFieldDescriptor, value[i].asInt());
                    break;
                }
                case FieldDescriptor::CPPTYPE_UINT32:
                {
                    pReflection->AddUInt32(&message, pFieldDescriptor, value[i].asUInt());
                    break;
                }
                case FieldDescriptor::CPPTYPE_INT64:
                {
                    pReflection->AddInt64(&message, pFieldDescriptor, value[i].asInt64());
                    break;
                }
                case FieldDescriptor::CPPTYPE_UINT64:
                {
                    pReflection->AddUInt64(&message, pFieldDescriptor, value[i].asUInt64());
                    break;
                }
                case FieldDescriptor::CPPTYPE_STRING:
                {
                    pReflection->AddString(&message, pFieldDescriptor, value[i].asString());
                    break;
                }
                case FieldDescriptor::CPPTYPE_BOOL:
                {
                    pReflection->AddBool(&message, pFieldDescriptor, value[i].asBool());
                    break;
                }
                case FieldDescriptor::CPPTYPE_DOUBLE:
                {
                    pReflection->AddDouble(&message, pFieldDescriptor, value[i].asDouble());
                    break;
                }
                case FieldDescriptor::CPPTYPE_FLOAT:
                {
                    pReflection->AddFloat(&message, pFieldDescriptor, value[i].asFloat());
                    break;
                }
                case FieldDescriptor::CPPTYPE_ENUM:
                {
                    if ((pEnumDes = (EnumDescriptor *)pFieldDescriptor->enum_type()) == NULL)
                    {
                        return 1;
                    }

                    if ((pEnumValueDes = (EnumValueDescriptor *)pEnumDes->FindValueByNumber(value[i].asInt())) == NULL)
                    {
                        return 1;
                    }

                    pReflection->AddEnum(&message, pFieldDescriptor, pEnumValueDes);
                    break;
                }
                case FieldDescriptor::CPPTYPE_MESSAGE:
                {
                    Message *pmessage = pReflection->AddMessage(&message, pFieldDescriptor);
                    if (key_map.size() == 0)
                    {
                        ret = ToPb(*pmessage, value[i]);
                    }
                    else
                    {
                        ret = ToPbMap(*pmessage, value[i], key_map);
                    }
                    break;
                }
                default:
                {
                    ret = 1;
                    break;
                }
            }
        }

        return ret;
    }

    int ToPbSingle(const Json::Value &value, const FieldDescriptor *pFieldDescriptor, Message &message, map<string, string>& key_map)
    {
        if (!check_type(value.type(), pFieldDescriptor->cpp_type()))
        {
            return 1;
        }

        const Reflection *pReflection = message.GetReflection();
        EnumDescriptor      *pEnumDes = NULL;
        EnumValueDescriptor *pEnumValueDes = NULL;
        int ret = 0;
        switch(pFieldDescriptor->cpp_type())
        {
            case FieldDescriptor::CPPTYPE_INT32:
            {
                pReflection->SetInt32(&message, pFieldDescriptor, value.asInt());
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT32:
            {
                pReflection->SetUInt32(&message, pFieldDescriptor, value.asUInt());
                break;
            }
            case FieldDescriptor::CPPTYPE_INT64:
            {
                pReflection->SetInt64(&message, pFieldDescriptor, value.asInt64());
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT64:
            {
                pReflection->SetUInt64(&message, pFieldDescriptor, value.asUInt64());
                break;
            }
            case FieldDescriptor::CPPTYPE_STRING:
            {
                pReflection->SetString(&message, pFieldDescriptor, value.asString());
                break;
            }
            case FieldDescriptor::CPPTYPE_BOOL:
            {
                pReflection->SetBool(&message, pFieldDescriptor, value.asBool());
                break;
            }
            case FieldDescriptor::CPPTYPE_DOUBLE:
            {
                pReflection->SetDouble(&message, pFieldDescriptor, value.asDouble());
                break;
            }
            case FieldDescriptor::CPPTYPE_FLOAT:
            {
                pReflection->SetFloat(&message, pFieldDescriptor, value.asFloat());
                break;
            }
            case FieldDescriptor::CPPTYPE_ENUM:
            {
                if ((pEnumDes = (EnumDescriptor *)pFieldDescriptor->enum_type()) == NULL)
                {
                    return 1;
                }

                if ((pEnumValueDes = (EnumValueDescriptor *)pEnumDes->FindValueByNumber(value.asInt())) == NULL)
                {
                    return 1;
                }

                pReflection->SetEnum(&message, pFieldDescriptor, pEnumValueDes);
                break;
            }
            case FieldDescriptor::CPPTYPE_MESSAGE:
            {
                Message *pmessage = pReflection->MutableMessage(&message, pFieldDescriptor);
                if (key_map.size() == 0)
                {
                    ret = ToPb(*pmessage, value);
                }
                else
                {
                    ret = ToPbMap(*pmessage, value, key_map);
                }

                break;
            }
            default:
            {
                ret = 1;
                break;
            }
        }

        return ret;
    }

    int ToPb(Message& message, const Json::Value& value)
    {
        //非object类型，在debug编译选项下可能会导致触发assert，导致进程退出
        if (value.type() != Json::objectValue)
        {
            return 1;
        }
        int ret = 0;
        const Descriptor* pDescriptor = message.GetDescriptor();
        const FieldDescriptor* pFieldDescriptor = NULL;
        bool bRepeated = false;
        map<string, string> key_map;

        for(int i = 0; i < pDescriptor->field_count(); i++)
        {
            string name_str;
            pFieldDescriptor = (FieldDescriptor *)pDescriptor->field(i);
            name_str = pFieldDescriptor->name();
            if (!value.isMember(name_str))
            {
                continue;
            }

            const Json::Value &field = value[name_str.c_str()];

            bRepeated = pFieldDescriptor->is_repeated();
            if ((bRepeated && !field.isArray()) || (!bRepeated && field.isArray()))
            {
                ret += 1;
                continue;
            }

            if (bRepeated)
            {
                ret += ToPbRepeated(field, pFieldDescriptor, message, key_map);
                continue;
            }

            ret += ToPbSingle(field, pFieldDescriptor, message, key_map);
        }

        return ret;
    }
    
    int ToPb2(Message& message, const Json::Value& value)
    {
        //非object类型，在debug编译选项下可能会导致触发assert，导致进程退出
        if (value.type() != Json::objectValue)
        {
            return 1;
        }
        int ret = 0;
        const Descriptor* pDescriptor = message.GetDescriptor();
        const FieldDescriptor* pFieldDescriptor = NULL;
        bool bRepeated = false;
        map<string, string> key_map;

        Json::Value::Members members(value.getMemberNames());
        for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
        {
            const std::string &name = *it;
            if (value[name].isNull())
            {
                continue;
            }

            pFieldDescriptor = pDescriptor->FindFieldByName(name);
            if (NULL == pFieldDescriptor)
            {
                ret += 1;
                continue;
            }

            bRepeated = pFieldDescriptor->is_repeated();
            if ((bRepeated && !value[name].isArray()) || (!bRepeated && value[name].isArray()))
            {
                ret += 1;
                continue;
            }

            if (bRepeated)
            {
                ret += ToPbRepeated(value[name], pFieldDescriptor, message, key_map);
                continue;
            }

            ret += ToPbSingle(value[name], pFieldDescriptor, message, key_map);
        }

        return ret;
    }

    int ToPbMap(Message& message, const Json::Value& value, map<string, string>& key_map)
    {
        int ret = 0;
        const Descriptor* pDescriptor = message.GetDescriptor();
        const FieldDescriptor* pFieldDescriptor = NULL;
        bool bRepeated = false;

        for(int i = 0; i < pDescriptor->field_count(); i++)
        {
            pFieldDescriptor = (FieldDescriptor *)pDescriptor->field(i);
            string name_str = pFieldDescriptor->containing_type()->name() + "." + pFieldDescriptor->name();
            
            map<string, string>::iterator it = key_map.find(name_str);
            if (it != key_map.end())
            {
                name_str = it->second;
            }
            else
            {
                name_str = pFieldDescriptor->name();
            }

            if (!value.isMember(name_str))
            {
                continue;
            }

            const Json::Value &field = value[name_str.c_str()];

            bRepeated = pFieldDescriptor->is_repeated();
            if ((bRepeated && !field.isArray()) || (!bRepeated && field.isArray()))
            {
                ret += 1;
                continue;
            }

            if (bRepeated)
            {
                ret += ToPbRepeated(field, pFieldDescriptor, message, key_map);
                continue;
            }

            ret += ToPbSingle(field, pFieldDescriptor, message, key_map);
        }

        return ret;
    }
}
