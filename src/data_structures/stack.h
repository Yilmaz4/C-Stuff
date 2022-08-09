#pragma once

#include <iostream>
#include <exception>

namespace stack {
	typedef size_t index_t;

	template <typename type> class LinkedStack final {
		struct Node {
			Node(type data, Node* ptr) {
				if (this->data)
					*this->data = data;
				this->ptr = ptr;
			}
			~Node() {
				free((void*)this->data);
			}
			// using malloc here because we will need to copy a local variable to
			// heap and I am not aware of any way of doing this with new operator
			type* data = static_cast<type*>(malloc(sizeof(type)));
			Node* ptr;
		};

		Node* top_ptr = nullptr;
		size_t length = 0;
	public:
		struct stack_empty_error : public std::exception {
			virtual const char* what() const throw() {
				return "Stack is empty";
			}
		};
		LinkedStack(void) = default;
		LinkedStack(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				push(obj);
			}
		}

		inline void push(type obj) {
			top_ptr = new Node(obj, top_ptr);
			length++;
		}
		inline type pop() {
			if (top_ptr == nullptr)
				throw stack_empty_error();
			type data = *top_ptr->data;
			Node* addr = top_ptr->ptr;
			delete top_ptr;
			top_ptr = addr;
			length--;
			return data;
		}
		inline type top() {
			if (top_ptr == nullptr)
				throw stack_empty_error();
			return *top_ptr->data;
		}
		inline size_t size() const {
			return length;
		}
		inline bool empty() const {
			return !this->length;
		}
		friend auto operator << (std::ostream& os, LinkedStack const& obj) -> std::ostream& {
			std::cout << "[";
			index_t i = 0;
			for (Node* addr = obj.top_ptr; addr; addr = addr->ptr, i++)
				std::cout << addr->data << ((i == obj.get_length() - 1) ? "" : ", ");
			std::cout << "]";
			return os;
		}
	};
}