#pragma once

#include "1_task/first.h"
#include "2_task/second.h"
#include "3_task/third.h"

#include <stdio.h>
#include <iostream>
#include <string>

namespace tests {

	bool test_1_task() {
		std::cout << std::endl << "TASK 1 BEGIN" << std::endl;
		std::cout << "test 1 name: const data" << std::endl;
		// prepaire + call
		std::cout << "number " << 3 << " as binary: ";
		t1::print_as_binary(3);
		std::cout << "number " << -3 << " as binary: ";
		t1::print_as_binary(-3);
		std::cout << "number " << 1563254657 << " as binary: ";
		t1::print_as_binary(1563254657);
		std::cout << "number " << -1563254657 << " as binary: ";
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

		auto f_out = [](t3::List& lst) {
			auto tmp = lst.get_head();
			while (tmp)
			{
				std::cout << tmp->data << ", ";
				if (tmp->rand)
					std::cout << "rand: " << tmp->rand->data << ", ";
				tmp = tmp->next;
			}
			std::cout << " |" << std::endl;
		};

		std::cout << std::endl << "TASK 3 BEGIN" << std::endl;
		std::cout << "test 1 name: const data" << std::endl;
		t3::List lst, lst_double;
		lst.push_back("str1");
		lst.push_back("str2");
		lst.push_back("str3");
		lst.push_back("str4");
		lst.push_back("str5");
		lst.push_back("str6");
		lst.push_back("str7");
		lst.push_back("str8");

		lst_double.push_back("demo1");
		lst_double.push_back("demo2");
		lst_double.push_back("demo3");
		lst_double.push_back("demo4");
		lst_double.push_back("demo5");
		lst_double.push_back("demo6");
		lst_double.push_back("demo7");
		lst_double.push_back("demo8");
		lst.deserialize(nullptr);
		auto path = "task3.txt";
		FILE* file = nullptr;
		if (!fopen_s(&file, path, "wb"))
		{
			lst.serialize(file);
			lst_double.serialize(file);
			std::fclose(file);
			file = nullptr;
		}

		t3::List from_file;
		t3::List from_file2;
		if (!fopen_s(&file, path, "rb"))
		{
			from_file.deserialize(file);
			from_file2.deserialize(file);
			std::fclose(file);
		}

		f_out(lst);
		f_out(from_file);
		f_out(lst_double);
		f_out(from_file2);
		if (lst != from_file) return false;
		if (lst_double != from_file2) return false;
		std::cout << "TASK 3 END" << std::endl;
		return true;
	}

	void run_all_tests() {
		if (test_1_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
		if (test_2_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
		if (test_3_task()) std::cout << "success!" << std::endl; else std::cout << "fail!" << std::endl;
	}
}