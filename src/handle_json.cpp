/**
 * Created by rrzhang on 2019/12/9.
 */

#include "handle_json.h"
#include <iostream>

#include "remote_string.h"

#include "my_rapidjson_size_t.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"

namespace remote {

    std::string JsonHandle::ToJson(const std::string &fun_name
                                   , const std::map<int, std::string> &struct_map
                                   , void *struct_ptr, int struct_size) {
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
        // TODO: cout
//        std::cout << "[remote fun] to json string : " << json << std::endl;
        return json;
    }
    //! TODO, some errors
//    std::string
//    ToJson2(const std::string &fun_name, const std::map<int, std::string> &struct_map, void *struct_ptr, int struct_size) {
//        std::string str;
//        rapidjson::Document document;
//        document.SetObject();
//
//        document.AddMember(rapidjson::StringRef("fun_name"), rapidjson::StringRef(fun_name.data(), fun_name.length())
//                           , document.GetAllocator());
//
//        std::string name_struct = remote::JsonKeyName::NameStruct(fun_name);
//        str = std::string((char *) struct_ptr, struct_size);
//        document.AddMember(rapidjson::StringRef(name_struct.data(), name_struct.length())
//                           , rapidjson::StringRef(str.data(), str.length())
//                           , document.GetAllocator());
//
//        rapidjson::Value array1(rapidjson::kArrayType);
//        rapidjson::Value array2(rapidjson::kArrayType);
//        for (auto &iter : struct_map) {
//            rapidjson::Value offset(rapidjson::kStringType);
//            std::string name_offsets = remote::JsonKeyName::NameOffsets(fun_name, iter.first);
//            offset.SetString(name_offsets.data(), name_offsets.length(), document.GetAllocator());
//            array1.PushBack(name_offsets, document.GetAllocator());
//
//            rapidjson::Value object(rapidjson::kObjectType);
//            object.AddMember(rapidjson::StringRef(name_offsets.data(), name_offsets.length()), rapidjson::StringRef(
//                    iter.second.data(), iter.second.length()), document.GetAllocator());
//            array2.PushBack(object, document.GetAllocator());
//        }
//        std::string name_ptr_keys = remote::JsonKeyName::NamePtrKeys(fun_name);
//        std::string name_ptrs = remote::JsonKeyName::NamePtrs(fun_name);
//        document.AddMember(rapidjson::StringRef(name_ptr_keys.data(), name_ptr_keys.length()), array1
//                           , document.GetAllocator());
//        document.AddMember(rapidjson::StringRef(name_ptrs.data(), name_ptrs.length()), array2, document.GetAllocator());
//
//        rapidjson::StringBuffer buffer;
//        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//        document.Accept(writer);
//        return std::string(buffer.GetString());
//    }

    std::map<int, std::string> JsonHandle::FromJson(const std::string &json, void *struct_address, int struct_size) {
        rapidjson::Document document;
        document.Parse(json.data());

        rapidjson::Value &v_name = document["fun_name"];
        rapidjson::Value &v_struct = document[remote::JsonKeyName::NameStruct(v_name.GetString()).data()];
        rapidjson::Value &v_keys = document[remote::JsonKeyName::NamePtrKeys(v_name.GetString()).data()];
        rapidjson::Value &v_ptrs = document[remote::JsonKeyName::NamePtrs(v_name.GetString()).data()];
        //! v_struct
        memcpy(struct_address, v_struct.GetString(), struct_size);
        std::map<int, std::string> struct_map;
        for (rapidjson::SizeType i = 0; i < v_keys.Size(); i++) {
            //! ptr_off 是该指针在 struct 内部的位置
            int ptr_off = std::stoi(std::string(v_keys[i].GetString())
                                            .substr(strlen(v_name.GetString()) + 1), nullptr, 10);
            //! ptr_str 是该指针在指向的内容，需要 copy 到 struct 指针指向的位置

            std::string ptr_str = v_ptrs[i][v_keys[i].GetString()].GetString();
            struct_map.insert(std::make_pair(ptr_off, ptr_str));
        }
        return struct_map;
    }

    std::string JsonHandle::FunctionNameFromJson(const std::string &json) {
        rapidjson::Document document;
        document.Parse(json.data());
        return std::string(document["fun_name"].GetString());
    }

    int JsonHandle::RetFromJson(const std::string &json) {
        rapidjson::Document document;
        document.Parse(json.data());
        return document["ret"].GetInt();
    }

    std::string JsonHandle::RetToJson(int ret) {
        rapidjson::Document doc;
        doc.SetObject();
        doc.AddMember("ret", ret, doc.GetAllocator());
        rapidjson::StringBuffer string_buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(string_buffer);
        doc.Accept(writer);

        return std::string(string_buffer.GetString());
    }

    std::string JsonHandle::DirVectorToJson(std::vector<StructHandle::Entry> vp) {

        rapidjson::Document document;
        document.SetObject();
// 添加json array
        rapidjson::Value array_objects(rapidjson::kArrayType);
        for (auto iter : vp) {
            rapidjson::Value object(rapidjson::kObjectType);

            rapidjson::Value path(rapidjson::kStringType);
            rapidjson::Value depth(rapidjson::kNumberType);
            path.SetString(iter.m_path.data(), iter.m_path.length(), document.GetAllocator());
            depth.SetInt(iter.m_depth);
            object.AddMember(rapidjson::StringRef("depth"), depth, document.GetAllocator());
            object.AddMember(rapidjson::StringRef("path"), path
                             , document.GetAllocator());
            array_objects.PushBack(object, document.GetAllocator());
        }
        document.AddMember(rapidjson::StringRef("vector_dir"), array_objects, document.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        return std::string(buffer.GetString());
    }

    std::vector<StructHandle::Entry> JsonHandle::DirVectorFromJson(std::string json) {
        rapidjson::Document document;
        document.Parse(json.data());

        std::vector<StructHandle::Entry> vp;
        rapidjson::Value &dir_vector = document["vector_dir"];

        for (rapidjson::SizeType i = 0; i < dir_vector.Size(); i++) {
            std::string path = dir_vector[i]["path"].GetString();
            int depth = dir_vector[i]["depth"].GetInt();
            vp.push_back(StructHandle::Entry(path, depth));
        }
        return vp;
    }
}