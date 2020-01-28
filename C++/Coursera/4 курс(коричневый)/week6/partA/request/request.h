#pragma once
#include <unordered_map>
#include <vector>
#include <string_view>
#include <optional>
#include <memory>
#include <iostream>

#include "parsers.h"
#include "bus_manager.h"


namespace Request {
	namespace {
		enum class Mode {
			MODIFY,
			READ
		};

		enum class Type {
			ADD_STOP,
			ADD_BUS,
			BUS_INFO
		};

		const std::unordered_map<std::string_view, Type> STR_TO_MODIFY_TYPE{
			{"Stop" , Type::ADD_STOP},
			{"Bus" , Type::ADD_BUS}
		};
		const std::unordered_map<std::string_view, Type> STR_TO_READ_TYPE{
			{"Bus" , Type::BUS_INFO}
		};

		std::optional<Type> ConvertTypeFromString(const Mode mode, std::string_view str);

		class IRequest;
		using RequestHolder = std::unique_ptr<IRequest>;
		RequestHolder Parse(const Mode mode, std::string_view str);

		class IRequest {
			Type type;
			virtual void ParseFrom(std::string_view input) = 0;
			friend RequestHolder Parse(const Mode mode, std::string_view str);

		public:
			IRequest(Type t) : type(t) {}
			virtual ~IRequest() = default;
			static RequestHolder Create(Type type);
			Type getType() const;
		};

		class IModify : public IRequest {
		public:
			explicit IModify(Type t) : IRequest(t) {}
			virtual void Process(BusManager& manager) const = 0;
		};

		template <typename ResultType>
		class IRead : public IRequest {
		public:
			explicit IRead(Type t) : IRequest(t) {}
			virtual ResultType Process(const BusManager& manager) const = 0;
		};

		class AddStop : public IModify {
			std::string stop_name;
			double latitude;
			double longitude;
			void ParseFrom(std::string_view input) override;
		public:
			AddStop() : IModify(Type::ADD_STOP), latitude(0), longitude(0) {}
			void Process(BusManager& manager) const override;
		};

		class AddBus : public IModify {
			std::string bus_name;
			std::vector<std::string> stops;
			void ParseFrom(std::string_view input) override;
		public:
			AddBus() : IModify(Type::ADD_BUS) {}
			void Process(BusManager& manager) const override;
		};

		class BusInfo : public IRead<std::string> {
			std::string bus_name;
			void ParseFrom(std::string_view input) override;
		public:
			BusInfo() : IRead(Type::BUS_INFO) {}
			std::string Process(const BusManager& manager) const override;
		};

		std::vector<RequestHolder> SplitRead(Mode mode, std::istream& in_stream);
	}

	struct Splited {
		std::vector<RequestHolder> modify;
		std::vector<RequestHolder> read;
	};

	Splited Read(std::istream& in_stream = std::cin);
	std::vector<std::string> Process(const Splited& requests);
	void Print(const std::vector<std::string>& responses, std::ostream& os_stream = std::cout);
}
