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
	LinkedString str2 = "Hello World!";
	std::cout << str2.substr(2, 5);
}