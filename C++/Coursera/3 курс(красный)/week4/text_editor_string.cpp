class Editor
{
	string text;
	string buffer;
	size_t pos;
public:
	// Реализуйте конструктор по умолчанию и объявленные методы
	Editor() : pos(0)
	{
		text.reserve(1'000'000);
		buffer.reserve(1'000'000);
	}

	void Left()
	{
		if (pos)
		{
			--pos;
		}
	}

	void Right()
	{
		if (pos < text.size())
		{
			++pos;
		}
	}

	void Insert(char token)
	{
		text.insert(pos++, 1, token); // вставка символа и сдвиг позиции
	}

	void Copy(size_t tokens = 1)
	{ // скопировать не более tokens символов с pos в буффер
		if (tokens)
		{
			buffer = text.substr(pos, tokens);
		}
		else
		{ // Копирование или вырезание фрагмента нулевой длины не оказывает влияния на текст, но опустошает буфер обмена
			buffer.clear();
		}
	}

	void Cut(size_t tokens = 1)
	{ // вырезать не более tokens символов с pos в буффер
		Copy(tokens);
		text.erase(pos, tokens);
	}

	void Paste()
	{
		text.insert(pos, buffer); // вставка строки и сдвиг позиции
		pos += buffer.size();
	}

	string GetText() const
	{
		return text;
	}
};
