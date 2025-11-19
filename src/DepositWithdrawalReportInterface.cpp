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
    response.AddMember("type", REPORT_RANGE_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    // Структура накопления итогов
    struct Total {
        double balance;
        std::string currency;
    };

    std::unordered_map<std::string, Total> totals_map;

    std::string group_mask;
    int from;
    int to;
    if (request.HasMember("group") && request["group"].IsNumber()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
    }
    if (request.HasMember("to") && request["to"].IsString()) {
        to = request["to"].GetInt();
    }

    std::vector<TradeRecord> trades_vector;

    try {
        server->GetTransactionsByGroup(group_mask, from, to, &trades_vector);
        std::cout << "Trades vector size: " << trades_vector.size() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
    }
}