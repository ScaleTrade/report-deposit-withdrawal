#include "DepositWithdrawalReportInterface.hpp"

#include <iomanip>

using namespace ast;

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Deposit Withdrawal report", allocator), allocator);
    response.AddMember("description",
        Value().SetString("Balance operations performed on trading accounts over the selected period. "
                          "Includes transaction ID, date, amount, and account details.",
             allocator), allocator);
    response.AddMember("type", REPORT_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    // Структура накопления итогов
    // struct Total {};
    //
    // std::unordered_map<std::string, Total> totals_map;

    std::string group_mask;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }


    try {

    } catch (const std::exception& e) {
        std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
    }
}