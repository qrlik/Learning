#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
using namespace std;

/* Parsers */
template <typename Number>
Number ReadDishesAmount(std::istream& stream) {
	Number number;
	stream >> number;
	stream.ignore(numeric_limits<streamsize>::max(), '\n');
	return number;
}

int ConvertToInt(string_view str) {
	size_t pos;
	const int result = stoi(string(str), &pos);
	if (pos != str.length())
	{
		ostringstream error;
		error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
		throw invalid_argument(error.str());
	}
	return result;
}

double ConvertToDouble(string_view str)
{
	size_t pos;
	const double result = stod(string(str), &pos);
	if (pos != str.length())
	{
		ostringstream error;
		error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
		throw invalid_argument(error.str());
	}
	return result;
}

string_view ReadPrefix(string_view& str, string_view delimiter = " ")
{
	string_view result;
	const size_t pos = str.find(delimiter);
	if (pos == str.npos) {
		result.swap(str);
	} else {
		result = str.substr(0, pos);
		str.remove_prefix(pos + delimiter.length());
	}
	return result;
}

using MaxType = uint64_t;
// 10^3(блюда) * 10^2(кол-во ингредиентов) * 10^3(кол-во ингредиента) * 10^3(гр\мл)
using ThsType = uint16_t; // для 1 <= x <= 1000

/* number system */
const unordered_map<string_view, ThsType> STR_TO_FACTOR{
	{"g", 1},
	{"kg", 1000},
	{"ml", 1},
	{"l", 1000},
	{"cnt", 1},
	{"tens", 10},
};

struct Ingredient {
	string _name;
	ThsType _amount{};
	ThsType _factor{};
};

struct Dish {
	string _name;
	vector<Ingredient> _ingredients;
	ThsType _tasters_amount{};
};

/* Parsing ingredient */
auto ParseIngredient(string_view ingredient_info) {
	Ingredient result;
	result._name = ReadPrefix(ingredient_info);
	result._amount = static_cast<ThsType>(ConvertToInt(ReadPrefix(ingredient_info)));
	result._factor = STR_TO_FACTOR.at(ingredient_info);
	return result;
}

auto ParseIngredients(int count, istream& in_stream) {
	vector<Ingredient> ingredients;
	ingredients.reserve(count);

	string ingredient_info;
	for (decltype(count) i = 0; i < count && getline(in_stream, ingredient_info); ++i) {
		ingredients.push_back(ParseIngredient(ingredient_info));
	}
	return ingredients;
}

/* Parsing dish */
auto ParseDish(string_view dish_info, istream& in_stream) {
	Dish result;
	result._name = ReadPrefix(dish_info);
	result._tasters_amount = static_cast<ThsType>(ConvertToInt(ReadPrefix(dish_info)));
	result._ingredients = ParseIngredients(ConvertToInt(dish_info), in_stream);

	return result;
}

auto ParseDishes(istream& in_stream) {
	const auto dishes_amount = ReadDishesAmount<ThsType>(in_stream);
	vector<Dish> dishes;
	dishes.reserve(dishes_amount);

	string dish_info;
	for (ThsType i = 0; i < dishes_amount && getline(in_stream, dish_info); ++i) {
		dishes.push_back(ParseDish(dish_info, in_stream));
	}
	return dishes;
}

/* Parsing price catalog */
struct IngredientPrice {
	ThsType _price{};
	ThsType _pack_amount{};
	ThsType _factor{};
};
using PriceCatalog = unordered_map<string, IngredientPrice>;

auto ParseIngredientPrice(string_view ingredient_info) {
	PriceCatalog::value_type result(ReadPrefix(ingredient_info), IngredientPrice{});
	auto& ing_price = result.second;
	ing_price._price= static_cast<ThsType>(ConvertToInt(ReadPrefix(ingredient_info)));
	ing_price._pack_amount = static_cast<ThsType>(ConvertToInt(ReadPrefix(ingredient_info)));
	ing_price._factor = STR_TO_FACTOR.at(ingredient_info);
	return result;
}

PriceCatalog ParsePriceCatalog(istream& in_stream) {
	const auto prices_amount = ReadDishesAmount<ThsType>(in_stream);

	PriceCatalog catalog;
	string price_info;
	for (ThsType i = 0; i < prices_amount && getline(in_stream, price_info); ++i) {
		catalog.insert(ParseIngredientPrice(price_info));
	}
	return catalog;
}

/* Parsing ingredient catalog */
struct Composition {
	double _protein{}; // белки
	double _fat{}; // жиры
	double _carbo{};  // углеводы
	double _energy{}; // эн. ценность
	Composition operator*(double factor) const {
		return { _protein * factor, _fat * factor,
		_carbo * factor, _energy * factor };
	}
	Composition& operator +=(const Composition& other) {
		_protein += other._protein;
		_fat += other._fat;
		_carbo += other._carbo;
		_energy += other._energy;
		return *this;
	}
};

struct IngredientData {
	Composition _composition;
	ThsType _amount{};
	ThsType _factor{};
};
using IngredientCatalog = unordered_map<string, IngredientData>;

auto ParseIngredientData(string_view ingredient_info) {
	IngredientCatalog::value_type result(ReadPrefix(ingredient_info), IngredientData{});
	auto& [composition, amount, factor] = result.second;

	amount = static_cast<ThsType>(ConvertToInt(ReadPrefix(ingredient_info)));
	factor = STR_TO_FACTOR.at(ReadPrefix(ingredient_info));

	composition._protein = ConvertToDouble(ReadPrefix(ingredient_info));
	composition._fat = ConvertToDouble(ReadPrefix(ingredient_info));
	composition._carbo = ConvertToDouble(ReadPrefix(ingredient_info));
	composition._energy = ConvertToDouble(ingredient_info);

	return result;
}

