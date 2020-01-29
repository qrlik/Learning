#include <string>
#include <string_view>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
using namespace std;

/* Parsers */
template <typename Number>
Number ReadNumberOnLine(std::istream& stream) {
    Number number;
    stream >> number;
    string dummy;
    getline(stream, dummy);
    return number;
}

int ConvertToInt(string_view str)
{
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

pair<string_view, optional<string_view>> SplitTwoStrict(string_view str, string_view delimiter = " ")
{
	const size_t pos = str.find(delimiter);
	if (pos == str.npos)
	{
		return { str, nullopt };
	}
	else
	{
		return { str.substr(0, pos), str.substr(pos + delimiter.length()) };
	}
}

pair<string_view, string_view> SplitTwo(string_view str, string_view delimiter = " ")
{
	const auto [lhs, rhs_opt] = SplitTwoStrict(str, delimiter);
	return { lhs, rhs_opt.value_or("") };
}

string_view ReadToken(string_view& str, string_view delimiter = " ")
{
	const auto [lhs, rhs] = SplitTwo(str, delimiter);
	str = rhs;
	return lhs;
}

/* number system */
enum class N_S { 
    G,
	KG,
    ML,
	L,
    CNT,
	TENS
};

const unordered_map<string_view, N_S> STR_TO_SYSTEM{
	{"g", N_S::G},
	{"kg", N_S::KG},
	{"ml", N_S::ML},
	{"l", N_S::L},
	{"cnt", N_S::CNT},
	{"tens", N_S::TENS},
};

struct Unit {
	N_S system;
	uint16_t amount;
};

Unit ConvertToUnit(string_view str) {
	if (const auto it = STR_TO_SYSTEM.find(str);
		it != STR_TO_SYSTEM.end()) {
		N_S system = it->second;
		uint16_t amount = (system == N_S::G || system == N_S::ML || system == N_S::CNT) ? 1 : 1000;
		amount = (system == N_S::TENS) ? 10 : amount;
		return Unit{ system, amount };
	}
	else { /* or std::optional */
		ostringstream error;
		error << "Wrong Unit format in \'" << str << "\'";
		throw invalid_argument(error.str());
	}
}

struct Ingredient {
    string _name;
	Unit _unit;
	bool operator==(const Ingredient& rhs) const {
		return _name == rhs._name;
	}
};

struct IngredientHasher {
	size_t operator()(const Ingredient& i) const {
		return shash(i._name);
	}
	hash<string> shash;
};

using Ingredients = unordered_map<Ingredient, size_t, IngredientHasher>;

class Dish {
    string _name;
	Ingredients _ingredients;
	size_t _tasters;
public:
	Dish(string n, Ingredients i, size_t t) :
		_name(move(n)), _ingredients(move(i)), _tasters(t) {}
	const Ingredients& getIngredients() const {
		return _ingredients;
	}
	size_t getTasters() const {
		return _tasters;
	}
	const string& getName() const {
		return _name;
	}
};

/* Parsing ingredient */
Ingredients::value_type ParseIngredient(string_view ingredient_info) {
	string name = string(ReadToken(ingredient_info));
	size_t amount = static_cast<size_t>((ConvertToInt(ReadToken(ingredient_info))));
	Unit unit = ConvertToUnit(ingredient_info);
	return pair(Ingredient{ name, unit }, amount);
}

Ingredients ParseIngredients(size_t count, istream& in_stream) {
	Ingredients ingredients;
	string ingredient_info;
	for (size_t i = 0; i < count && getline(in_stream, ingredient_info); ++i) {
		ingredients.insert(ParseIngredient(ingredient_info));
	}
	return ingredients;
}

/* Parsing dish */
Dish ParseDish(string_view dish_info, istream& in_stream) {
	string name = string(ReadToken(dish_info));
	size_t tasters = static_cast<size_t>(ConvertToInt(ReadToken(dish_info)));
	size_t ingredients_count = static_cast<size_t>(ConvertToInt(dish_info));
	Ingredients ingredients = ParseIngredients(ingredients_count, in_stream);

	return Dish(move(name), move(ingredients), tasters);
}

vector<Dish> ParseDishes(istream& in_stream = cin) {
	const size_t count = ReadNumberOnLine<size_t>(in_stream);

	vector<Dish> dishes;
	dishes.reserve(count);

	string dish_info;
	for (size_t i = 0; i < count && getline(in_stream, dish_info); ++i) {
		dishes.push_back(ParseDish(dish_info, in_stream));
	}
	return dishes;
}

/* Parsing price catalog */
struct IngredientPrice {
	uint16_t _price;
	uint16_t _amount;
	Unit _unit;
};
using PriceCatalog = unordered_map<string, IngredientPrice>;

PriceCatalog::value_type ParseIngredientPrice(string_view ingredient_info) {
	string name = string(ReadToken(ingredient_info));
	uint16_t price = static_cast<uint16_t>(ConvertToInt(ReadToken(ingredient_info)));
	uint16_t amount = static_cast<uint16_t>(ConvertToInt(ReadToken(ingredient_info)));
	Unit unit = ConvertToUnit(ingredient_info);
	return pair(move(name), IngredientPrice{ price, amount, unit });
}

PriceCatalog ParsePriceCatalog(istream& in_stream = cin) {
	const size_t count = ReadNumberOnLine<size_t>(in_stream);

	PriceCatalog catalog;
	string price_info;
	for (size_t i = 0; i < count && getline(in_stream, price_info); ++i) {
		catalog.insert(ParseIngredientPrice(price_info));
	}
	return catalog;
}

/* Parsing ingredient catalog */
struct Composition {
	double _protein;
	double _fat; /*no bounds*/
	double _carbo;  /* (-~ ; 1000] */
	double _energy;
	Composition operator*(double factor) const {
		return { _protein * factor, _fat * factor,
		_carbo * factor, _energy * factor };
	}
	const Composition& operator +=(const Composition& other) {
		_protein += other._protein;
		_fat += other._fat;
		_carbo += other._carbo;
		_energy += other._energy;
		return *this;
	}
};

struct IngredientData {
	Composition _composition;
	Unit _unit;
	uint16_t _amount;
};
using IngredientCatalog = unordered_map<string, IngredientData>;

IngredientCatalog::value_type ParseIngredientData(string_view ingredient_info) {
	string name = string(ReadToken(ingredient_info));
	uint16_t amount = static_cast<uint16_t>(ConvertToInt(ReadToken(ingredient_info)));
	Unit unit = ConvertToUnit(ReadToken(ingredient_info));

	Composition composition{};
	composition._protein = ConvertToDouble(ReadToken(ingredient_info));
	composition._fat = ConvertToDouble(ReadToken(ingredient_info));
	composition._carbo = ConvertToDouble(ReadToken(ingredient_info));
	composition._energy = ConvertToDouble(ingredient_info);

	return pair(move(name), IngredientData{ composition, unit, amount });
}

IngredientCatalog ParseIngredientCatalog(istream& in_stream = cin) {
	const size_t count = ReadNumberOnLine<size_t>(in_stream);

	IngredientCatalog catalog;
	string data_info;
	for (size_t i = 0; i < count && getline(in_stream, data_info); ++i) {
		catalog.insert(ParseIngredientData(data_info));
	}
	return catalog;
}

using DishComposition = unordered_map<string, Composition>;

/* суммируем все ингредиенты необходимые для блюд */
pair<Ingredients, DishComposition> SumAllDishes(const vector<Dish>& dishes,
	const IngredientCatalog& catalog, const PriceCatalog& prices) {
	Ingredients summary; // общее количество ингредиентов
	DishComposition dishes_composition;

	for (const auto& dish : dishes) {
		Composition& dish_composition = dishes_composition[dish.getName()];
		for (const auto& ingredient : dish.getIngredients()) {
			summary[ingredient.first] += dish.getTasters() * ingredient.second;

			const IngredientData& ing_data = catalog.at(ingredient.first._name);
			const size_t amount = ingredient.second * ingredient.first._unit.amount; // необходимое количество
			const size_t catalog_amount = ing_data._amount * ing_data._unit.amount; // количество в каталоге
			const double factor = static_cast<double>(amount) / catalog_amount;
			dish_composition += ing_data._composition * factor;
		}
	}
	Ingredient tmp{};
	for (const auto& ingredient : prices) {
		tmp = { ingredient.first, ingredient.second._unit };
		if (auto it = summary.find(tmp); it == summary.end()) {
			summary.insert({ move(tmp), 0 });
		}
	}

	return { move(summary), move(dishes_composition) };
}

pair<Ingredients, size_t> CalculateValues(const Ingredients& ingredients, const PriceCatalog& prices) {
	Ingredients packing_count;
	size_t summ_price = 0;

	for (const auto& ingredient : ingredients) {
		const string& name = ingredient.first._name;
		const IngredientPrice& price = prices.at(name);

		const size_t amount = ingredient.second * ingredient.first._unit.amount; // необходимое количество
		const size_t one_package = price._amount * price._unit.amount; // одна упаковка
		const size_t pack_count = [=]() {
			bool is_not_full_pack = (amount % one_package) ? 1 : 0;
			return amount / one_package + is_not_full_pack;
		}();

		packing_count[ingredient.first] = pack_count;
		summ_price += pack_count * price._price;
	}

	return { move(packing_count), summ_price };
}

void PrintResults(size_t check, const Ingredients& packeges, const DishComposition& compositions, ostream& os_stream = cout) {
	os_stream << check;
	for (const auto& ingredient_packages : packeges) {
		os_stream << '\n' << ingredient_packages.first._name << " " << ingredient_packages.second;
	}
	for (const auto& dish_composition : compositions) {
		const Composition& comp = dish_composition.second;
		os_stream << '\n' << dish_composition.first << " " << comp._protein << " " <<
			comp._fat << " " << comp._carbo << " " << comp._energy;
	}
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
//	vector<Dish> dishes = ParseDishes(input);
//	PriceCatalog prices = ParsePriceCatalog(input);
//	IngredientCatalog catalog = ParseIngredientCatalog(input);
//
//	auto [ingredients, composition] = SumAllDishes(dishes, catalog, prices);
//	auto [packages, check] = CalculateValues(ingredients, prices);
//
//	ostringstream output;
//	PrintResults(check, packages, composition, output);
//	ASSERT_EQUAL(output.str(), "734\n"
//		"egg 4\n"
//		"milk 2\n"
//		"sausage 2\n"
//		"butter 1\n"
//		"cream 0\n"
//		"salt 1\n"
//		"toasted_bread 1\n"
//		"sandwich 6.00 13.29 21.50 228.3\n"
//		"omelet 57.360 57.540 5.314 177.800\n"
//	);
//}
//
//void TestAll() {
//	TestRunner tr;
//	RUN_TEST(tr, Test);
//}

int main()
{
	//TestAll();

	vector<Dish> dishes = ParseDishes();
	PriceCatalog prices = ParsePriceCatalog();
	IngredientCatalog catalog = ParseIngredientCatalog();

	auto [ingredients, composition] = SumAllDishes(dishes, catalog, prices);
	auto [packages, check] = CalculateValues(ingredients, prices);

	PrintResults(check, packages, composition);
    return 0;
}
