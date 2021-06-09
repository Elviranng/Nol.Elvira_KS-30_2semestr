#include <iostream>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <string>
#include <locale>

using namespace std;

template<typename T>
class MyList {

private:

	// Узел списка (состоит из значения, указателя на след. и пред. элемент списка)
	class Node {
	public:
		T* value = nullptr;  // Указатель на значение узла (т.к. шаблонный контейнер)
		Node* prev = nullptr;  // Указатель на пред. элемент
		Node* next = nullptr;  // Указатель на след. элемент

		// Конструктор по-умолчанию (для пустых элементов)
		Node() {}

		// Конструктор с параметрами
		Node(T value) {
			this->value = new T(value);
		}

		~Node() {
			if (this->value != nullptr) {
				delete value;
			}
		}
	};

	Node* first = nullptr;  // Указатель на начало списка
	Node* last = nullptr;  // Указатель на конец списка
	int elems = 0; // Кол-во элементов

public:

	// Итератор списка
	class Iterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using iterator_category = std::random_access_iterator_tag;

	protected:
		Node* pointer_node;  // Указатель на узел
		Node* pointer_last;

	public:
		Iterator(Node* pointer_node, Node* pointer_last) :
			pointer_node(pointer_node),
			pointer_last(pointer_last)
		{}

		// Перегрузка постфиксного инкремента (с фиктивным параметром)
		Iterator operator++(int) {
			Iterator tmp_it(this->pointer_node, this->pointer_last);
			this->pointer_node = this->pointer_node->next;
			return tmp_it;
		}

		// Перегрузка префиксного инкремента
		Iterator& operator++() {
			this->pointer_node = this->pointer_node->next;
			return (*this);
		}

		// Перегрузка постфиксного декремента (с фиктивным параметром)
		Iterator operator--(int) {
			Iterator tmp_it(this->pointer_node, this->pointer_last);
			if (this->pointer_node == nullptr) {
				this->pointer_node = this->pointer_last;
			}
			else {
				this->pointer_node = this->pointer_node->prev;
			}
			return tmp_it;
		}

		// Перегрузка префиксного декремента
		Iterator& operator--() {
			if (this->pointer_node == nullptr) {
				this->pointer_node = this->pointer_last;
			}
			else {
				this->pointer_node = this->pointer_node->prev;
			}
			return (*this);
		}

		// Перегрузка оператора разыменования
		T& operator*() const {
			if (this->pointer_node != nullptr) {
				return *(this->pointer_node->value);
			}
			else {
				throw std::runtime_error("Exception_runtime: empty node has no value");
			}
		}

		// Перегрузка оператора равенства
		bool operator==(const Iterator& b) const {
			return this->pointer_node == b.pointer_node;
		}

		// Перегрузка оператора неравенства
		bool operator!=(const Iterator& b) const {
			return this->pointer_node != b.pointer_node;
		}

		// Перегрузка оператора меньше
		bool operator<(const Iterator& b) const {
			return ((b - (*this)) > 0);
		}

		// Оператор сложения дифф типа с итератором
		Iterator operator+(difference_type x) const {
			Iterator tmp_it = *this;
			for (size_t i = 0; i < x; ++i) {
				++(tmp_it);
			}
			return tmp_it;
		}

		// Оператор вычитания дифф типа из итератора
		Iterator operator-(difference_type x) const {
			Iterator tmp_it = *this;
			for (size_t i = 0; i < x; ++i) {
				--(tmp_it);
			}
			return tmp_it;
		}

		// Оператор вычитания итераторов
		difference_type operator-(const Iterator& b) const {
			difference_type i = 0;
			Iterator tmp_it = *this;
			while (tmp_it.pointer_node != b.pointer_node) {
				(tmp_it)--;
				i++;
			}
			return i;
		}

