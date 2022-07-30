#pragma once

template <typename type> class LinkedStack {
	void* top = nullptr;
	size_t length = 0;
protected:
	struct Node {
		Node(type data, Node* ptr) {
			this->data = data;
			this->ptr = ptr;
		}
		type data;
		Node* ptr;
	};
public:
	struct empty_stack_error : public std::exception {
		virtual const char* what() const throw() {
			return "Stack is empty";
		}
	};
	LinkedStack(void) = default;

	void push(type obj) {
		top = (void*)new Node(obj, (Node*)top);
		length++;
	}
	type pop() {
		if (top == nullptr)
			throw empty_stack_error();
		type data = ((Node*)top)->data;
		Node* addr = ((Node*)top)->ptr;
		delete top;
		top = (void*)addr;
		length--;
		return data;
	}
	type get() {
		if (top == nullptr)
			throw empty_stack_error();
		return ((Node*)top)->data;
	}
	size_t get_length() const {
		return length;
	}
	friend auto operator << (std::ostream& os, LinkedStack const& obj) -> std::ostream& {
		std::cout << "[";
		index_t i = 0;
		for (void* addr = obj.top; addr; addr = ((Node*)addr)->ptr, i++)
			std::cout << ((Node*)addr)->data << ((i == obj.get_length() - 1) ? "" : ", ");
		std::cout << "]";
		return os;
	}
};
