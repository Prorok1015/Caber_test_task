#include <string_view>
#include "third.h"

#include <unordered_map>
#include <fstream>

using NodePtr = t3::ListNode*;
template <typename T, typename U> using map = std::unordered_map<T, U>;

// ����: ������� ����������� ����������� ������ �� �����������,
// ����� ������� ������ ��� ����� ��� ���� ������� ����� ����
// �� ������� ����(� �������� ����� ��������� id �������� ����������� � �����)
// ���� ����� � ������ ��� ���, �� ����� ������ ��� ������,
// ������ ������� ����� � ���������� ����� ����.
// � ������ ��� "�������" ���������, 
// �.�. ����� ����� ������� ���� rand � ����� ������� ��� ���, 
// �� �� ������� ������ ���� � ������ ��������� �� ���
// ����������, ��� ����� �� ��� ������ ��� ������ �� �����, �� �������� ��� ������ ������
// (� ��� ����� ���� ��� ���� ����� ��������� �� ��������� ����)
t3::ListNode* new_or_lazy(t3::ListNode* key, map<NodePtr, NodePtr>& lazyMap)
{
	if (lazyMap.find(key) == lazyMap.end())
		lazyMap[key] = new t3::ListNode;
	return lazyMap[key];
}

// ������ ������� ������������,
// ����� ��������� �������� � ���������� ��������.
// �� ���� �������� id ����� � ��������� ��������� ���������� � �����
t3::ListNode* lazy_call(t3::ListNode* rand, map<NodePtr, NodePtr>& lazyMap)
{
	if (!rand) return nullptr;
	return new_or_lazy(rand, lazyMap);
}


void t3::List::serialize(FILE* file)
{
	if (!file) return;

	std::ofstream os(file);
	// ��������� ����� ���-�� ��������� 
	os.write(reinterpret_cast<const char*>(&count), sizeof(count));
	for(auto tmp = get_head(); tmp; tmp = tmp->next)
	{
		size_t size = tmp->data.size();
		// ��������� ��������� �� ����, �� ��������� � �������� id ������
		os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp)).
		// ��������� ��������� �� ����� �� ��������� �����
		   write(reinterpret_cast<const char*>(&tmp->rand), sizeof(tmp->rand)).
		// ��������� ������(� ������ ������ ���-��) �������� � ������
		   write(reinterpret_cast<const char*>(&size), sizeof(size)).
		// ��������� ���� �������
		   write(tmp->data.c_str(), size);
	}
	/*
	 ����� �������, ���� ������ �������� ���:
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
	// ���������� ��������� ��� ����������
	int read_count = 0;
	// ��������� �� ����� ���������� ��������� ������� ��������� �������
	is.read(reinterpret_cast<char*>(&read_count), sizeof(read_count));

	//����� ��� ����������� ��������� �� ����� � ��������� � �����
	map<NodePtr, NodePtr> lazyMap;
	while (read_count != count)
	{
		ListNode* cmp = nullptr;
		ListNode* rand = nullptr;
		size_t size = 0;
		std::string data;

		// ��������� ���� ������, ���� �� ���������,�� �������
		if (!is
			.read(reinterpret_cast<char*>(&cmp), sizeof(cmp))
			.read(reinterpret_cast<char*>(&rand), sizeof(rand))
			.read(reinterpret_cast<char*>(&size), sizeof(size))
			.read((data.resize(size), data.data()), size)
			) break;

		// �������� ������������ ���������
		ListNode* temp = new_or_lazy(cmp, lazyMap);
		temp->next = nullptr;
		temp->data = std::move(data);

		if (head)
		{
			temp->prev = tail;
			tail->next = temp;
			// ����������� "�������" ���������
			temp->rand = lazy_call(rand, lazyMap);
			tail = temp;
		}
		else //���� ������ ������
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

/*-----------------------------��������������� �������--------------------------------------*/
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
		else //���� ������ ������
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
	else //���� ������ ������
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
