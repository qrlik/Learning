#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record
{ // структура данных
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

struct SecondIndex
{ // структура для хранения указателя и итераторов, для быстрой работы функций
    const Record* record;
    multimap<int, const Record*>::iterator timestamp;
    multimap<int, const Record*>::iterator karma;
    multimap<string, const Record*>::iterator user;
};

// Реализуйте этот класс
class Database
{
    unordered_map<string, Record> data; // id - Records
    unordered_map<string, SecondIndex> ids; // словарь по id с быстрым доступом к данным
    multimap<int, const Record*> timestamp_si; // вторичный индекс для timestamp
    multimap<int, const Record*> karma_si; // вторичный индекс для karma
    multimap<string, const Record*> user_si; // вторичный индекс для user_si
    template<typename C, typename T, typename Callback> void RangeBy(const C& cont, const T& begin, const T& end, Callback callback) const
    { // итерация по [begin,end] пока Callback возвращает true
        for (auto start = cont.lower_bound(begin); start != cont.upper_bound(end); ++start)
        {
            if (!callback(*start->second))
            {
                break;
            }
        }
    }

public:
    bool Put(const Record& record) // Операция Put должна возвращать true, если вставка удалась, и false, если вставка не удалась, потому что в базе данных уже есть запись с таким же id.
    {// В последнем случае состояние базы данных не должно меняться
        auto [it, is_insert] = data.emplace(record.id, record); // пробуем вставить пару
        if (is_insert)
        { // вставка произошла, нужно обновить данные
            auto time_it = timestamp_si.emplace(record.timestamp, &it->second); // обновляем индексы
            auto karma_it = karma_si.emplace(record.karma, &it->second);
            auto user_it = user_si.emplace(record.user, &it->second);
            SecondIndex tmp { &it->second, time_it, karma_it, user_it };
            ids.emplace(record.id, move(tmp));
        }
        return is_insert;
    }

    const Record* GetById(const string& id) const // Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id
    {
        try
        {
            return &data.at(id);
        }
        catch (out_of_range&)
        {
            return nullptr;
        }
    }

    bool Erase(const string& id) // Операция Erase должна возвращать true, если удалось удалить элемент с заданным id, и false, если элемент с заданным id не был найден.
    { // В последнем случае состояние базы данных не должно меняться
        bool result = data.erase(id) != 0;
        if (result)
        { // если удаление произошло
            auto old = ids.find(id);
            timestamp_si.erase(old->second.timestamp);
            karma_si.erase(old->second.karma);
            user_si.erase(old->second.user);
            ids.erase(old);
        }
        return result;
    }

    // Подразумевается, что callback должен возвращать true, если требуется продолжить обход найденных записей, и false в противном случае.
    // Например, это позволит вывести первые 10 записей или найти первую запись, удовлетворяющую дополнительному критерию
    // Все границы интервалов - включительные, [low, high]
    template <typename Callback> void RangeByTimestamp(int low, int high, Callback callback) const
    {
        RangeBy(timestamp_si, low, high, callback);
    }

    template <typename Callback> void RangeByKarma(int low, int high, Callback callback) const
    {
        RangeBy(karma_si, low, high, callback);
    }

    template <typename Callback> void AllByUser(const string& user, Callback callback) const
    {
        RangeBy(user_si, user, user, callback);
    }
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
    db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
    db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
    db.Erase("id");
    db.Put({ "id", final_body, "not-master", 1536107260, -10 });

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
