#pragma once
#include <unordered_map>
#include <vector>
#include <string_view>
#include <optional>
#include <memory>
#include <iostream>

#include "parsers.h"
#include "bus_router.h"
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
		BUS_INFO,
		ROUTE_INFO
	};

	const std::unordered_map<std::string_view, Type> STR_TO_MODIFY_TYPE{
		{"Stop" , Type::ADD_STOP},
		{"Bus" , Type::ADD_BUS}
	};
	const std::unordered_map<std::string_view, Type> STR_TO_READ_TYPE{
		{"Stop" , Type::STOP_INFO},
		{"Bus" , Type::BUS_INFO},
		{"Route", Type::ROUTE_INFO}
	};

	class IRequest;
	using RequestHolder = std::unique_ptr<IRequest>;

	namespace Handler { /* считывание/обработка запросов */
		struct Settings {
			double bus_velocity;
			uint16_t bus_wait_time;
		};
		struct Splited {
			std::vector<RequestHolder> modify;
			std::vector<RequestHolder> read;
		};
		std::pair<Settings,Splited> Read(std::istream& in_stream = std::cin);
		Document Process(const std::pair<Settings, Splited>& requests);
	};

	/* интерфейсы запросов */
	class IRequest {
		Type type;
		static RequestHolder Create(Type type);
		virtual void ParseFrom(const Node& input) = 0;
	protected:
		explicit IRequest(Type t);
	public:
		static RequestHolder Parse(const Mode mode, const Node& node);

		virtual ~IRequest() = default;
		Type getType() const;
	};

	class IModify : public IRequest {
	protected:
		explicit IModify(Type t);
	public:
		virtual void Process(BusManager& manager) const = 0;
	};

	template <typename ResultType, typename ReadType>
	class IRead : public IRequest {
	protected:
		explicit IRead(Type t);
	public:
		virtual ResultType Process(const ReadType& manager) const = 0;
	};

	/* modify запросы */
	class AddStop : public IModify {
		std::string stop_name;
		std::unordered_map<std::string, size_t> distances;
		double latitude;
		double longitude;
		void ParseFrom(const Node& node) override;
	public:
		AddStop();
		void Process(BusManager& manager) const override;
	};

	class AddBus : public IModify {
		std::string bus_name;
		std::vector<std::string> stops;
		bool is_roundtrip;
		void ParseFrom(const Node& node) override;
	public:
		AddBus();
		void Process(BusManager& manager) const override;
	};

	/* read запросы */
	class StopInfo : public IRead<Node, BusManager> {
		std::string stop_name;
		size_t id;
		void ParseFrom(const Node& node) override;
	public:
		StopInfo();
		Node Process(const BusManager& manager) const override;
	};

	class BusInfo : public IRead<Node, BusManager> {
		std::string bus_name;
		size_t id;
		void ParseFrom(const Node& node) override;
	public:
		BusInfo();
		Node Process(const BusManager& manager) const override;
	};

	class RouteInfo : public IRead<Node, BusRouter> {
		std::string from_stop;
		std::string to_stop;
		size_t id;
		void ParseFrom(const Node& node) override;
	public:
		RouteInfo();
		Node Process(const BusRouter& router) const override;
	};
}
