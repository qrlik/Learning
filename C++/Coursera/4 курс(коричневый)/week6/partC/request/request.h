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
	enum class Mode; /* режим запросов */
	enum class Type;

	class IRequest;
	using RequestHolder = std::unique_ptr<IRequest>;

	namespace Handler { /* считывание/обработка запросов */
		struct Splited {
			std::vector<RequestHolder> modify;
			std::vector<RequestHolder> read;
		};
		Splited Read(std::istream& in_stream = std::cin);
		std::vector<std::string> Process(const Splited& requests);
		void Print(const std::vector<std::string>& responses, std::ostream& os_stream = std::cout);
	};

	/* интерфейсы запросов */
	class IRequest {
		Type type;
		static RequestHolder Create(Type type);
		virtual void ParseFrom(std::string_view input) = 0;
	protected:
		IRequest(Type t) : type(t) {}
	public:
		static RequestHolder Parse(const Mode mode, std::string_view str);

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
		void ParseFrom(std::string_view input) override;
	public:
		AddStop();
		void Process(BusManager& manager) const override;
	};

	class AddBus : public IModify {
		std::string bus_name;
		std::vector<std::string> stops;
		void ParseFrom(std::string_view input) override;
	public:
		AddBus();
		void Process(BusManager& manager) const override;
	};

	/* read запросы */
	class StopInfo : public IRead<std::string> {
		std::string stop_name;
		void ParseFrom(std::string_view input) override;
	public:
		StopInfo();
		std::string Process(const BusManager& manager) const override;
	};

	class BusInfo : public IRead<std::string> {
		std::string bus_name;
		void ParseFrom(std::string_view input) override;
	public:
		BusInfo();
		std::string Process(const BusManager& manager) const override;
	};
}
