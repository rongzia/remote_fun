//
// Created by rrzhang on 2019/12/31.
//

#include <iostream>
#include <map>
#include "remote_string.h"

#include "my_rapidjson_size_t.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"

using namespace std;

std::string
ToJson(const std::string &fun_name, const std::map<int, std::string> &struct_map, void *struct_ptr, int struct_size) {
    rapidjson::StringBuffer string_buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);

    writer.StartObject();
    {
        //! fun_name,
        //! e.g. "fun_name":"pwrite"
        writer.Key("fun_name");
        writer.String(fun_name.data(), fun_name.length(), true);
    }
    {
        //! struct_[name], the struct, contains data and ptrs
        //! e.g. "struct_pwrite":"..."
        // TODO: true 参数
        std::string str = std::string((char *) struct_ptr, struct_size);
        writer.Key(remote::JsonKeyName::NameStruct(fun_name).data());
        writer.String(str.data(), str.length(), true);
    }
    {
        //! [name]_keys,
        //! e.g. "pwrite_keys":["pwrite_8","pwrite_16"]
        writer.Key(remote::JsonKeyName::NamePtrKeys(fun_name).data());
        writer.StartArray();
        for (auto &iter : struct_map) {
            writer.String(remote::JsonKeyName::NameOffsets(fun_name, iter.first).data());
        }
        writer.EndArray();
    }
    {
        //! [name]_ptrs,
        //! e.g. "pwrite_ptrs":[{"pwrite_8":"foo","pwrite_16":"bar}]
        writer.Key(remote::JsonKeyName::NamePtrs(fun_name).data());
        writer.StartArray();
        for (auto &iter : struct_map) {
            writer.StartObject();
            writer.Key(remote::JsonKeyName::NameOffsets(fun_name, iter.first).data());
            writer.String(iter.second.data(), iter.second.length(), true);
            writer.EndObject();
        }
        writer.EndArray();
    }
    writer.EndObject();

    std::string json(string_buffer.GetString());
#ifdef MULTI_MASTER_ZHANG_DEBUG_JSON
    std::cout << "to json string : " << json << std::endl;
#endif // MULTI_MASTER_ZHANG_DEBUG_JSON
    return json;
}




struct StructPwrite {
    int fd;
    char *buf;
    size_t nbytes;
    off64_t offset;
};

struct StructClose {
    int fd;
};

int main() {

    auto *struct_ptr1 = new StructPwrite();
    struct_ptr1->buf = (char *) "add";
    struct_ptr1->fd = 1;
    struct_ptr1->nbytes = 3;
    struct_ptr1->offset = 0;
    std::map<int, std::string> struct_map;
    struct_map.insert(
            std::make_pair((int64_t) &struct_ptr1->buf - (int64_t) struct_ptr1, std::string(struct_ptr1->buf
                                                                                            , struct_ptr1->nbytes))
    );
    cout <<struct_ptr1->buf<<endl;
    cout <<struct_map.begin()->second<<endl;
    ToJson("pwrite", struct_map, struct_ptr1, sizeof(StructPwrite));


    return 0;
}