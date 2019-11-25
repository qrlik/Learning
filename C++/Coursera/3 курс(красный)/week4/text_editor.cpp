#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor
{
	list<char> text;
	list<char>::iterator pos;
	string buffer;

	list<char>::iterator Iteration(list<char>::iterator pos, int count)
	{
		while (pos != text.end() && count > 0)
		{
			++pos;
			--count;
		}
		while (pos != text.begin() && count < 0)
		{
			--pos;
			++count;
		}
		return pos;
	}
public:
	// Реализуйте конструктор по умолчанию и объявленные методы
	Editor() : pos(text.begin()) {}

	void Left()
	{ // сдвинуть курсор влево
		pos = Iteration(pos, -1);
	}

	void Right()
	{ // сдвинуть курсор вправо
		pos = Iteration(pos, 1);
	}

	void Insert(char token)
	{ // вставить символ token
		text.insert(pos, token);
	}

	void Paste()
	{ // вставить содержимое буфера в текущую позицию курсора
		text.insert(pos, buffer.begin(), buffer.end());
	}

	void Copy(size_t tokens = 1)
	{ // скопировать не более tokens символов с pos в буффер
		buffer.assign(pos, Iteration(pos, tokens));
	}

	void Cut(size_t tokens = 1)
	{ // вырезать не более tokens символов с pos в буффер
		Copy(tokens);
		pos = text.erase(pos, Iteration(pos, tokens));
	}

	string GetText() const
	{
		return string(text.begin(), text.end());
	}
};

int main() {
	Editor editor;
	const string text = "hello, world";
	for (char c : text) {
		editor.Insert(c);
	}
	// Текущее состояние редактора: `hello, world|`
	for (size_t i = 0; i < text.size(); ++i) {
		editor.Left();
	}
	// Текущее состояние редактора: `|hello, world`
	editor.Cut(7);
	// Текущее состояние редактора: `|world`
	// в буфере обмена находится текст `hello, `
	for (size_t i = 0; i < 5; ++i) {
		editor.Right();
	}
	// Текущее состояние редактора: `world|`
	editor.Insert(',');
	editor.Insert(' ');
	// Текущее состояние редактора: `world, |`
	editor.Paste();
	// Текущее состояние редактора: `world, hello, |`
	editor.Left();
	editor.Left();
	//Текущее состояние редактора: `world, hello|, `
	editor.Cut(3); // Будут вырезаны 2 символа
	// Текущее состояние редактора: `world, hello|`
	cout << editor.GetText();

	return 0;
}