		// Метод получения указателя на следующий элемент
		Node* getNext() const {
			return this->pointer_node->next;
		}
	};

	// Обратный итератор списка:
	class ReverseIterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using iterator_category = std::random_access_iterator_tag;

	protected:
		Node* pointer_node;  // Указатель на узел
		Node* pointer_last;

	public:
		ReverseIterator(Node* pointer_node, Node* pointer_last) :
			pointer_node(pointer_node),
			pointer_last(pointer_last)
		{}

		// Перегрузка постфиксного инкремента (с фиктивным параметром)
		ReverseIterator operator++(int) {
			ReverseIterator tmp_it(this->pointer_node, this->pointer_last);
			this->pointer_node = this->pointer_node->prev;
			return tmp_it;
		}

		// Перегрузка префиксного инкремента
		ReverseIterator& operator++() {
			this->pointer_node = this->pointer_node->prev;
			return (*this);
		}

		// Перегрузка постфиксного декремента (с фиктивным параметром)
		ReverseIterator operator--(int) {
			ReverseIterator tmp_it(this->pointer_node, this->pointer_last);
			if (this->pointer_node == nullptr) {
				this->pointer_node = this->pointer_last;
			}
			else {
				this->pointer_node = this->pointer_node->next;
			}
			return tmp_it;
		}

		// Перегрузка префиксного декремента
		ReverseIterator& operator--() {
			if (this->pointer_node == nullptr) {
				this->pointer_node = this->pointer_last;
			}
			else {
				this->pointer_node = this->pointer_node->next;
			}
			return (*this);
		}

		// Перегрузка оператора разыменования
		T& operator*() const {
			if (this->pointer_node != nullptr) {
				return *(this->pointer_node->value);
			}
			else {
				throw std::runtime_error("Exception_runtime: empty node has no value");
			}
		}

		// Перегрузка оператора равенства
		bool operator==(const ReverseIterator& b) const {
			return this->pointer_node == b.pointer_node;
		}

		// Перегрузка оператора неравенства
		bool operator!=(const ReverseIterator& b) const {
			return this->pointer_node != b.pointer_node;
		}

		// Перегрузка оператора меньше
		bool operator<(const ReverseIterator& b) const {
			return ((b - (*this)) > 0);
		}

		// Оператор сложения дифф типа с итератором
		ReverseIterator operator+(difference_type x) const {
			ReverseIterator tmp_it = *this;
			for (size_t i = 0; i < x; ++i) {
				++(tmp_it);
			}
			return tmp_it;
		}

		// Оператор вычитания дифф типа из итератора
		ReverseIterator operator-(difference_type x) const {
			ReverseIterator tmp_it = *this;
			for (size_t i = 0; i < x; ++i) {
				--(tmp_it);
			}
			return tmp_it;
		}

		// Оператор вычитания итераторов
		difference_type operator-(const ReverseIterator& b) const {
			difference_type i = 0;
			ReverseIterator tmp_it = *this;
			while (tmp_it.pointer_node != b.pointer_node) {
				(tmp_it)--;
				i++;
			}
			return i;
		}

		// Метод получения указателя на следующий элемент
		Node* getNext() const {
			return this->pointer_node->next;
		}
	};

	// Конструктор по-умолчанию
	MyList() {}

	// Конструктор списка инициализации
	MyList(initializer_list<T> init) {
		for (auto el : init)
			this->push(el);
	}

	// Деструктор
	~MyList() {
		int my_size = this->size();
		if (my_size > 0) {
			for (int i = 0; i < my_size; i++) {
				this->pop();
			}
		}
	}

    // Добавление элемента в конец
	void push(T value) {
		Node* node = new Node(value);

		if (this->first == nullptr) {
			this->first = node;
		}
		if (this->last == nullptr) {
			this->last = node;
		}
		else {
			node->prev = this->last;
			this->last->next = node;
			this->last = node;
		}


		this->elems++;
	}

	// Добавление после произвольного элемента
	void insert(T value, int position) {
        if((this->size() <= position) || (position < 0)){
            throw std::runtime_error("Exception_runtime: no such element in list");
        }

		Node* node = new Node(value);

        if(this->size() == 0){
            if (this->first == nullptr) {
			    this->first = node;
            }
            if (this->last == nullptr) {
                this->last = node;
            }
        }else if(position < this->size() - 1){
            Node* tmp_node = this->first;
            for(int i = 0; i < position; i++){
                tmp_node = tmp_node->next;
            }

			node->prev = tmp_node;
            node->next = tmp_node->next;
			tmp_node->next->prev = node;
			tmp_node->next = node;
		}else{
			node->prev = this->last;
            node->next = nullptr;
            this->last->next = node;
			this->last = node;
        }


		this->elems++;
	}

	// Взятие элемента из начала
	T pop() {
		if (this->first != nullptr) {

			this->elems--;

			T val = *(this->first->value);

			if (this->first == this->last) {

				delete this->first;

				this->last = nullptr;
				this->first = nullptr;

			}
			else {

				Node* tmp_node = this->first->next;


				delete this->first;
				this->first = tmp_node;
				this->first->prev = nullptr;
			}

			return val;

		}
		else {
			throw std::logic_error("Exception_logic: empty node has no value");
		}
	}

    // Взятие произвольного элемента
	T erase(int position) {
		if (position < this->size() && position >= 0) {

            
            Node* tmp_node_main = this->first;
            for(int i = 0; i < position; i++){
                tmp_node_main = tmp_node_main->next;
            }

            T val = *(tmp_node_main->value);

			if (this->first == this->last) {

				delete this->first;

				this->last = nullptr;
				this->first = nullptr;

			}
			else {
                if(position == 0){
                    Node* tmp_node = this->first->next;

                    delete this->first;
                    this->first = tmp_node;
                    this->first->prev = nullptr;
                }else if(position == this->size() - 1){
                    Node* tmp_node = this->last->prev;

                    delete this->last;
                    this->last = tmp_node;
                    this->last->next = nullptr;
                }else{
                    tmp_node_main->prev->next = tmp_node_main->next;
                    tmp_node_main->next->prev = tmp_node_main->prev;
                    tmp_node_main->prev = nullptr;
                    tmp_node_main->next = nullptr;
                    delete tmp_node_main;
                }
				
			}

            this->elems--;
			return val;

		}
		else {
			throw std::logic_error("Exception_logic: empty node has no value");
		}
	}

	// Метод возврата итератора на начало
	Iterator begin() {
		return Iterator(this->first, this->last);
	}

	// Метод возврата итератора на конец
	Iterator end() {
		return Iterator(nullptr, this->last);
	}

	// Метод возврата обратного итератора на конец (начало)
	ReverseIterator rbegin() {
		return ReverseIterator(this->last, this->first);
	}

	// Метод возврата обратного итератора на начало (конец)
	ReverseIterator rend() {
		return ReverseIterator(nullptr, this->first);
	}

	// Метод подсчета кол-ва элементов
	int size() {
		return this->elems;
	}

	// Метод проверки на пустоту
	bool empty() {
		return this->first ? false : true;
	}

};

