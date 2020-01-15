#include <numeric>
#include "request.h"

using namespace std;

optional<Type> ConvertRequestTypeFromString(string_view str) {
	if (const auto it = STR_TO_REQUEST_TYPE.find(str);
		it != STR_TO_REQUEST_TYPE.end()) {
		return it->second;
	}
	else {
		return nullopt;
	}
}

// ComputeIncomeRequest
void ComputeIncomeRequest::ParseFrom(string_view input) {
	date_from = Date::FromString(ReadToken(input));
	date_to = Date::FromString(input);
}

double ComputeIncomeRequest::Process(const BudgetManager& manager) const{
	const auto range = manager.MakeDateRange(date_from, date_to);
	return accumulate(begin(range), end(range), MoneyState{}).ComputeIncome();
}

// PayTaxRequest
void PayTaxRequest::ParseFrom(string_view input) {
	date_from = Date::FromString(ReadToken(input));
	date_to = Date::FromString(ReadToken(input));
	percentage = ConvertToInt(input);
}

void PayTaxRequest::Process(BudgetManager& manager) const{
	for (auto& money : manager.MakeDateRange(date_from, date_to)) {
		money.earned *= 1 - percentage / 100.0;
	}
}

RequestHolder Request::Create(Type type) {
	switch (type) {
	case Type::COMPUTE_INCOME:
		return make_unique<ComputeIncomeRequest>();
	case Type::EARN:
		return make_unique<AddMoneyRequest<+1>>();
	case Type::SPEND:
		return make_unique<AddMoneyRequest<-1>>();
	case Type::PAY_TAX:
		return make_unique<PayTaxRequest>();
	default:
		return nullptr;
	}
}

// Request Parsers

RequestHolder ParseRequest(string_view request_str) {
	const auto request_type = ConvertRequestTypeFromString(ReadToken(request_str));
	if (!request_type) {
		return nullptr;
	}
	RequestHolder request = Request::Create(*request_type);
	if (request) {
		request->ParseFrom(request_str);
	};
	return request;
}

vector<RequestHolder> ReadRequests(istream& in_stream)
{
	const size_t count = ReadNumberOnLine<size_t>(in_stream);

	vector<RequestHolder> requests;
	requests.reserve(count);
	string request_str;

	for (size_t i = 0; i < count && getline(in_stream, request_str); ++i)
	{
		if (auto request = ParseRequest(request_str)) {
			requests.push_back(move(request));
		}
	}
	return requests;
}

vector<double> ProcessRequests(const vector<RequestHolder>& requests) {
	vector<double> responses;
	BudgetManager manager;
	for (const auto& request_holder : requests) {
		if (request_holder->type == Type::COMPUTE_INCOME) {
			const auto& request = static_cast<const ComputeIncomeRequest&>(*request_holder);
			responses.push_back(request.Process(manager));
		}
		else {
			const auto& request = static_cast<const ModifyRequest&>(*request_holder);
			request.Process(manager);
		}
	}
	return responses;
}

void PrintResponses(const vector<double>& responses, ostream& stream) {
	for (const double response : responses) {
		stream << response << endl;
	}
}

