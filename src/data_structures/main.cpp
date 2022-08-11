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
	std::string str1 = "Hello World!";
	LinkedString str2 = "Hello Neighbor!";
	
	char cArray[15];
	std::cout << str2.copy(cArray, 20, 0);
	
}