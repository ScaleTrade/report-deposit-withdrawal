#pragma once

#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>

#include "rapidjson/document.h"
#include "ReportServerInterface.h"
#include "validators/RequestValidator.h"
#include "ast/Ast.hpp"
#include "sbxTableBuilder/SBXTableBuilder.hpp"
#include "structures/ReportType.h"
#include "structures/ValidationResult.h"
#include "utils/Utils.h"

using namespace ast;

extern "C" {
    int GetReportApiVersion();

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