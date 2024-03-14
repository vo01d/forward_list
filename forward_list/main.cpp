#include <iostream>
#include <vector>

#include "task.h"

int main() {
	// some test code
	task::forward_list<std::string> firstCtor;
	std::cout << "firstCtor empty() = " << std::boolalpha << firstCtor.empty() << std::endl;

	task::forward_list<std::string> secondCtor(5, "C++");
	std::cout << "Second ctor: ";
	for (const auto& value : secondCtor) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	std::vector<std::string> vec{ "5", "4", "3", "2", "1" };
	task::forward_list<std::string> fourCtor(vec.begin(), vec.end());
	std::cout << "Four ctor: ";
	for (const auto& value : fourCtor) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	task::forward_list<std::string> copyCtor(fourCtor);
	std::cout << "Copy ctor: ";
	for (const auto& value : copyCtor) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	task::forward_list<std::string> moveCtor(std::move(copyCtor));
	std::cout << "Move ctor: ";
	for (const auto& value : moveCtor) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	task::forward_list<double> initializer_listCtor{ 2.0, 3.0, 4.0 };
	std::cout << "initializer_listCtor ctor: ";
	for (const auto& value : initializer_listCtor) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	task::forward_list<int> pushPopList;
	pushPopList.push_front(4);
	std::cout << "pushPopList front() = " << pushPopList.front() << std::endl;
	pushPopList.pop_front();
	std::cout << "pushPopList empty() = " << std::boolalpha << firstCtor.empty() << std::endl;

	task::forward_list<int> swapList1{ 1,2,3,4,5 };
	task::forward_list<int> swapList2{ 5,4,3,2,1 };
	std::cout << "Before swap:" << std::endl;
	for (auto it = swapList1.cbegin(); it != swapList1.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	for (auto it = swapList2.cbegin(); it != swapList2.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	swapList1.swap(swapList2);

	std::cout << "After swap:" << std::endl;
	for (auto it = swapList1.cbegin(); it != swapList1.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	for (auto it = swapList2.cbegin(); it != swapList2.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	return 0;
}