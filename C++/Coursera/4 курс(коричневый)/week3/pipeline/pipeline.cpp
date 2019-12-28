#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Email {
    string from;
    string to;
    string body;
};

class Worker {
    mutable unique_ptr<Worker> next_worker;
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const
    {
        if (next_worker)
        {
            next_worker->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next)
    {
        if (next_worker) // см. PipelineBuilder, с реализацией через вектор эти проверки не нужны
        {
            next_worker->SetNext(move(next));
        }
        else
        {
            next_worker = move(next);
        }
    }
};


class Reader : public Worker {
    istream& input;
public:
    Reader(istream& is) : input(is) {}
    void Process(unique_ptr<Email> email) override
    {
        PassOn(move(email));
    }
    void Run() override
    {
        Email new_email;
        while (getline(input, new_email.from) &&
            getline(input, new_email.to) && getline(input, new_email.body))
        {
            Process(make_unique<Email>(new_email));
        }
    }
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
private:
    Function predicate;
public:
    Filter(Function p) : predicate(move(p)) {}
    void Process(unique_ptr<Email> email) override
    {
        if (predicate(*email))
        {
            PassOn(move(email));
        }
    }
};


class Copier : public Worker {
    string second_recipient;
public:
    Copier(string str) : second_recipient(move(str)) {}
    void Process(unique_ptr<Email> email) override
    {
        if (second_recipient != email->to)
        {
            auto copy = make_unique<Email>(Email{ email->from, second_recipient, email->body });
            PassOn(move(email));
            PassOn(move(copy));
        }
        else
        {
            PassOn(move(email));
        }
    }
};


class Sender : public Worker {
    ostream& output;
public:
    Sender(ostream& os) : output(os) {}
    void Process(unique_ptr<Email> email) override
    {
        output << email->from << '\n' <<
            email->to << '\n' <<
            email->body << '\n';
        PassOn(move(email));
    }
};


// реализуйте класс
class PipelineBuilder {
    unique_ptr<Worker> worker; // лучше как в решении сделать вектор, а в методе Build разово схлопнуть его в один Worker. Так не будет рекурсии в SetNext
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) : worker(make_unique<Reader>(in)) {}

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter)
    {
        worker->SetNext(make_unique<Filter>(move(filter)));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient)
    {
        worker->SetNext(make_unique<Copier>(recipient));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out)
    {
        worker->SetNext(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build()
    {
        return move(worker);
    }
};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
        );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
        });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
        );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
