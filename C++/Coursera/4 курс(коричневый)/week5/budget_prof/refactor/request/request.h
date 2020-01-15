#pragma once
#include <unordered_map>
#include <string_view>
#include <optional>
#include <memory>
#include <iostream>

#include "budget.h"

enum class Type {
	COMPUTE_INCOME,
	EARN,
	SPEND,
	PAY_TAX
};

const std::unordered_map<std::string_view, Type> STR_TO_REQUEST_TYPE = {
	{"ComputeIncome", Type::COMPUTE_INCOME},
	{"Earn", Type::EARN},
	{"Spend", Type::SPEND},
	{"PayTax", Type::PAY_TAX}
};

std::optional<Type> ConvertRequestTypeFromString(std::string_view str);

struct Request;
using RequestHolder = std::unique_ptr<Request>;
struct Request
{
	const Type type;

	explicit Request(Type type) : type(type) {}
	virtual void ParseFrom(std::string_view input) = 0;
	static RequestHolder Create(Type type);
	virtual ~Request() = default;
};

template <typename ResultType>
struct ReadRequest : Request {
	using Request::Request;
	virtual ResultType Process(const BudgetManager& manager) const = 0;
};

struct ModifyRequest : Request {
	using Request::Request;
	virtual void Process(BudgetManager& manager) const = 0;
};

struct ComputeIncomeRequest : ReadRequest<double> {
	ComputeIncomeRequest() : ReadRequest(Type::COMPUTE_INCOME) {}
	void ParseFrom(std::string_view input) override;
	double Process(const BudgetManager& manager) const override;

	Date date_from = START_DATE;
	Date date_to = START_DATE;
};

template <int SIGN>
struct AddMoneyRequest : ModifyRequest {
	static_assert(SIGN == -1 || SIGN == 1);

	AddMoneyRequest() : ModifyRequest(SIGN == 1 ? Type::EARN : Type::SPEND) {}
	void ParseFrom(std::string_view input) override {
		date_from = Date::FromString(ReadToken(input));
		date_to = Date::FromString(ReadToken(input));
		value = ConvertToInt(input);
	}

	void Process(BudgetManager& manager) const override {
		const auto date_range = manager.MakeDateRange(date_from, date_to);
		const double daily_value = value * 1.0 / size(date_range);
		const MoneyState daily_change =
			SIGN == 1
			? MoneyState{ .earned = daily_value }
		: MoneyState{ .spent = daily_value };
		for (auto& money : date_range) {
			money += daily_change;
		}
	}

	Date date_from = START_DATE;
	Date date_to = START_DATE;
	size_t value = 0;
};

struct PayTaxRequest : ModifyRequest {
	PayTaxRequest() : ModifyRequest(Type::PAY_TAX) {}
	void ParseFrom(std::string_view input) override;
	void Process(BudgetManager& manager) const override;

	Date date_from = START_DATE;
	Date date_to = START_DATE;
	uint8_t percentage = 0;
};

RequestHolder ParseRequest(std::string_view request_str);
std::vector<RequestHolder> ReadRequests(std::istream& in_stream = std::cin);
std::vector<double> ProcessRequests(const std::vector<RequestHolder>& requests);
void PrintResponses(const std::vector<double>& responses, std::ostream& stream = std::cout);
