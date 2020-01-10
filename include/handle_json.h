/**
 * Created by rrzhang on 2019/12/11.
 */

#ifndef MULTI_MASTER_HANDLE_JSON_H
#define MULTI_MASTER_HANDLE_JSON_H

#include <map>
#include <vector>
#include "handle_struct.h"

namespace remote {
    class JsonHandle {
    public:
        static std::string ToJson(const std::string &fun_name, const std::map<int, std::string> &struct_map
                                  , void *struct_address, int struct_size);

        static std::map<int, std::string> FromJson(const std::string &json, void *struct_address, int struct_size);

        static std::string FunctionNameFromJson(const std::string &json);

        static int RetFromJson(const std::string &json);

        static std::string RetToJson(int ret);

        static std::string DirVectorToJson(std::vector<StructHandle::Entry> vp);

        static std::vector<StructHandle::Entry> DirVectorFromJson(std::string json);
    };
}
#endif //MULTI_MASTER_HANDLE_JSON_H
