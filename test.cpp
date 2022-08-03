#include <iostream>
#include <algorithm>
#include <iomanip>

#include "src\linkedlist.h"
#include "src\stack.h"
#include "src\queue.h"
#include "src\circulararray.h"

bool testSinglyLinkedList() {
	SinglyLinkedList<int> mySLL;
	for (int i = 0; i < 10; i++) {
		mySLL.push_back(i);
	}
	bool checks[10 * 4]{};
	for (int i = 0; i < 10; i++) {
		checks[i] = mySLL[i] == i;
	}
	for (int i = 0; i < 10; i++) {
		mySLL[i] = i * 2;
		checks[i + 10] = i == mySLL[i] / 2;
	}
	mySLL.reverse();
	for (int i = 9; i >= 0; i--) {
		checks[i + 20] = mySLL[9 - i] == i * 2;
	}
	for (int i = 0; i < 10; i++) {
		mySLL.remove(i * 2);
		checks[i + 30] = !mySLL.has(i);
	}
	return (std::all_of(
		std::begin(checks),
		std::end(checks),
		[](bool i) {
			return i;
		})
	);
}

int main() {
	std::cout << std::boolalpha << testSinglyLinkedList();
}
