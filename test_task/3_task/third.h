#pragma once
/**
	Лебедюк Захар 2022 год
	Модуль разработан в рамках тестового задания для Caber Interactive.
*/
namespace t3 {
	// структуру ListNode модифицировать нельзя
	struct ListNode {
		ListNode* prev = nullptr;
		ListNode* next = nullptr;
		// указатель на произвольный элемент данного списка, либо NULL
		ListNode* rand = nullptr; 
		std::string data;
	};

	class List {
	public:
		void serialize(FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
		void deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))
		/// <summary>
		/// Очищает лист
		/// </summary>
		void clear();
	private:
		ListNode* head = nullptr;
		ListNode* tail = nullptr;
		int count = 0;

	public:
		/*-----вспомогательные функции-----*/
		void push_back(std::string_view data);
		ListNode* get_head() { return head; };
		ListNode* get(int i);
		List() = default;
		List(const List & l2) { *this = l2; };
		~List();
		bool operator == (List & l2);
		void operator = (const List & l2);
		ListNode* rand_node_or_null();
		//**********************************
	};

}