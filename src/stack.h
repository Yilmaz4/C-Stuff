#pragma once

template <typename type> class Stack {
	void* top_ptr = nullptr;
	size_t length = 0;
protected:
	struct Node {
		Node(type data, Node* ptr) {
			this->data = (type*)malloc(sizeof(data));
			*this->data = data;
			this->ptr = ptr;
		}
		~Node() {
			free((void*)this->data);
		}
		type* data;
		Node* ptr;
	};
public:
	struct stack_empty_error : public std::exception {
		virtual const char* what() const throw() {
			return "Stack is empty";
		}
	};
	Stack(void) = default;

	inline void push(type obj) {
		top_ptr = (void*)new Node(obj, (Node*)top_ptr);
		length++;
	}
	inline type pop() {
		if (top_ptr == nullptr)
			throw stack_empty_error();
		type data = *((Node*)top_ptr)->data;
		Node* addr = ((Node*)top_ptr)->ptr;
		delete top_ptr;
		top_ptr = (void*)addr;
		length--;
		return data;
	}
	inline type top() {
		if (top_ptr == nullptr)
			throw stack_empty_error();
		return *((Node*)top_ptr)->data;
	}
	inline size_t size() const {
		return length;
	}
	inline bool empty() const {
		return !this->length;
	}
	friend auto operator << (std::ostream& os, Stack const& obj) -> std::ostream& {
		std::cout << "[";
		index_t i = 0;
		for (void* addr = obj.top_ptr; addr; addr = ((Node*)addr)->ptr, i++)
			std::cout << ((Node*)addr)->data << ((i == obj.get_length() - 1) ? "" : ", ");
		std::cout << "]";
		return os;
	}
};
