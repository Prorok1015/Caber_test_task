#pragma once
/**
	������� ����� 2022 ���
	������ ���������� � ������ ��������� ������� ��� Caber Interactive.
*/

namespace t3 {
	// ��������� ListNode �������������� ������
	struct ListNode {
		ListNode* prev = nullptr;
		ListNode* next = nullptr;
		// ��������� �� ������������ ������� ������� ������, ���� NULL
		ListNode* rand = nullptr; 
		std::string data;
	};

	class List {
	public:
		void serialize(FILE* file); // ���������� � ���� (���� ������ � ������� fopen(path, "wb"))

		void deserialize(FILE* file); // �������� �� ����� (���� ������ � ������� fopen(path, "rb"))

		void push_back(std::string_view data);
		ListNode* get_head() { return head; };
		ListNode* get(int i);

		~List();
		bool operator == (List& l2);
		void operator = (List& l2);
	private:
		ListNode* rand_node_or_null();
		ListNode* head = nullptr;
		ListNode* tail = nullptr;
		int count = 0;
	};

}