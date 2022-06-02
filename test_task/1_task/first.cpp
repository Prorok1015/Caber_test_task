#include "first.h"
#include <iostream>

// ��� ������ ��������� int �� �� ��������, �.�. 
// ���� ��������� �� ����������� ������ ����� �� �� ����� �� ��������
// (���� ���������, ������ ��� � ����������� ����������� ���� �������� ��� int,
// ������� �������� ������� ��� ������������� ���������).
//
// ���������� const char* ���� �� ���� �������������, 
// ������ ��� �� ������� �� ��� ������ � ���� ��������, 
// ������ ��� �� ���� ������ ������� ��� ��� ���.
char* t1::as_binary(int num)
{
	// ������ ������ ������ ����� ���������� � ������ � �����.
	constexpr int bits = sizeof(num) * 8;
	// �������� ������ ��� ������ � ���� ��������� ����.
	// �� 1 ������� ������ ��� ������������ ����.
	char* out = new char[bits + 1];
	out[bits] = '\0';
	// �������� �� ������� ����, bits ���.
	for (int ipos = 0, step = bits - 1; ipos < bits; ++ipos, --step) {
		// ������� ����������� ��� ����� ����� ��������� ���� �� ����� (��������� ����� 1),
		// ���� �� 0, �� � ������ ��������� '0' ���� 1, �� '1'
		out[ipos] = ((num >> step) & 1) + '0';
	}
	// ���������� ��������� �� ����������� ������, 
	// �� ��������� ������� ������������ ������ ���
	return out; 
}

void t1::print_as_binary(int num)
{
	auto* bin = as_binary(num);
	// ��� ��� �� ����� �� �++, �� ��������� �������� �����
	std::cout << bin << std::endl;
	delete[] bin;
}
