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
	array::Array<int, 3> arr1{ 0, 5, 2 };
	array::Array<int, 3> arr2{ 0, 6, 2 };

	std::cout << (arr1 < arr2);
}