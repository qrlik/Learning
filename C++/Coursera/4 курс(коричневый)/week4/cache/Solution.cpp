#include "Common.h"

#include <list>
#include <unordered_map>
#include <mutex>

using namespace std;

class LruCache : public ICache {
    unordered_map<string, list<shared_ptr<const IBook>>::iterator> search; // поиск наличия книги в кэше по названию
    list<shared_ptr<const IBook>> data; // список книг по их рангу, от максимального до минимального
    mutex cache_mutex;
    shared_ptr<IBooksUnpacker> books_unpacker; // распаковщик книг
    Settings settings; // настройки кэша
    size_t free_memory;
public:
    LruCache(shared_ptr<IBooksUnpacker> unpacker,const Settings& sets): books_unpacker(move(unpacker)), settings(sets), free_memory(sets.max_memory) {
        // реализуйте метод
    }

    BookPtr GetBook(const string& book_name) override { // multi-thread
        lock_guard<mutex> cache_guard(cache_mutex);
        if (auto book = search.find(book_name); book != search.end())
        { // если книга уже в кэше
            data.push_front(move(*book->second)); // меняем ранг книги на максимальный
            data.erase(book->second); // удаляем старый нод
            book->second = data.begin(); // обновляем итератор
        }
        else
        { // если книги в кэше нет
            unique_ptr<const IBook> new_book = books_unpacker->UnpackBook(book_name); // распаковываем книгу
            size_t book_size = new_book->GetContent().size(); // размер новой книги

            if (book_size > settings.max_memory)
            { // если книга больше допустимого размера кэша, то после вызова метода кэш остаётся пустым, то есть книга в него не добавляется
                search.clear();
                data.clear();
                free_memory = settings.max_memory;
                return new_book; // unique_ptr -> shared_ptr
            }

            while (book_size > free_memory)
            { // если свободного места не хватает, удаляем книги с наименьшим рангом (хвост списка)
                const IBook& book_to_delete = *data.back();
                search.erase(book_to_delete.GetName());
                free_memory += book_to_delete.GetContent().size();
                data.pop_back();
            }

            data.push_front(move(new_book)); // добавляем книгу в кэш
            search[book_name] = data.begin();
            free_memory -= book_size;
        }
        return data.front();
    }
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings) {
    return make_unique<LruCache>(move(books_unpacker), settings);
}
