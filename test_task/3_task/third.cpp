#include <string_view>
#include "third.h"

#include <unordered_map>
#include <fstream>
#include <random>

using NodePtr = t3::ListNode*;
template <typename T, typename U> using umap = std::unordered_map<T, U>;
using ListNodeCache = umap<std::uint32_t, NodePtr>;
using namespace std::placeholders;

// идея: создать отображение считываемых данных на сохраняемые,
// таким образом каждый раз когда нам надо создать новую ноду
// мы смотрим ключ(в качестве ключа выступает id элемента записанного в файле)
// если связи с ключом еще нет, то таких данных еще небыло,
// значит создаем связь и возвращаем новую ноду.
// я назвал это "ленивый" указатель, 
// т.к. когда нужно связать поле rand с нодой которой еще нет, 
// то мы создаем пустую ноду и отдаем указатель на нее
// расчитывая, что когда до нее дойдут при чтении из файла, то заполним все нужные данные
// (в том числе если она тоже будет ссылаться на случайную ноду)
t3::ListNode* require_node(std::uint32_t key, ListNodeCache& lazyMap)
{
	if (!key) {
		return nullptr;
	}

	if (lazyMap.find(key) == lazyMap.end())
		lazyMap[key] = new t3::ListNode;
	return lazyMap[key];
}

void t3::List::serialize(FILE* file)
{
	if (!file) {
		return;
	}

	std::ofstream os(file);
	// сохраняем общее кол-во элементов 
	os.write(reinterpret_cast<const char*>(&count), sizeof(count));
	for(auto tmp = get_head(); tmp; tmp = tmp->next)
	{
		std::uint64_t size = tmp->data.size();
		std::uint32_t node_key = reinterpret_cast<std::uint32_t>(tmp);
		std::uint32_t rand_key = reinterpret_cast<std::uint32_t>(tmp->rand);
		// сохраняем указатель на ноду, он выступает в качестве id записи
		os.write(reinterpret_cast<const char*>(&node_key), sizeof(node_key)).
		// сохраняем указатель на свясь со случайной нодой
		   write(reinterpret_cast<const char*>(&rand_key), sizeof(rand_key)).
		// сохраняем размер(в данном случае кол-во) символов в строке
		   write(reinterpret_cast<const char*>(&size), sizeof(size)).
		// сохраняем сами символы
		   write(tmp->data.c_str(), size);
	}
	/*
	 таким образом, блок данных выглядит так:
		std::uint64_t

		std::uint32_t
		std::uint32_t
		std::uint64_t
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
	if (!file) {
		return;
	}

	std::ifstream is(file);

	clear();

	std::uint64_t read_count = 0;
	is.read(reinterpret_cast<char*>(&read_count), sizeof(read_count));

	ListNodeCache cache;
	auto allocate = std::bind(require_node, _1, std::ref(cache));

	while (read_count != count)
	{
		std::uint32_t key = 0;
		std::uint32_t key_rand = 0;
		std::uint64_t size = 0;
		std::string data;

		if (!is
			.read(reinterpret_cast<char*>(&key),	  sizeof(key))
			.read(reinterpret_cast<char*>(&key_rand), sizeof(key_rand))
			.read(reinterpret_cast<char*>(&size),	  sizeof(size))
			.read((data.resize(size), data.data()),   size)
			) 
		{
			break;
		}

		insert_back(data,
			std::bind(allocate, key),
			std::bind(allocate, key_rand)
		);
	}
}

void t3::List::clear() {
	while (head) {
		tail = head->next;
		delete head;
		head = tail;
		--count;
	}
}

/*-----------------------------Вспомогательные функции--------------------------------------*/

t3::List& t3::List::operator= (const List& rhs)
{
	if (this == &rhs) {
		return *this;
	}

	clear();

	ListNodeCache cache;
	auto allocate = std::bind(require_node, _1, std::ref(cache));
	for(ListNode* right_node = rhs.head; right_node; right_node = right_node->next)
	{
		insert_back(right_node->data,
			std::bind(allocate, reinterpret_cast<std::uint32_t>(right_node)),
			std::bind(allocate, reinterpret_cast<std::uint32_t>(right_node->rand))
		);
	}

	return *this;
}

bool t3::List::operator == (const List& rhs) const noexcept
{
	if (rhs.count != count) return false;

	ListNode* node = head;
	ListNode* r_node = rhs.head;
	while (node && r_node)
	{
		if (node->data != r_node->data) {
			return false;
		}

		if (node->rand && r_node->rand && 
			node->rand->data != r_node->rand->data) {
			return false;
		}

		r_node = r_node->next;
		node = node->next;
	}

	return true;
}

void t3::List::push_back(std::string_view data)
{
	insert_back(data, [] { return new ListNode; }, std::bind(&List::rand_node_or_null, this));
}

void t3::List::insert_back(std::string_view data, std::function<ListNode* ()> allocate_node, std::function<ListNode* ()> allocate_random_node)
{
	if (ListNode* temp = allocate_node()) {
		temp->rand = allocate_random_node();
		temp->data = data;

		if (head) {
			temp->prev = tail;
			tail->next = temp;
			tail = temp;
		}
		else //Если список пустой
		{
			head = tail = temp;
		}
		++count;
	}
}

t3::ListNode* t3::List::get(int idx)
{
	if (idx < 0 || idx > count) {
		return nullptr;
	}

	int it = 0;
	ListNode* cur = head;
	while (cur && idx != it) {
		cur = cur->next;
		++it;	
	}

	return cur;
}

t3::ListNode* t3::List::rand_node_or_null()
{
	static int i = 0;
	if (++i % 3 == 0 && count > 0)
		return get(std::min(std::rand() % count, count - 1));
	return nullptr;
}
