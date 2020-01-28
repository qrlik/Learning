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

double BusManager::Stop_info::Length(const Stop_info& other) const {
	// углы в радианах
	double lat_1 = latitude * PI / 180;
	double long_1 = longitude * PI / 180;

	double lat_2 = other.latitude * PI / 180;
	double long_2 = other.longitude * PI / 180;
	// угловая длина ортодромии
	double l = acos(sin(lat_1) * sin(lat_2) + cos(lat_1) * cos(lat_2) * cos(long_2 - long_1));
	return l * EARTH_RADIUS;
}

double BusManager::CalculateLength(const vector<string_view>& stops) const {
	double result = 0.0;
	for (size_t i = 0; i + 1 < stops.size(); ++i) {
		result += stop_data.at(stops[i].data()).Length(stop_data.at(stops[i + 1].data()));
	}
	return result;
}

void BusManager::AddStop(string name, Stop_info info) {
	stop_data[move(name)] = info;
}

void BusManager::AddBus(string name, const vector<string>& stops) {
	vector<string_view> stops_sv;
	unordered_set<string_view> uniques;
	for (const auto& stop : stops) { // создаем sv для bus_data из stop_data
		if (auto it = stop_data.find(stop); it != stop_data.end()) { // по условию входных данных проверка необязательна
			stops_sv.push_back(it->first);
			uniques.insert(it->first);
		}
		else {
			throw invalid_argument("Stop: \'" + stop + "\' dont exist in stop_data");
		}
	}
	double length = CalculateLength(stops_sv);
	bus_data[move(name)] = Bus_info{ move(stops_sv), length, uniques.size() };
}

string BusManager::BusInfo(string name) const {
	auto it = bus_data.find(name);
	ostringstream result;
	result.setf(ios::fixed);
	result.precision(6);
	result << "Bus " + move(name) + ": ";
	if (it == bus_data.end()) {
		result << "not found";
	}
	else {
		result << it->second.stops.size() << " stops on route, " <<
			it->second.unique_stops << " unique stops, " <<
			it->second.length << " route length";
	}
	return result.str();
}
