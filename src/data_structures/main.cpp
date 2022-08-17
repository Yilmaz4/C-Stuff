#include <iostream>

#include "array.h"
#include "linkedlist.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"
#include "string.h"

using namespace tree;
using namespace string;

int main(int argc, char* argv[]) {
	linkedlist::SinglyLinkedList<int> myList{
		0, 1, 2, 3, 4
	};
	linkedlist::SinglyLinkedList<int> mySecondList{
		10, 11, 12, 13, 14
	};
	myList.merge(mySecondList);
	std::cout << myList;
}