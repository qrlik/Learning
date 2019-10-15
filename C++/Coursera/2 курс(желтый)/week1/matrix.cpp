#include <iostream>
#include <vector>

using namespace std;

/*
В этой задаче вам надо разработать класс Matrix для представления целочисленной матрицы. Более подробно класс Matrix должен иметь:

конструктор по умолчанию, который создаёт матрицу с нулём строк и нулём столбцов

конструктор от двух целочисленных параметров: num_rows и num_cols, — которые задают количество строк и столбцов матрицы соответственно

метод Reset, принимающий два целочисленных параметра, которые задают количество строк и столбцов матрицы соответственно

константный метод At, который принимает номер строки и номер столбца (именно в этом порядке; нумерация строк и столбцов начинается с нуля)
и возвращает значение в соответствущей ячейке матрицы

неконстантный метод At с такими же параметрами, но возвращающий ссылку на значение в соответствущей ячейке матрицы
константные методы GetNumRows и GetNumColumns, которые возвращают количество строк и столбцов матрицы соответственно

Если количество строк или количество столбцов, переданное в конструктор класса Matrix или метод Reset, оказалось отрицательным, то должно
быть выброшено стандартное исключение out_of_range.

Это же исключение должен бросать метод At, если переданная в него ячейка выходит за границы матрицы.

Кроме того для класса Matrix должны быть определены следующие операторы:

оператор ввода из потока istream; формат ввода простой — сначала задаётся количество строк и столбцов (именно в этом порядке), а затем все
элементы матрицы: сначала элемент первой строки и первого столбца, затем элемент первой строки и второго столбца и так далее

оператор вывода в поток ostream; он должен выводить матрицу в том же формате, в каком её читает оператор ввода, — в первой строке количество
строк и столбцов, во второй — элементы первой строки, в третьей — элементы второй строки и т.д.

оператор проверки на равенство (==): он должен возвращать true, если сравниваемые матрицы имеют одинаковый размер и все их соответствующие
элементы равны между собой, в противном случае он должен возвращать false.

оператор сложения: он должен принимать две матрицы и возвращать новую матрицу, которая является их суммой; если переданные матрицы имеют
разные размеры этот оператор должен выбрасывать стандартное исключение invalid_argument.
*/

class Matrix
{
private:
	size_t m_rows;
	size_t m_columns;
	vector<vector<int>> m_data;
public:
	explicit Matrix(int rows = 0, int columns = 0)
	{
		Reset(rows, columns);
	}
	void Reset(int rows, int columns)
	{ // метод обновления матрицы
		if (rows < 0 || columns < 0)
		{
			throw out_of_range("Reset method oor");
		}
		if (rows == 0 || columns == 0)
		{ // определение пустой матрицы
			rows = columns = 0;
		}
		m_rows = rows;
		m_columns = columns;
		m_data.assign(rows, vector<int>(columns)); // матрица нулей
	}
	int At(int row, int column) const
	{
		return m_data.at(row).at(column);
	}
	int& At(int row, int column)
	{
		return m_data.at(row).at(column);
	}
	size_t GetNumRows() const
	{
		return m_rows;
	}
	size_t GetNumColumns() const
	{
		return m_columns;
	}
	bool operator==(const Matrix& rhs) const
	{
		return m_data == rhs.m_data;
	}
	Matrix operator+(const Matrix& rhs) const
	{
		if (m_rows == rhs.m_rows && m_columns == rhs.m_columns)
		{
			Matrix result(m_rows, m_columns);
			for (size_t i = 0; i < m_rows; ++i)
			{
				for (size_t j = 0; j < m_columns; ++j)
				{
					result.m_data[i][j] = m_data[i][j] + rhs.m_data[i][j];
				}
			}
			return result;
		}
		throw invalid_argument("operator+ invalid");
	}
};

istream& operator>>(istream& is, Matrix& matrix)
{
	int x, y;
	is >> x >> y;
	matrix.Reset(x, y);
	for (size_t i = 0; i < matrix.GetNumRows(); ++i)
	{
		for (size_t j = 0; j < matrix.GetNumColumns(); ++j)
		{
			is >> matrix.At(i, j);
		}
	}
	return is;
}

ostream& operator<<(ostream& os, const Matrix& matrix)
{
	os << matrix.GetNumRows() << ' ' << matrix.GetNumColumns() << endl;
	for (size_t i = 0; i < matrix.GetNumRows(); ++i)
	{
		for (size_t j = 0; j < matrix.GetNumColumns(); ++j)
		{
			if (j > 0)
			{
				os << ' ';
			}
			os << matrix.At(i, j);
		}
		os << endl;
	}
	return os;
}

int main() 
{
	try
	{
		Matrix my_matrix1, my_matrix2;
		cin >> my_matrix1 >> my_matrix2;
		cout << my_matrix1 + my_matrix2;
	}
	catch (logic_error & ex)
	{
		cout << ex.what() << endl;
	}
	return 0;
}
