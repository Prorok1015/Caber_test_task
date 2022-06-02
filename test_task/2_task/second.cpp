#include "second.h"
#include <iostream>

void t2::remove_dups(char* str)
{
	if (str == nullptr) return;

	size_t len = std::strlen(str);
	size_t cur = 0;
	char now_uniq = str[0];
	for (int i = 1; i < len; ++i)
	{
		if (str[i] != now_uniq) {
			now_uniq = str[i];
			str[++cur] = now_uniq;
		}
	}

	str[cur + 1] = '\0';
}
