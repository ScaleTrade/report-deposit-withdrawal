#include "PluginInterface.h"

#include "validators/RequestValidator.h"

extern "C" int GetReportApiVersion() {
    return ReportServerInterface::GetApiVersion();
}

extern "C" void AboutReport(rapidjson::Value&                   request,
                            rapidjson::Value&                   response,
                            rapidjson::Document::AllocatorType& allocator,
                            ReportServerInterface*              server) {
    response.AddMember("version", 1, allocator);
    response.AddMember(
        "name", Value().SetString("Deposit Withdrawal report", allocator), allocator);
    response.AddMember(
        "description",
        Value().SetString(
            "Balance operations performed on trading accounts over the selected period. "
            "Includes transaction ID, date, amount, and account details.",
            allocator),
        allocator);
    response.AddMember("type", static_cast<int>(ReportType::RangeGroup), allocator);
    response.AddMember("key", Value().SetString("DEPOSIT_WITHDRAWAL_REPORT", allocator), allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value&                   request,
                             rapidjson::Value&                   response,
                             rapidjson::Document::AllocatorType& allocator,
                             ReportServerInterface*              server) {

    // Validation
    const ReportType       report_type = ReportType::RangeGroup;
    const ValidationResult validation_result =
        RequestValidator::ValidateRequest(report_type, request);

    if (!validation_result.allowed) {
        utils::WriteAccessError(validation_result, response, allocator);

        std::cerr << "[DepositWithdrawalReportInterface]: " << validation_result.code
                  << ", message: " << validation_result.message << std::endl;
        return;
    }

    std::cout << "[DepositWithdrawalReportInterface]: " << validation_result.code
              << ", message: " << validation_result.message << std::endl;

    // Execution
    std::string group_mask = request["group"].GetString();
    int         from       = request["from"].GetInt();
    int         to         = request["to"].GetInt();

    std::vector<ReportTradeRecord>          trades_vector;
    std::vector<ReportGroupRecord>          groups_vector;
    std::unordered_map<std::string, double> total_profit_map;

    try {
        server->GetTransactionsByGroup(group_mask, from, to, &trades_vector);
        server->GetAllGroups(&groups_vector);
    } catch (const std::exception& e) {
        std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
    }

    // Main table
    TableBuilder table_builder("DepositWithdrawalReportTable");

    // Table props
    table_builder.SetIdColumn("order");
    table_builder.SetOrderBy("order", "DESC");
    table_builder.EnableAutoSave(false);
    table_builder.EnableRefreshButton(false);
    table_builder.EnableBookmarksButton(false);
    table_builder.EnableExportButton(true);
    table_builder.EnableTotal(true);
    table_builder.SetTotalDataTitle("TOTAL");

    // Filters
    FilterConfig search_filter;
    search_filter.type = FilterType::Search;

    FilterConfig date_time_filter;
    date_time_filter.type = FilterType::DateTime;

    // Columns
    table_builder.AddColumn({"order", "ORDER", 1, search_filter});
    table_builder.AddColumn({"login", "LOGIN", 2, search_filter});
    table_builder.AddColumn({"name", "NAME", 3, search_filter});
    table_builder.AddColumn({"type", "TYPE", 4, search_filter});
    table_builder.AddColumn({"close_time", "CLOSE_TIME", 5, date_time_filter});
    table_builder.AddColumn({"comment", "COMMENT", 6, search_filter});
    table_builder.AddColumn({"profit", "AMOUNT", 7, search_filter});
    table_builder.AddColumn({"currency", "CURRENCY", 8, search_filter});
    table_builder.AddColumn({"group", "GROUP", 9, search_filter});

    for (const auto& trade : trades_vector) {
        if (trade.cmd == ReportTradeCommand::BalanceIn ||
            trade.cmd == ReportTradeCommand::BalanceOut) {
            ReportAccountRecord account;

            try {
                server->GetAccountByLogin(trade.login, &account);
            } catch (const std::exception& e) {
                std::cout << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
            }

            std::string currency   = utils::GetGroupCurrencyByName(groups_vector, account.group);
            double      multiplier = 1;

            // Conversion disabled
            // if (currency == "USD") {
            //     try {
            //         server->CalculateConvertRateByCurrency(
            //             currency, "USD", static_cast<int>(trade.cmd), &multiplier);
            //     } catch (const std::exception& e) {
            //         std::cerr << "[DepositWithdrawalReportInterface]: " << e.what() << std::endl;
            //     }
            // }

            total_profit_map[currency] += trade.profit * multiplier;

            table_builder.AddRow({utils::TruncateDouble(trade.order, 0),
                                  utils::TruncateDouble(trade.login, 0),
                                  account.name,
                                  utils::ConvertCmdToString(static_cast<int>(trade.cmd)),
                                  utils::FormatTimestampToString(trade.close_time),
                                  trade.comment,
                                  utils::TruncateDouble(trade.profit * multiplier, 2),
                                  currency,
                                  account.group});
        }
    }

    // Total row
    JSONArray totals_array;
    for (const auto& [currency, profit] : total_profit_map) {
        totals_array.emplace_back(
            JSONObject{{"profit", utils::TruncateDouble(profit, 2)}, {"currency", currency}});
    }

    table_builder.SetTotalData(totals_array);

    const JSONObject table_props = table_builder.CreateTableProps();
    const Node       table_node  = Table({}, table_props);

    const Node report = div({h1({text("Deposit Withdrawal Report")}), table_node});

    utils::CreateUI(report, response, allocator);
}