// Тест 1: заполнение контейнера 1000 целыми числами в диапазоне от -1000 до 1000 и подсчет их суммы, среднего, минимального и максимального
void test_1() {
	const int MIN = -1000;
	const int MAX = 1000;

	const int AMOUNT = 1000;

	int min_int = MAX;
	int max_int = MIN;
	int sum = 0;

	MyList<int> my_list;

	// Заполняем 1000 элементами список:
	for (int i = 0; i < AMOUNT; i++) {
		my_list.push(MIN + rand() % (MAX - MIN + 1));
	}

	// В for auto считаем макс и мин:
	for (auto el : my_list) {

		if (el < min_int) {
			min_int = el;
		}

		if (el > max_int) {
			max_int = el;
		}
	}

	int m_size = my_list.size();

	// В for считаем сумму:
	for (int i = 0; i < m_size; i++) {
		sum += my_list.pop();
	}

	// Выводим статистику:
	cout << endl << "Тест 1:" << endl;
	cout << "Сумма: " << sum << endl;
	cout << "Max: " << max_int << endl;
	cout << "Min: " << min_int << endl;

}

// Тест 2: Провести проверку работы операций вставки и изъятия элементов на коллекции из 10 строковых элементов
void test_2() {
	MyList<string> my_list;

	cout << endl << "Тест 2:" << endl;
	cout << "Изначальный размер списка: " << my_list.size() << endl;

	// Заполняем 10 элементами (строками):
	my_list.push("One");
	my_list.push("Two");
	my_list.push("Three");
	my_list.push("Four");
	my_list.push("Five");
	my_list.push("Six");
	my_list.push("Seven");
	my_list.push("Eight");
	my_list.push("Nine");
	my_list.push("Ten");

	// Выводим список и его размер:
	cout << "Список: ";

	for (auto el : my_list) {
		cout << el << " ";
	}

	cout << endl << "Новый размер списка: " << my_list.size() << endl;

	// Теперь всё удаляем:
	int q_size = my_list.size();

	for (int i = 0; i < q_size; i++) {
		my_list.pop();
	}

	cout << endl << "Новый размер списка после удаления: " << my_list.size() << endl;


}

/*
Тест 3:
Заполнение контейнера 100 структур содержащих фамилию, имя, отчество и дату рождения(от 01.01.1980 до 01.01.2020)
значения каждого поля генерируются случайно из набора заранее заданных.
После заполнение необходимо найти всех людей младше 20 лет и старше 30 и создать новые структуры содержащие результат фильтрации,
проверить выполнение на правильность подсчётом кол-ва элементов не подходящих под условие в новых структурах.
*/

// Структура даты:
struct Date {
	int day;
	int month;
	int year;
};

// Структура человека:
struct Person {
	string name;
	string surname;
	string patronim;
	Date birth;
};

