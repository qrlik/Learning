#pragma once
#include <unordered_map>
#include <vector>
#include <string_view>
#include <optional>
#include <memory>
#include <iostream>

#include "parsers.h"
#include "bus_manager.h"
#include "json.h"

namespace Request {
	using namespace Json;

	enum class Mode {
		MODIFY,
		READ
	};
	enum class Type {
		ADD_STOP,
		ADD_BUS,
		STOP_INFO,
		BUS_INFO
	};

	const std::unordered_map<std::string_view, Type> STR_TO_MODIFY_TYPE{
		{"Stop" , Type::ADD_STOP},
		{"Bus" , Type::ADD_BUS}
	};
	const std::unordered_map<std::string_view, Type> STR_TO_READ_TYPE{
		{"Stop" , Type::STOP_INFO},
		{"Bus" , Type::BUS_INFO}
	};

	class IRequest;
	using RequestHolder = std::unique_ptr<IRequest>;

	namespace Handler { /* считывание/обработка запросов */
		struct Splited {
			std::vector<RequestHolder> modify;
			std::vector<RequestHolder> read;
		};
		Splited Read(std::istream& in_stream = std::cin);
		Document Process(const Splited& requests);
	};

	/* интерфейсы запросов */
	class IRequest {
		Type type;
		static RequestHolder Create(Type type);
		virtual void ParseFrom(const Node& input) = 0;
	protected:
		IRequest(Type t) : type(t) {}
	public:
		static RequestHolder Parse(const Mode mode, const Node& node);

		virtual ~IRequest() = default;
		Type getType() const;
	};

	class IModify : public IRequest {
	protected:
		explicit IModify(Type t) : IRequest(t) {}
	public:
		virtual void Process(BusManager& manager) const = 0;
	};

	template <typename ResultType>
	class IRead : public IRequest {
	protected:
		explicit IRead(Type t) : IRequest(t) {}
	public:
		virtual ResultType Process(const BusManager& manager) const = 0;
	};

	/* modify запросы */
	class AddStop : public IModify {
		std::string stop_name;
		std::unordered_map<std::string, size_t> distances;
		double latitude;
		double longitude;
		void ParseFrom(const Node& input) override;
	public:
		AddStop();
		void Process(BusManager& manager) const override;
	};

	class AddBus : public IModify {
		std::string bus_name;
		std::vector<std::string> stops;
		void ParseFrom(const Node& input) override;
	public:
		AddBus();
		void Process(BusManager& manager) const override;
	};

	/* read запросы */
	class StopInfo : public IRead<Node> {
		std::string stop_name;
		size_t id;
		void ParseFrom(const Node& input) override;
	public:
		StopInfo();
		Node Process(const BusManager& manager) const override;
	};

	class BusInfo : public IRead<Node> {
		std::string bus_name;
		size_t id;
		void ParseFrom(const Node& input) override;
	public:
		BusInfo();
		Node Process(const BusManager& manager) const override;
	};
}
