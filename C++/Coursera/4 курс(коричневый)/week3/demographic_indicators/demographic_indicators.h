#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Person {
    std::string name;
    size_t age, income;
    bool is_male;
};

class DemographicIndicators
{
    std::vector<size_t> _age; // диаграмма возрастов (индекс -> количество людей с данным возрастом)
    std::vector<uint64_t> _wealthy; // диаграмма капиталов (индекс -> капитал Топ(индекс+1) самых богатых)

    std::unordered_map<std::string, size_t> _male_name; // имя - количество людей
    std::unordered_map<std::string, size_t> _female_name; // имя - количество людей
    std::pair<size_t, std::string> _top_male_name; // две пары самых популярных имен
    std::pair<size_t, std::string> _top_female_name;

    void AddAge(size_t age);
    void AddName(std::string name, bool is_male);
    void BuildWealthy();
public:
    DemographicIndicators();
    void ReadPeople(std::istream& input);
    size_t AgeCount(int age) const; // количество людей, возраст которых не меньше age
    uint64_t WealtyCount(int top) const; // суммарный доход первых top богатейших людей
    std::string TopName(bool is_male) const; // М\Ж имя, которое встречается больше всего раз
};
