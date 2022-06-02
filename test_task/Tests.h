#pragma once

#include "1_task/first.h"
#include "2_task/second.h"

#include <iostream>
#include <string>

namespace tests {

	bool test_1_task() {
		std::cout << std::endl << "TASK 1 BEGIN" << std::endl;
		std::cout << "test 1 name: const data" << std::endl;
		// prepaire + call
		t1::print_as_binary(3);
		t1::print_as_binary(-3);
		t1::print_as_binary(1563254657);
		t1::print_as_binary(-1563254657);

		auto* data1 = t1::as_binary(3);
		auto* data2 = t1::as_binary(-3);

		// campare
		if (std::strcmp(data1, "00000000000000000000000000000011") != 0) return false;
		if (std::strcmp(data2, "11111111111111111111111111111101") != 0) return false;

		delete[] data1;
		delete[] data2;

#ifndef NOUINPUT1
		std::cout << "test 2 name: user`s number" << std::endl;
		std::cout << "input int number: ";
		int number = 0;
		std::cin >> number;
		t1::print_as_binary(number);
#endif // !NOUINPUT

		std::cout << "TASK 1 END" << std::endl;

		return true;
	}

	bool test_2_task() {
		std::cout << std::endl << "TASK 2 BEGIN" << std::endl;
		std::cout << "test 1 name: const data" << std::endl;
		// prepair data
		char str[] = "AAA BBB   CCC";
		std::cout << "old data: " 
					<< str << std::endl;
		// call
		t2::remove_dups(str);

		std::cout << "new data: " 
					<< str << std::endl;
		// campare
		if (std::strcmp(str, "A B C") != 0) return false;

#ifndef NOUINPUT2
		std::cout << "test 2 name: user`s string" << std::endl;
		std::cout << "input string: " ;
		std::string s;
		std::getline(std::cin, s);
		std::cout << "old data: " 
					<< s << std::endl;

		char* data = new char[s.length()+1];
		s.copy(data, s.length());
		data[s.length()] = '\0';

		t2::remove_dups(data);
		std::cout << "new data: " 
					<< data << std::endl;

		delete[] data;
#endif // !NOUINPUT

		std::cout << "TASK 2 END" << std::endl;
		return true;
	}

	bool test_3_task() {

		return true;
	}

	bool stress_test_2(int n) {

		for (int i = 0; i < n; ++i)
		{
			char* data = new char[31];
			data[30] = '\0';
			for (int j = 0; j < 30; ++j)
				data[j] = 'A';

			t2::remove_dups(data);
			delete[] data;
		}
		return true;
	}

	void run_all_tests() {
		if (test_1_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
		if (test_2_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
		if (test_3_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
	}
}