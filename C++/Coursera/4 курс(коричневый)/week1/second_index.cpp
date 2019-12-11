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

struct SecondIndexIt
{ // структура для хранения указателя и итераторов, для быстрой работы функций
    multimap<int, const Record*>::iterator timestamp;
    multimap<int, const Record*>::iterator karma;
    multimap<string, const Record*>::iterator user;
};

// Реализуйте этот класс
class Database
{
    unordered_map<string, Record> data; // id - Records
    unordered_map<string, SecondIndexIt> ids; // словарь по id с быстрым доступом к данным
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
        	const Record* ptr = &it->second;
            auto time_it = timestamp_si.emplace(record.timestamp, ptr); // обновляем индексы
            auto karma_it = karma_si.emplace(record.karma, ptr);
            auto user_it = user_si.emplace(record.user, ptr);
            SecondIndexIt tmp { time_it, karma_it, user_it };
            ids.emplace(record.id, move(tmp));
        }
        return is_insert;
    }

    const Record* GetById(const string& id) const // Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id
    {
        if (auto it = data.find(id); it != data.end())
        {
            return &it->second;
        }
        else
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
            const SecondIndexIt& iters= old->second;
            timestamp_si.erase(iters.timestamp);
            karma_si.erase(iters.karma);
            user_si.erase(iters.user);
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
