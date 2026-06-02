#pragma once

#include <string>

#include "rapidjson/document.h"
#include "structures/ReportType.h"
#include "structures/ValidationResult.h"
#include "utils/Utils.h"

class RequestValidator {
public:
    static ValidationResult ValidateRequest(ReportType              report_type,
                                            const rapidjson::Value& request);

private:
    static ValidationResult ValidateNone(const rapidjson::Value& request);
    static ValidationResult ValidateRange(const rapidjson::Value& request);
    static ValidationResult ValidateDaily(const rapidjson::Value& request);
    static ValidationResult ValidateAccount(const rapidjson::Value& request);
    static ValidationResult ValidateSymbol(const rapidjson::Value& request);
    static ValidationResult ValidateGroup(const rapidjson::Value& request);
    static ValidationResult ValidateRangeGroup(const rapidjson::Value& request);
    static ValidationResult ValidateDailyGroup(const rapidjson::Value& request);
    static ValidationResult ValidateRangeAccount(const rapidjson::Value& request);
    static ValidationResult ValidateDailyAccount(const rapidjson::Value& request);
    static ValidationResult ValidateRangeSymbol(const rapidjson::Value& request);
    static ValidationResult ValidateDailySymbol(const rapidjson::Value& request);
    static ValidationResult ValidateRangeGroupSymbol(const rapidjson::Value& request);
    static ValidationResult ValidateDailyGroupSymbol(const rapidjson::Value& request);
};