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
	LinkedString str1;

	for (auto const& c : str1) {
		std::cout << c;
	}
	//std::cout << str1.find('w', 0) << std::endl;
}