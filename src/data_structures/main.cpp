#include <iostream>

#include "circulararray.h"
#include "linkedlist.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"
#include "string.h"

using namespace tree;
using namespace string;

int main(int argc, char* argv[]) {
	queue::LinkedQueue<int> myList {
		0, 5, 2, 7, 3, 1
	};
	for (auto const& obj : myList) {
		std::cout << obj;
	}
}