IngredientCatalog ParseIngredientCatalog(istream& in_stream) {
	const auto ing_amount = ReadDishesAmount<ThsType>(in_stream);

	IngredientCatalog catalog;
	string data_info;
	for (ThsType i = 0; i < ing_amount && getline(in_stream, data_info); ++i) {
		catalog.insert(ParseIngredientData(data_info));
	}
	return catalog;
}

/* суммируем все ингредиенты необходимые для блюд */
auto SumAllDishes(const vector<Dish>& dishes,
	const IngredientCatalog& catalog, const PriceCatalog& prices) {
	unordered_map<string, Composition> dishes_composition;
	unordered_map<string, MaxType> summary; // общее количество ингредиентов
	for (const auto& ing : prices) {
		summary.emplace(ing.first, 0);
	}

	for (const auto& dish : dishes) {
		auto& composition = dishes_composition[dish._name];
		for (const auto& ingredient : dish._ingredients) {
			auto ing_amount = ingredient._amount * ingredient._factor; // количество в блюде
			summary[ingredient._name] += static_cast<MaxType>(dish._tasters_amount) * ing_amount; // необходимо всего

			const auto& ing_data = catalog.at(ingredient._name);
			const double factor = ing_amount / (static_cast<double>(ing_data._amount) * ing_data._factor);
			composition += ing_data._composition * factor;
		}
	}

	return pair{ move(summary), move(dishes_composition) };
}

auto CalculatePacksAndCost(const unordered_map<string, MaxType>& ingredients_summary, const PriceCatalog& prices) {
	decay_t<decltype(ingredients_summary)> packs_amount;
	MaxType cost = 0;

	for (const auto& [name, amount] : ingredients_summary) {
		const auto& ing_price = prices.at(name);

		const auto one_package = ing_price._pack_amount * ing_price._factor; // одна упаковка
		const bool is_not_full_pack = (amount % one_package) ? 1 : 0;
		auto packs = amount / one_package + is_not_full_pack;
		packs_amount[name] = packs;
		cost += packs * ing_price._price;
	}

	return pair{ move(packs_amount), cost };
}

void PrintResults(uint64_t cost, const unordered_map<string, MaxType>& packs,
	const unordered_map<string, Composition>& dish_composition, ostream& out_stream) {
	out_stream << cost;
	for (const auto& [name, packs] : packs) {
		out_stream << '\n' << name << " " << packs;
	}
	for (const auto& [name, composition] : dish_composition) {
		out_stream << '\n' << name << " " << composition._protein << " " <<
			composition._fat << " " << composition._carbo << " " << composition._energy;
	}
}

void ComputeAndPrint(istream& is, ostream& os) {
	auto dishes = ParseDishes(is); // out_of_range, invalid_argument
	auto prices = ParsePriceCatalog(is); // out_of_range, invalid_argument
	auto catalog = ParseIngredientCatalog(is); // out_of_range, invalid_argument

	auto [ingredients_summary, dish_composition] = SumAllDishes(dishes, catalog, prices);
	auto [packs, cost] = CalculatePacksAndCost(ingredients_summary, prices); // out_of_range

	PrintResults(cost, packs, dish_composition, os);
}
//
//#include "test_runner.h"
//void Test() {
//	istringstream input("2\n"
//		"sandwich 7 3\n"
//		"butter 10 g\n"
//		"toasted_bread 2 cnt\n"
//		"sausage 30 g\n"
//		"omelet 9 4\n"
//		"egg 4 cnt\n"
//		"milk 120 ml\n"
//		"salt 1 g\n"
//		"sausage 50 g\n"
//		"7\n"
//		"egg 61 1 tens\n"
//		"milk 58 1 l\n"
//		"sausage 100 480 g\n"
//		"butter 120 180 g\n"
//		"cream 100 350 g\n"
//		"salt 14 1000 g\n"
//		"toasted_bread 40 20 cnt\n"
//		"8\n"
//		"egg 1 cnt 13 12 1 16.4\n"
//		"milk 1 l 3 4.5 4.7 60\n"
//		"chocolate 90 g 6.8 36.3 47.1 546\n"
//		"salt 1 kg 0 0 0 0\n"
//		"strawberry 100 g 0.4 0.1 7 35\n"
//		"sausage 100 g 10 18 1.5 210\n"
//		"toasted_bread 5 cnt 7.3 1.6 52.3 248\n"
//		"butter 100 g 0.8 72.5 1.3 661\n"
//	);
//
//	ostringstream output;
//	ComputeAndPrint(input, output);
//	ASSERT_EQUAL(output.str(), "734\n"
//		"egg 4\n"
//		"milk 2\n"
//		"toasted_bread 1\n"
//		"sausage 2\n"
//		"butter 1\n"
//		"cream 0\n"
//		"salt 1\n"
//		"sandwich 6 13.29 21.5 228.3\n"
//		"omelet 57.36 57.54 5.314 177.8"
//	);
//}
//
//void TestAll() {
//	TestRunner tr;
//	RUN_TEST(tr, Test);
//}

int main() {
	//TestAll();
	ComputeAndPrint(cin, cout);
	return 0;
}
