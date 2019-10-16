#include <iostream>
#include <map>
#include <vector>
#include <tuple>

using namespace std;
// Здесь Lang — идентификатор языка
enum class Lang { DE, FR, IT };
// Имеется база регионов, представленная вектором структур Region
struct Region
{
	string std_name; // имя региона
	string parent_std_name; // имя родителя
	map<Lang, string> names; // имя на разных языках
	int64_t population;
	bool operator<(const Region& rhs) const
	{
		return tie(std_name, parent_std_name, names, population) < tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
	}
	bool operator==(const Region& rhs) const
	{
		return tie(std_name, parent_std_name, names, population) == tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
	}
};

struct reg_wrapper
{
	const Region& reg_ref;
	explicit reg_wrapper(const Region& reg) : reg_ref(reg) {}
	operator Region() { return reg_ref; }
	bool operator<(const reg_wrapper rhs) const
	{
		return reg_ref < rhs.reg_ref;
	}
	bool operator==(const reg_wrapper rhs) const
	{
		return reg_ref == rhs.reg_ref;
	}
};

/*
Напишите функцию FindMaxRepetitionCount, принимающую базу регионов и определяющую, какое максимальное количество повторов она содержит.
Две записи (объекты типа Region) считаются различными, если они отличаются хотя бы одним полем.
Если все записи уникальны, считайте максимальное количество повторов равным 1. Если записи отсутствуют, верните 0. Гарантируется, что типа int достаточно для хранения ответа.
*/

int FindMaxRepetitionCount(const vector<Region>& regions)
{
	// 1.map<Region&, count>
	// vector -> map
	// for(map) -> max count
	map<reg_wrapper, int> search; // карта поиска (регион(ссылка в обертке) - количество повторов)
	for (size_t i = 0; i < regions.size(); ++i)
	{
		if (search.find(static_cast<reg_wrapper>(regions[i])) == search.end())
		{ // если данный регион еще не встречался
			for (size_t j = i; j < regions.size(); ++j)
			{ // считаем все повторы
				++search[static_cast<reg_wrapper>(regions[j])];
			}
		}
	}
	int result = 0;
	for (const auto& reg : search)
	{
		if (reg.second > result)
		{
			result = reg.second;
		}
	}
	return result;
}

int main() {
	cout << FindMaxRepetitionCount({
		{
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Russia",
			"Eurasia",
			{{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
			89
		}, {
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Russia",
			"Eurasia",
			{{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
			89
		},
		}) << endl;

	cout << FindMaxRepetitionCount({
		{
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Russia",
			"Eurasia",
			{{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
			89
		}, {
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Moscow",
			"Toulouse",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			89
		}, {
			"Moscow",
			"Russia",
			{{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
			31
		},
		}) << endl;

	return 0;
}
