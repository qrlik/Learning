#include "test_runner.h"

#include <functional>
#include <string>
#include <optional>
using namespace std;

/*
Иногда на практике встречаются объекты, создание которых занимает значительное время,
однако они бывают нужны не при каждом запуске программы. Например, клиент системы контроля версий может обращаться к локальным файлам,
а может — к удалённому репозиторию. Тогда устанавливать соединение с удалённым репозиторием можно,
только если пользователь решил к нему обратиться.

Для представления таких объектов может быть полезен шаблон LazyValue<T>.
У него есть константный метод const T& Get(), который возвращает ссылку на созданный объект.
В своём конструкторе LazyValue принимает инициализирующую функцию,
которая используется при первом вызове метода Get для создания хранимого объекта.
Последующие вызовы Get просто возвращают ссылку на хранимый объект.

Таким образом, объект класса LazyValue<T> не тратит время на создание хранимого объекта до первого вызова метода Get.
*/

template <typename T> class LazyValue
{
    std::function<T()> _init;
    mutable std::optional<T> _value;
public:
    explicit LazyValue(std::function<T()> init) : _init(std::move(init)) {}

    bool HasValue() const
    {
        return _value.has_value();
    }
    const T& Get() const
    {
        if (!_value)
        {
            _value = _init();
        }
        return _value.value(); // *_value
    }

};

void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string] { return big_string; });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
            });
    }
    ASSERT(!called);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    return 0;
}
