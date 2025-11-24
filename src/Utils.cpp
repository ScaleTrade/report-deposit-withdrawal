#include "Utils.hpp"

namespace utils {
    void CreateUI(const ast::Node& node, rapidjson::Value& response, rapidjson::Document::AllocatorType& allocator) {
        Value node_object(kObjectType);
        to_json(node, node_object, allocator);

        Value content_array(kArrayType);
        content_array.PushBack(node_object, allocator);

        Value model_object(kObjectType);
        model_object.AddMember("size", "xxxl", allocator);
        model_object.AddMember("headerContent", Value(kArrayType), allocator);
        model_object.AddMember("footerContent", Value(kArrayType), allocator);
        model_object.AddMember("content", content_array, allocator);

        Value ui_object(kObjectType);
        ui_object.AddMember("modal", model_object, allocator);

        response.SetObject();
        response.AddMember("ui", ui_object, allocator);
    }

    std::string FormatTimestampToString(const time_t timestamp) {
        std::tm tm{};
        localtime_r(&timestamp, &tm);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y.%m.%d %H:%M:%S");
        return oss.str();
    }
}