#include <vector>
#include <iterator>
using namespace std;

/*
считаем, что нумерация элементов массива начинается с единицы
и входные данные корректны
*/

template <typename T>
void Merge(T* A, size_t p, size_t q, size_t r) {
	p -= 1;
	q -= 1;
	r -= 1;
	vector<T> Left(make_move_iterator(A + p), make_move_iterator(A + q + 1));
	vector<T> Right(make_move_iterator(A + q + 1), make_move_iterator(A + r + 1));
	typename vector<T>::iterator r_b = Right.begin(); // auto for c++11
	typename vector<T>::iterator r_e = Right.end();
	typename vector<T>::iterator l_e = Left.end();

	for (typename vector<T>::iterator l_b = Left.begin(); l_b != l_e; ++p) {
		if (r_b == r_e) {
			move(l_b, l_e, A + p);
			break;
		}
		else if (*l_b <= *r_b) {
			A[p] = move(*l_b);
			++l_b;
		}
		else {
			A[p] = move(*r_b);
			++r_b;
		}
	}
	move(r_b, r_e, A + p);
}

template <typename T>
void Sort(T* A, size_t p, size_t r) {
	if (!A || p >= r) {
		return;
	}
	size_t q = (p + r) / 2;
	Sort(A, p, q);
	Sort(A, q + 1, r);
	Merge(A, p, q, r);
}

int main()
{
	return 0;
}
