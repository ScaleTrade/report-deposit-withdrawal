#pragma once

#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <unordered_map>

#include "rapidjson/document.h"
#include "ReportServerInterface.h"
#include "ast/Ast.hpp"
#include "sbxTableBuilder/SBXTableBuilder.hpp"
#include "structures/ReportType.h"
#include "utils/Utils.h"

extern "C" {
    void AboutReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     ReportServerInterface* server);

    void DestroyReport();

    void CreateReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     ReportServerInterface* server);
}