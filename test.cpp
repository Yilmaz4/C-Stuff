#include <iostream>
#include <algorithm>
#include <iomanip>
#include <windows.h>

#include "src\linkedlist.h"
#include "src\stack.h"
#include "src\queue.h"
#include "src\circulararray.h"

struct Result {
	explicit Result(bool* base, int length) : base(base), length(length) {}
	bool* base;
	const int length;
};

int testSinglyLinkedList() {
	constexpr int length = 10 * 4;
	SinglyLinkedList<int> mySLL {0, 1, 2, 3, 4};
	bool checks[length];
	for (int i = 0; i < 5; i++) {
		checks[i + 0] = mySLL.has(i);
		mySLL.remove(i);
		checks[i + 5] = !mySLL.has(i);
	}
	for (int i = 0; i < 10; i++) {
		mySLL.push_back(i);
		checks[i + 10] = mySLL[i] == i;
	}
	for (int i = 0; i < 10; i++) {
		mySLL[i] = i * 2;
		checks[i + 20] = i == mySLL[i] / 2;
	}
	mySLL.reverse();
	for (int i = 9; i >= 0; i--) {
		checks[i + 30] = mySLL[9 - i] == i * 2;
	}
	index_t i = 0;
	SinglyLinkedList<unsigned long long> errors;
	for (bool* addr = checks; i < length; addr++, i++)
		if (!addr)
			errors.push_back(i);
	if (errors.empty())
		std::cout << "SLL (Singly Linked List) tests have passed!" << std::endl;
	else {
		std::cout << "SLL (Singly Linked List) tests have failed!"
			<< "Errors found in tests:" << std::endl;
		for (index_t i = 0; i < errors.size(); i++) {
			std::cout << std::string(4, ' ') << errors[i] << std::endl;
		}
		return 0;
	}
	return 1;
}

int testCircularArray() {
	constexpr int length = 10;
	CircularArray<int> myCA {0, 1, 2, 3, 4};
	bool checks[length];
	for (int i = 0; i < 5; i++) {
		checks[i + 0] = myCA[i] == i;
		myCA[i] = i * 2;
		checks[i + 5] = myCA[i] == i * 2;
	}
	index_t i = 0;
	SinglyLinkedList<unsigned long long> errors;
	for (bool* addr = checks; i < length; addr++, i++)
		if (!addr)
			errors.push_back(i);
	if (errors.empty())
		std::cout << "CA (Circular Array) tests have passed!" << std::endl;
	else {
		std::cout << "CA (Circular Array) tests have failed!"
			<< "Errors found in tests:" << std::endl;
		for (index_t i = 0; i < errors.size(); i++) {
			std::cout << std::string(4, ' ') << errors[i] << std::endl;
		}
		return 0;
	}
	return 1;
}

int testLinkedQueue() {
	constexpr int length = 10;
	LinkedQueue<int> myLQ { 0, 1, 2, 3, 4 };
	bool checks[length];
	for (int i = 0; i < 5; i++) {
		checks[i + 0] = myLQ.dequeue() == i;
		myLQ.enqueue(4 - i);
		checks[i + 5] = myLQ.front() == 4 - i;
	}
	index_t i = 0;
	SinglyLinkedList<unsigned long long> errors;
	for (bool* addr = checks; i < length; addr++, i++)
		if (!addr)
			errors.push_back(i);
	if (errors.empty())
		std::cout << "LQ (Linked Queue) tests have passed!" << std::endl;
	else {
		std::cout << "LQ (Linked Queue) tests have failed!"
			<< "Errors found in tests:" << std::endl;
		for (index_t i = 0; i < errors.size(); i++) {
			std::cout << std::string(4, ' ') << errors[i] << std::endl;
		}
		return 0;
	}
	return 1;
}

int testArrayQueue() {
	constexpr int length = 10;
	ArrayQueue<int> myAQ { 0, 1, 2, 3, 4 };
	bool checks[length];
	for (int i = 0; i < 5; i++) {
		checks[i + 0] = myAQ.dequeue() == i;
		myAQ.enqueue(4 - i);
		checks[i + 5] = myAQ.front() == 4 - i;
	}
	index_t i = 0;
	SinglyLinkedList<unsigned long long> errors;
	for (bool* addr = checks; i < length; addr++, i++)
		if (!addr)
			errors.push_back(i);
	if (errors.empty())
		std::cout << "AQ (Array Queue) tests have passed!" << std::endl;
	else {
		std::cout << "AQ (Array Queue) tests have failed!"
			<< "Errors found in tests:" << std::endl;
		for (index_t i = 0; i < errors.size(); i++) {
			std::cout << std::string(4, ' ') << errors[i] << std::endl;
		}
		return 0;
	}
	return 1;
}

int testLinkedStack() {
	constexpr int length = 10;
	LinkedStack<int> myLS { 4, 3, 2, 1, 0 };
	bool checks[length];
	for (int i = 0; i < 5; i++) {
		checks[i + 0] = myLS.pop() == i;
		myLS.push(i);
		checks[i + 5] = myLS.top() == 4 - i;
	}
	index_t i = 0;
	SinglyLinkedList<unsigned long long> errors;
	for (bool* addr = checks; i < length; addr++, i++)
		if (!addr)
			errors.push_back(i);
	if (errors.empty())
		std::cout << "LS (Linked Stack) tests have passed!" << std::endl;
	else {
		std::cout << "LS (Linked Stack) tests have failed!"
			<< "Errors found in tests:" << std::endl;
		for (index_t i = 0; i < errors.size(); i++) {
			std::cout << std::string(4, ' ') << errors[i] << std::endl;
		}
		return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	SinglyLinkedList<int> results;
	results.push_back(testSinglyLinkedList());
	results.push_back(testCircularArray());
	results.push_back(testLinkedQueue());
	results.push_back(testArrayQueue());
	results.push_back(testLinkedStack());
	for (int i = 0; i < results.size(); i++) {
		if (!results[i]) {
			std::cout << std::endl << "Some tests have failed!" << std::endl;
			return 1;
		}
	}
	std::cout << std::endl << "All tests have passed!" << std::endl;
	return 0;
}
