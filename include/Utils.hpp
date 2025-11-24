#pragma once

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "ast/Ast.hpp"
#include "Structures.hpp"

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t timestamp);
}