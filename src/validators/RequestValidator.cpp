#include "RequestValidator.h"

ValidationResult RequestValidator::ValidateRequest(ReportType              report_type,
                                                   const rapidjson::Value& request) {

    switch (report_type) {
        case ReportType::None:
            return ValidateNone(request);
        case ReportType::Range:
            return ValidateRange(request);
        case ReportType::Daily:
            return ValidateDaily(request);
        case ReportType::Account:
            return ValidateAccount(request);
        case ReportType::Symbol:
            return ValidateSymbol(request);
        case ReportType::Group:
            return ValidateGroup(request);
        case ReportType::RangeGroup:
            return ValidateRangeGroup(request);
        case ReportType::DailyGroup:
            return ValidateDailyGroup(request);
        case ReportType::RangeAccount:
            return ValidateRangeAccount(request);
        case ReportType::DailyAccount:
            return ValidateDailyAccount(request);
        case ReportType::RangeSymbol:
            return ValidateRangeSymbol(request);
        case ReportType::DailySymbol:
            return ValidateDailySymbol(request);
        case ReportType::RangeGroupSymbol:
            return ValidateRangeGroupSymbol(request);
        case ReportType::DailyGroupSymbol:
            return ValidateDailyGroupSymbol(request);

        default: {
            ValidationResult result;
            result.allowed = false;
            result.code    = 404;
            result.message = "Unknown report type";
            return result;
        }
    }
}

ValidationResult RequestValidator::ValidateNone(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "None: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRange(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Range: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDaily(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Daily: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateAccount(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Account: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateSymbol(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Symbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateGroup(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Group: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeGroup(const rapidjson::Value& request) {
    ValidationResult result;

    if (!request.HasMember("group") || !request["group"].IsString()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'group'";
        return result;
    }

    if (!request.HasMember("from") || !request["from"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'from'";
        return result;
    }

    if (!request.HasMember("to") || !request["to"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'to'";
        return result;
    }

    const std::string group = request["group"].GetString();
    if (group == "*") {
        result.allowed = true;
        result.code    = 200;
        result.message = "RangeGroup: access granted (all groups)";
        return result;
    }

    const rapidjson::Value& access = request["__access"];
    const std::string       groups = access["groups"].GetString();
    if (groups == "*") {
        result.allowed = true;
        result.code    = 200;
        result.message = "ValidateRangeGroup: access granted (user has all groups)";
        return result;
    }

    const std::set<std::string> allowed_groups   = utils::SplitToSet(groups);
    const std::set<std::string> requested_groups = utils::SplitToSet(group);

    for (const std::string& requested_group : requested_groups) {
        if (allowed_groups.find(requested_group) == allowed_groups.end()) {
            result.allowed = false;
            result.code    = 403;
            result.message =
                "ValidateRangeGroup: access denied for group '" + requested_group + "'";
            return result;
        }
    }

    // Все запрошенные группы доступны
    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateRangeGroup: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateDailyGroup(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailyGroup: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeAccount(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "RangeAccount: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailyAccount(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailyAccount: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeSymbol(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "RangeSymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailySymbol(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailySymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeGroupSymbol(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "RangeGroupSymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailyGroupSymbol(const rapidjson::Value& request) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailyGroupSymbol: access granted (stub)";
    return result;
}