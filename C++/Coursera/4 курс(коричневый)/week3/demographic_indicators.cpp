#include "demographic_indicators.h"

#include <algorithm>
#include <iostream>

using namespace std;

DemographicIndicators::DemographicIndicators() : _age(201, 0), // максимальный возраст - 200
_top_male_name(0, {}), _top_female_name(0, {}) {}

void DemographicIndicators::AddAge(size_t age)
{
    if (age < _age.size())
    {
        for (size_t i = 0; i <= age; ++i)
        {
            ++_age[i];
        }
    }
}

void DemographicIndicators::AddName(string name, bool is_male)
{
    unordered_map<string, size_t>& counter = (is_male) ? _male_name : _female_name; // выбираем нужные данные
    pair<size_t, string>& top_name = (is_male) ? _top_male_name : _top_female_name;

    size_t& name_count = ++counter[name];
    if (name_count > top_name.first)
    {
        top_name.first = name_count;
        top_name.second = move(name);
    }
    else if (name_count == top_name.first && name < top_name.second)
    { // Если таких имён несколько, программа должна вывести лексикографически минимальное
        top_name.second = move(name);
    }
}

void DemographicIndicators::BuildWealthy()
{
    sort(_wealthy.begin(), _wealthy.end(), greater<uint64_t>()); // сортируем по убыванию
    uint64_t sum = 0;
    for (size_t i = 0; i < _wealthy.size(); ++i)
    {
        sum += _wealthy[i]; // Добавляем к общему капиталу i-самых богатых людей
        _wealthy[i] = sum;
    }
}

void DemographicIndicators::ReadPeople(istream& input) {
    int count;
    input >> count;
    if (count <= 0)
    {
        return;
    }
    _wealthy.resize(count);

    Person p;
    char gender;
    for (int i = 0; i < count; ++i)
    {
        input >> p.name >> p.age >> p.income >> gender; // считываение данных
        p.is_male = gender == 'M';

        AddAge(p.age); // счетчик возрастов
        _wealthy[i] = p.income; // данные для диаграммы капиталов
        AddName(move(p.name), p.is_male); // счетчик имен
    }
    BuildWealthy(); // составляем диаграмму капиталов
}

size_t DemographicIndicators::AgeCount(int age) const
{// количество людей, возраст которых не меньше age
    if (age < 0)
    {
        return _age[0];
    }
    return _age[age];
}

uint64_t DemographicIndicators::WealtyCount(int top) const
{// суммарный доход первых top богатейших людей
    if (top <= 0)
    {
        return 0;
    }
    return _wealthy[top - 1];
}

string DemographicIndicators::TopName(bool is_male) const
{// М\Ж имя, которое встречается больше всего раз
    const pair<size_t, string>& top_name = (is_male) ? _top_male_name : _top_female_name;
    return top_name.second; // если счетчик нулевой, вернет пустую строку
}
