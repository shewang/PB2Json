/*
 * =====================================================================================
 *
 *       Filename:  pb2json.h
 *
 *    Description:  protobuf to json convert
 *
 *        Version:  1.0
 *        Created:  Friday, May 03, 2013 04:41:37 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shewangliu,
 *   Organization:  tencent
 *
 * =====================================================================================
 */

#ifndef __PROTOBUF_TO_JSON_H_
#define __PROTOBUF_TO_JSON_H_
#include <arpa/inet.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <json/json.h>
/*
#if __BYTE_ORDER == __BIG_ENDIAN
#ifndef ntohll
#define ntohll(x)   (x)
#endif
#ifndef htonll
#define htonll(x)   (x)
#endif
#else
#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifndef ntohll
#define ntohll(x)   __bswap_64 (x)
#endif
#ifndef htonll
#define htonll(x)   __bswap_64 (x)
#endif
#endif
#endif

#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#ifndef likely
#define likely(x)       if((x))
#endif
#ifndef unlikely
#define unlikely(x)     if((x))
#endif
#else
#ifndef likely
#define likely(x)       if(__builtin_expect((x) != 0, 1))
#endif
#ifndef unlikely
#define unlikely(x)     if(__builtin_expect((x) != 0, 0))
#endif
#endif
*/
using namespace google::protobuf;
namespace PB2Json
{
    int ToJsonObj(const Message& message, Json::Value& value);
    int ToJson(const Message& message, const FieldDescriptor* pFieldDescriptor, Json::Value& value);
    int ToJsonArray(const Message& message, const FieldDescriptor* pFieldDescriptor, Json::Value& value);
    int ToPbSingle(const Json::Value &value, const FieldDescriptor *pFieldDescriptor, Message &message);
    int ToPbRepeated(const Json::Value &value, const FieldDescriptor *pFieldDescriptor, Message &message);
    int ToPb(Message& message, const Json::Value& value);
}
#endif

