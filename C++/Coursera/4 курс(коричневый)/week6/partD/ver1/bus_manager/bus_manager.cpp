#include "bus_manager.h"
#include <unordered_set>
#include <cmath>
#include <numeric>
#include <sstream>

using namespace std;

namespace {
	const size_t EARTH_RADIUS = 6'371'000;
	const double PI = 3.1415926535;
}

double BusManager::Stop_info::GeoLength(const Stop_info& other) const { /* рассчет географической дистанции */
	// углы в радианах
	double lat_1 = latitude * PI / 180;
	double long_1 = longitude * PI / 180;

	double lat_2 = other.latitude * PI / 180;
	double long_2 = other.longitude * PI / 180;
	// угловая длина ортодромии
	double l = acos(sin(lat_1) * sin(lat_2) + cos(lat_1) * cos(lat_2) * cos(long_2 - long_1));
	return l * EARTH_RADIUS;
}

optional<size_t> BusManager::Stop_info::RoadLength(string_view other) const {
	if (auto it = distances.find(other); it != distances.end()) {
		return it->second;
	}
	else {
		return nullopt;
	}
}

BusManager::Lenght_info BusManager::CalculateLength(const vector<string_view>& stops) const {
	double road_length = 0.0;
	double geo_length = 0.0;
	for (size_t i = 0; i + 1 < stops.size(); ++i) {
		const Stop_info& lhs = stop_data.at(stops[i].data());
		const Stop_info& rhs = stop_data.at(stops[i + 1].data());
		auto road = lhs.RoadLength(stops[i + 1]);
		double geo = lhs.GeoLength(rhs);
		road_length += (road) ? road.value() : geo;
		geo_length += geo;
	}
	return { road_length, road_length / geo_length };
}

void BusManager::AddStop(string name, const unordered_map<string, size_t>& distances,
	double latitude, double longitude) {
	auto stop = stop_data.insert({ move(name), Stop_info{} }).first;
	stop->second.latitude = latitude;
	stop->second.longitude = longitude;
	auto& stop_dist = stop->second.distances;

	for (const auto& [stop_name, distance] : distances) {
		auto it = stop_data.insert({ stop_name, Stop_info{} }).first;
		stop_dist[it->first] = distance;
		it->second.distances.insert({ stop->first, distance });
		/* По умолчанию предполагается, что расстояние от stop до stop_name# равно расстоянию от stop_name# до stop,
		если только расстояние от stop_name# до stop не задано явным образом при добавлении остановки stop_name# */
	}
}

void BusManager::AddBus(string name, const vector<string>& stops) {
	auto [bus, is_insert] = bus_data.insert({ move(name), Bus_info{} });
	if (!is_insert) { /* по условию bus_name уникальны */
		throw invalid_argument("Bus: \'" + bus->first + "\' already exist in bus_data");
	}

	vector<string_view>& stops_sv = bus->second.stops;
	stops_sv.reserve(stops.size());
	unordered_set<string_view> uniques;
	for (const auto& stop : stops) { // создаем sv для bus_data из stop_data
		if (auto it = stop_data.find(stop); it != stop_data.end()) {
			stops_sv.push_back(it->first); // string_view для stops в Bus_info
			uniques.insert(it->first);
			it->second.buses.insert(bus->first); // обновляем buses в Stop_info
		}
		else { /* по условию stop_name уже в данных */
			throw invalid_argument("Stop: \'" + stop + "\' dont exist in stop_data");
		}
	}
	Lenght_info length_i = CalculateLength(stops_sv);
	bus->second = Bus_info{ move(stops_sv), length_i, uniques.size() };
}

const BusManager::Stop_info* BusManager::StopInfo(const string& name) const {
	if (auto it = stop_data.find(name); it != stop_data.end()) {
		return &it->second;
	}
	return nullptr;
}

const BusManager::Bus_info* BusManager::BusInfo(const string& name) const {
	if (auto it = bus_data.find(name); it != bus_data.end()) {
		return &it->second;
	}
	return nullptr;
}