// Функция 3-го теста:
void test_3() {
	const int DAY = 86400;
	const int AMOUNT = 100;

	// Данные для генерации:
	const string NAMES[5] = { "Serena", "Dan", "Blaire", "Chuck", "Neight" };
	const string SURNAMES[5] = { "Humphrey", "Waldorf", "Van-Der-Woodson", "Bass", "Archibald" };
	const string PATRONIMS[5] = { "Petrovich", "Ivanovna", "Vladimirovna", "Fedorovich", "Egorovna" };
	const int NAMES_POOL = 5; // Кол-во имен для выбора

	// Данные для генерации даты:
	const int DAY_MAX = 31;
	const int MONTH_MAX = 12;
	const int YEAR_MIN = 1980;
	const int YEAR_MAX = 2020;

	MyList<Person> li;

	// Генерируем людей:
	for (int i = 0; i < AMOUNT; i++) {

		li.push(Person{
			NAMES[rand() % NAMES_POOL],
			SURNAMES[rand() % NAMES_POOL],
			PATRONIMS[rand() % NAMES_POOL],
			Date{
				(1 + rand() % DAY_MAX),
				(1 + rand() % MONTH_MAX),
				(YEAR_MIN + rand() % (YEAR_MAX - YEAR_MIN + 1))
			}
			});

	}
	cout << endl << "Тест 3: " << endl;

	MyList<Person> younger_20;
	MyList<Person> older_30;

	// Получаем текущую дату:
	time_t t_now = time(0);

	// Фильтруем:
	for (int i = li.size(); i > 0; i--) {

		Person tmp_p = li.pop();

		tm dateBirth = { 0, 0, 0, tmp_p.birth.day, tmp_p.birth.month - 1, tmp_p.birth.year - 1900, 0, 0, 0 };

		time_t t_birth = mktime(&dateBirth);

		time_t daysDifference = ((t_birth > t_now) ? t_birth - t_now : t_now - t_birth) / DAY;
		daysDifference /= 365;

		// Если младше 20:
		if (daysDifference < 20) {
			younger_20.push(tmp_p);
			// Если старше 30:
		}
		else if (daysDifference > 30) {
			older_30.push(tmp_p);
			// Иначе:
		}
		else {
			li.push(tmp_p);
		}
	}

	cout << "Младше 20: " << younger_20.size() << endl;
	cout << "Старше 30: " << older_30.size() << endl;
	cout << "Остальные: " << li.size() << endl;
	cout << "Проверка: " << younger_20.size() << " + " << older_30.size() << " + " << li.size() << " = " << AMOUNT << "? - ";
	(((younger_20.size() + older_30.size() + li.size()) == AMOUNT) ? (cout << "Да") : (cout << "Нет"));

}

// Тест 4: Заполнить структуру 1000 элементов и отсортировать ее, проверить правильность использую структуру из стандартной библиотеки и сравнив результат
void test_4() {

	// Использовать будем двусвязный список типа deque
	MyList<int> my_list;
	deque<int> li;

	const int AMOUNT = 1000;

	const int MIN = -1000;
	const int MAX = 1000;

	// Заполняем 1000 элементами список:
	for (int i = 0; i < AMOUNT; i++) {

		li.push_back(MIN + rand() % (MAX - MIN + 1));

		my_list.push(li.back());

	}

	cout << endl << "li: ";
	for (auto el : li)
		cout << el << " ";
	cout << endl << "m: ";
	for (auto el : my_list)
		cout << el << " ";

	// Сортируем с помощью стандартной библиотеки:
	sort(my_list.begin(), my_list.end());
	//sort(my_list.rbegin(), my_list.rend()); <- можно и так
	sort(li.begin(), li.end());
	
	cout << endl << endl << "li: ";
	for (auto el : li)
		cout << el << " ";
	cout << endl << "m: ";
	for (auto el : my_list)
		cout << el << " ";

	// Проверяем поэлементно:
	bool is_correct = true;

	for (int i = 0; i < my_list.size(); i++) {
		if (my_list.pop() != li.front()) {
			is_correct = false;
			break;
		}
		li.pop_front();
	}

	// Выводим результат:
	cout << endl << endl << "Тест 4: " << endl;

	if (is_correct) {
		cout << "Всё верно, списки сортируются одинаково" << endl;
	}
	else {
		cout << "Что-то не так, списки отсортированы по-разному" << endl;
	}
}

// Тест 5: (Список) Перемешать все элементы отсортированного списка в случайном порядке.
void test_5() {
	MyList<int> ml({ 0, 1, 2, 3, 4, 5 });

    int tmp_val;
	for(int i = 0; i < ml.size(); i++){
        tmp_val = ml.erase(i);
        ml.insert(tmp_val, rand() % (ml.size()));
    }

	// Выводим результат:
	cout << endl << "Тест 5:" << endl;

	for (auto el : ml) {
		cout << el << " ";
	}

}

int main() {
	srand(time(0));
	setlocale(LC_ALL, "Russian");

	test_1();
	test_2();
	test_3();
	test_4();
	test_5();

	cout << endl;
}