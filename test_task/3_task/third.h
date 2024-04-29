#pragma once
/**
	Лебедюк Захар 2022 год
	Модуль разработан в рамках тестового задания для Caber Interactive.
*/

#include <functional>

namespace t3 {
	// структуру ListNode модифицировать нельзя
	struct ListNode {
		ListNode* prev = nullptr;
		ListNode* next = nullptr;
		// указатель на произвольный элемент данного списка, либо NULL
		ListNode* rand = nullptr; 
		std::string data;
	};

	class List final {
	public:
		List() noexcept = default;
		~List() {
			clear();
		};
		List(const List & rhs) { *this = rhs; };
		List(List&& rhs) noexcept = default;
		List& operator= (const List & l2);
		List& operator= (List&& l2) noexcept = default;

		bool operator == (const List & l2) const noexcept;
		void push_back(std::string_view data);
		ListNode* get_head() { return head; };
		ListNode* get(int i);

		void serialize(FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
		void deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

		void clear();

	private:
		void insert_back(std::string_view data, std::function<ListNode* ()> allocate_node, std::function<ListNode* ()> allocate_random_node);
		ListNode* rand_node_or_null();

	private:
		ListNode* head = nullptr;
		ListNode* tail = nullptr;
		std::uint64_t count = 0;
	};

}