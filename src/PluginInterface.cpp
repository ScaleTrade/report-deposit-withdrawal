#include "PluginInterface.hpp"

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
    std::vector<GroupRecord> groups_vector;

    try {
        server->GetTransactionsByGroup(group_mask, from, to, &trades_vector);
        server->GetAllGroups(&groups_vector);
    } catch (const std::exception& e) {
        std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
    }

    // Лямбда для поиска валюты аккаунта по группе
    auto get_group_currency = [&](const std::string& group_name) -> std::string {
        for (const auto& group : groups_vector) {
            if (group.group == group_name) {
                return group.currency;
            }
        }
        return "N/A"; // группа не найдена - валюта не определена
    };

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    auto make_table = [&](const std::vector<AccountRecord>& accounts) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Order")})}),
            th({div({text("Login")})}),
            th({div({text("Name")})}),
            th({div({text("Time")})}),
            th({div({text("Comment")})}),
            th({div({text("Amount")})}),
            th({div({text("Currency")})}),
        }));

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    const Node report = div({
        h1({ text("Deposit Withdrawal Report") }),
        // make_table(accounts_vector),
    });

    utils::CreateUI(report, response, allocator);
}