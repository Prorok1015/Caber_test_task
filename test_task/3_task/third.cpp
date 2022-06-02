#include <string_view>
#include "third.h"
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>


void t3::List::serialize(FILE* file)
{
}

void t3::List::deserialize(FILE* file)
{
}


/*-----------------------------Вспомогательные функции--------------------------------------*/
using NodePtr = t3::ListNode*;
template <typename T, typename U> using map = std::unordered_map<T, U>;

t3::ListNode* new_or_lazy(t3::ListNode* key, map<NodePtr, NodePtr>& lazyMap);

t3::ListNode* lazy_call(t3::ListNode* rand, map<NodePtr, NodePtr>& lazyMap)
{
	if (rand == nullptr) return nullptr;
	return new_or_lazy(rand, lazyMap);
}

t3::ListNode* new_or_lazy(t3::ListNode* key, map<NodePtr, NodePtr>& lazyMap)
{
	if (lazyMap.find(key) == lazyMap.end())
		lazyMap[key] = new t3::ListNode;
	return lazyMap[key];
}

void t3::List::operator = (List& l2)
{
	while (head)
	{
		tail = head->next;
		delete head;
		head = tail;
		--count;
	}
	map<NodePtr, NodePtr> lazyMap;
	auto cmp = l2.head;
	while (cmp)
	{
		ListNode* temp = new_or_lazy(cmp, lazyMap);
		temp->next = nullptr;
		temp->data = cmp->data;

		if (head != nullptr)
		{
			temp->prev = tail;
			tail->next = temp;
			temp->rand = lazy_call(cmp->rand, lazyMap);
			tail = temp;
		}
		else //Если список пустой
		{
			temp->prev = nullptr;
			head = tail = temp;
		}
		++count;

		cmp = cmp->next;
	}
}

bool t3::List::operator == (List& l2)
{
	if (l2.count == count) return false;

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
	else //Если список пустой
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
		if (temp != nullptr)
		{
			temp = temp->next;
			++it;
		}
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
