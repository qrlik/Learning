#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

struct Record
{ // структура данных
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

struct IdData
{ // структура для хранения данных и итераторов, для быстрой работы функций
    Record record;
    multimap<int, const Record*>::iterator timestamp;
    multimap<int, const Record*>::iterator karma;
    list<const Record*>::iterator user;
};

// Реализуйте этот класс
class Database
{
    unordered_map<string, IdData> data;                 // id - Records
    multimap<int, const Record*> timestamp_si;          // вторичный индекс для timestamp
    multimap<int, const Record*> karma_si;              // вторичный индекс для karma
    unordered_map<string, list<const Record*>> user_si; // вторичный индекс для user_si
    template<typename T, typename Callback> void RangeBy(const multimap<int, const Record*>& cont, const T& begin, const T& end, Callback callback) const
    { // итерация по [begin,end] пока Callback возвращает true
        auto r_begin = cont.lower_bound(begin);
        auto r_end = cont.upper_bound(end);
        for (auto start = r_begin; start != r_end; ++start)
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
        auto [it, is_insert] = data.insert({ record.id, {record} }); // пробуем вставить пару
        if (is_insert)
        { // вставка произошла, нужно добавить данные в индексы и обновить в SecondIndexIt
            IdData& data = it->second;
            const Record* ptr = &data.record;
            data.timestamp = timestamp_si.emplace(ptr->timestamp, ptr);
            data.karma = karma_si.emplace(ptr->karma, ptr);
            data.user = user_si[ptr->user].insert(user_si[ptr->user].end(), ptr);
        }
        return is_insert;
    }

    const Record* GetById(const string& id) const // Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id
    {
        if (auto it = data.find(id); it != data.end())
        {
            return &it->second.record;
        }
        return nullptr;
    }

    bool Erase(const string& id) // Операция Erase должна возвращать true, если удалось удалить элемент с заданным id, и false, если элемент с заданным id не был найден.
    { // В последнем случае состояние базы данных не должно меняться
        if (auto search = data.find(id); search != data.end())
        {
            const IdData& id_data = search->second;
            timestamp_si.erase(id_data.timestamp);
            karma_si.erase(id_data.karma);
            user_si[search->second.record.user].erase(id_data.user);
            data.erase(search);
            return true;
        }
        return false;
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
        if (auto search = user_si.find(user); search != user_si.end())
        {
            for (auto ptr : search->second)
            {
                if (!callback(*ptr))
                {
                    break;
                }
            }
        }
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
