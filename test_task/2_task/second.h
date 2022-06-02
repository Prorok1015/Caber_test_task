#pragma once
/**
	������� ����� 2022 ���
	������ ���������� � ������ ��������� ������� ��� Caber Interactive.
*/
namespace t2 {
	/// <summary>
	///  ������� �������� ����� ������ ���������� �� ����� ����������
	/// </summary>
	/// <param name="start">
	///  - ������ �������������� ������������ �����. 
	/// </param>
	/// <returns> ����� ������ �� ������� ��������� </returns>
	constexpr size_t my_strlen(const char* start)
	{
		const char* end = start;
		for (; *end != '\0'; ++end)
			;
		return end - start;
	}

	/// <summary>
	///  ������� ������������� ��� �������� ������ ������ ����������� ��������.
	///  �� ���������� ��������� ������, �� ����������� ������ ����������� �� ����� ����������.
	/// </summary>
	/// <param name="str">
	///	 - ������ �������������� ������������ �����.
	/// </param>
	constexpr void remove_dups(char* str)
	{
		//  ������ ���������� ����������� � .cpp ����, 
		//  ��� ��� ��� ������ �� ����� ���������� ���������� ����� �� ����������� ����� � ������
		//  ���� ���������, �� ��������� ������ �� ����������

		if (!str) return;

		size_t len = my_strlen(str);
		size_t cur = 0;
		char now_uniq = str[0];
		for (int i = 1; i < len; ++i)
		{
			// ���� ��������� ����� ������
			if (str[i] != now_uniq) {
				// ��������� ������������� ������
				now_uniq = str[i];
				// ���������� ����� ������ � ���� 
				// �� ��������� ���������� �������� ������ 
				str[++cur] = now_uniq;
			}
		}

		str[cur + 1] = '\0';
	}

}