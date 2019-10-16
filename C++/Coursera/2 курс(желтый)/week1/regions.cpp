#include <iostream>
#include <algorithm>
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
{ // обертка для регионов, чтобы использовать ключи-ссылки в словаре, а не делать тяжелые копии
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
	int result = 0;
	map<reg_wrapper, int> search; // карта поиска (регион(ссылка в обертке) - количество повторов)
	for (const auto& reg : regions)
	{ // сразу заносим в карту и ищем максимум
		result = max(result, ++search[static_cast<reg_wrapper>(reg)]);
	} // красивое решение, до которого я не додумался =)
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
	// 3 1
	return 0;
}
