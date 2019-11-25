class Editor
{
	deque<char> text;
	deque<char>::iterator pos;
	string buffer;
public:
	// Реализуйте конструктор по умолчанию и объявленные методы
	Editor() : pos(text.begin()) {}

	void Left()
	{
		if (pos > text.begin())
		{
			--pos;
		}
	}

	void Right()
	{
		if (pos < text.end())
		{
			++pos;
		}
	}

	void Insert(char token)
	{
		pos = text.insert(pos, token); // вставка символа и сдвиг позиции
		++pos;
	}

	void Copy(size_t tokens = 1)
	{ // скопировать не более tokens символов с pos в буффер
		if (tokens)
		{
			//auto end_it = (pos + tokens >= text.end()) ? text.end() : pos + tokens;
			auto end_it = (distance(pos, text.end()) <= static_cast<int>(tokens)) ? text.end() : pos + tokens;
			buffer.assign(pos,end_it);
		}
		else
		{ // Копирование или вырезание фрагмента нулевой длины не оказывает влияния на текст, но опустошает буфер обмена
			buffer.clear();
		}
	}

	void Cut(size_t tokens = 1)
	{ // вырезать не более tokens символов с pos в буффер
		Copy(tokens);
		auto end_it = (distance(pos, text.end()) <= static_cast<int>(tokens)) ? text.end() : pos + tokens;
		pos = text.erase(pos, end_it);
	}

	void Paste()
	{
		pos = text.insert(pos, buffer.begin(), buffer.end()); // вставка строки и сдвиг позиции
		pos += buffer.size();
	}

	string GetText() const
	{
		return string(text.begin(), text.end());
	}
};
