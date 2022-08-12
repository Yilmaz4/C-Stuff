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
	LinkedString str1 = "hELLO wORLD!";
	LinkedString str2 = "Hello World!";
	
	str1.swap(str2);
	std::cout << str1 << std::endl << str2;
}