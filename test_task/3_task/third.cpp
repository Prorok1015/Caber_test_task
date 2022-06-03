#include <string_view>
#include "third.h"

#include <unordered_map>
#include <fstream>

using NodePtr = t3::ListNode*;
template <typename T, typename U> using map = std::unordered_map<T, U>;

// иде€: создать отображение считываемых данных на сохран€емые,
// таким образом каждый раз когда нам надо создать новую ноду
// мы смотрим ключ(в качестве ключа выступает id элемента записанного в файле)
// если св€зи с ключом еще нет, то таких данных еще небыло,
// значит создаем св€зь и возвращаем новую ноду.
// € назвал это "ленивый" указатель, 
// т.к. когда нужно св€зать поле rand с нодой которой еще нет, 
// то мы создаем пустую ноду и отдаем указатель на нее
// расчитыва€, что когда до нее дойдут при чтении из файла, то заполним все нужные данные
// (в том числе если она тоже будет ссылатьс€ на случайную ноду)
t3::ListNode* new_or_lazy(t3::ListNode* key, map<NodePtr, NodePtr>& lazyMap)
{
	if (lazyMap.find(key) == lazyMap.end())
		lazyMap[key] = new t3::ListNode;
	return lazyMap[key];
}

// данна€ функци€ используетс€,
// чтобы присвоить прив€зку к случайному элементу.
// на вход получает id св€зи с случайным элементом записанным в файле
t3::ListNode* lazy_call(t3::ListNode* rand, map<NodePtr, NodePtr>& lazyMap)
{
	if (!rand) return nullptr;
	return new_or_lazy(rand, lazyMap);
}


void t3::List::serialize(FILE* file)
{
	if (!file) return;

	std::ofstream os(file);
	// сохран€ем общее кол-во элементов 
	os.write(reinterpret_cast<const char*>(&count), sizeof(count));
	for(auto tmp = get_head(); tmp; tmp = tmp->next)
	{
		size_t size = tmp->data.size();
		// сохран€ем указатель на ноду, он выступает в качестве id записи
		os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp)).
		// сохран€ем указатель на св€сь со случайной нодой
		   write(reinterpret_cast<const char*>(&tmp->rand), sizeof(tmp->rand)).
		// сохран€ем размер(в данном случае кол-во) символов в строке
		   write(reinterpret_cast<const char*>(&size), sizeof(size)).
		// сохран€ем сами символы
		   write(tmp->data.c_str(), size);
	}
	/*
	 таким образом, блок данных выгл€дит так:
		int

		ListNode*
		ListNode*
		size_t
		string

		ListNode*
		ListNode*
		size_t
		string

		...
	*/
}

void t3::List::deserialize(FILE* file)
{
	if (!file) return;

	std::ifstream is(file);

	clear();
	// количество элементов дл€ считывани€
	int read_count = 0;
	// считываем из файла количество элементов которых предстоит считать
	is.read(reinterpret_cast<char*>(&read_count), sizeof(read_count));

	//карта дл€ отображени€ элементов из файла к элементам в листе
	map<NodePtr, NodePtr> lazyMap;
	while (read_count != count)
	{
		ListNode* cmp = nullptr;
		ListNode* rand = nullptr;
		size_t size = 0;
		std::string data;

		// считываем блок данных, если не считалось,то выходим
		if (!is
			.read(reinterpret_cast<char*>(&cmp), sizeof(cmp))
			.read(reinterpret_cast<char*>(&rand), sizeof(rand))
			.read(reinterpret_cast<char*>(&size), sizeof(size))
			.read((data.resize(size), data.data()), size)
			) break;

		// получаем отображенный указатель
		ListNode* temp = new_or_lazy(cmp, lazyMap);
		temp->next = nullptr;
		temp->data = std::move(data);

		if (head)
		{
			temp->prev = tail;
			tail->next = temp;
			// присваиваем "ленивый" указатель
			temp->rand = lazy_call(rand, lazyMap);
			tail = temp;
		}
		else //≈сли список пустой
		{
			temp->prev = nullptr;
			head = tail = temp;
		}
		++count;
	}
}

void t3::List::clear() {
	while (head)
	{
		tail = head->next;
		delete head;
		head = tail;
		--count;
	}
}

/*-----------------------------¬спомогательные функции--------------------------------------*/
#include <random>
void t3::List::operator = (const List& l2)
{
	if (this == &l2) return;

	clear();

	map<NodePtr, NodePtr> lazyMap;
	
	for(auto cmp = l2.head; cmp; cmp = cmp->next)
	{
		ListNode* temp = new_or_lazy(cmp, lazyMap);
		temp->next = nullptr;
		temp->data = cmp->data;

		if (head)
		{
			temp->prev = tail;
			tail->next = temp;
			temp->rand = lazy_call(cmp->rand, lazyMap);
			tail = temp;
		}
		else //≈сли список пустой
		{
			temp->prev = nullptr;
			head = tail = temp;
		}
		++count;

	}
}

bool t3::List::operator == (List& l2)
{
	if (l2.count != count) return false;

	auto tmp = head;
	auto cmp = l2.head;
	while (tmp && cmp)
	{
		bool result = tmp->data == cmp->data;

		auto r1 = tmp->rand;
		auto r2 = cmp->rand;

		if (r1 && r2)
		{
			result &= r1->data == r2->data;
		}

		if (!result) return false;

		cmp = cmp->next;
		tmp = tmp->next;
	}

	return true;
}

void t3::List::push_back(std::string_view data)
{
	ListNode* temp = new ListNode;
	temp->next = NULL;
	temp->data = data;

	if (head != NULL)
	{
		temp->prev = tail;
		tail->next = temp;
		temp->rand = rand_node_or_null();
		tail = temp;
	}
	else //≈сли список пустой
	{
		temp->prev = NULL;
		head = tail = temp;
	}
	++count;
}

t3::ListNode* t3::List::get(int i)
{
	int it = 0;
	ListNode* temp = head;
	while (temp && i != it)
	{
		temp = temp->next;
		++it;	
	}
	return temp;
}

t3::ListNode* t3::List::rand_node_or_null()
{
	static int i = 0;
	if (++i % 3 == 0)
		return get(std::min(std::rand() % count, count - 1));
	return nullptr;
}

t3::List::~List()                           
{
	while (head)                      
	{
		tail = head->next;            
		delete head;                  
		head = tail;                   
	}
}
