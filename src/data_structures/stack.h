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
		class Iterator {
			Node* addr = nullptr;
		public:
			Iterator(Node* ptr) : addr(ptr) {}

			type& operator * () const {
				return *addr->data;
			}
			Iterator& operator ++ () {
				addr = addr->ptr;
				return *this;
			}
			Iterator operator ++ (int) {
				Iterator temp = *this;
				++(*this);
				return temp;
			}
			friend bool operator == (const Iterator& a, const Iterator& b) {
				return a.addr == b.addr;
			}
			friend bool operator != (const Iterator& a, const Iterator& b) {
				return a.addr != b.addr;
			}
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

		void push(type obj) {
			top_ptr = new Node(obj, top_ptr);
			length++;
		}
		type pop() {
			if (top_ptr == nullptr)
				throw stack_empty_error();
			type data = *top_ptr->data;
			Node* addr = top_ptr->ptr;
			delete top_ptr;
			top_ptr = addr;
			length--;
			return data;
		}
		type top() {
			if (top_ptr == nullptr)
				throw stack_empty_error();
			return *top_ptr->data;
		}
		Iterator begin() {
			return Iterator(top_ptr);
		}
		Iterator end() {
			Node* addr = top_ptr;
			for (index_t i = 0; addr; addr = addr->ptr, i++);
			return Iterator(addr);
		}
		size_t size() const {
			return length;
		}
		bool empty() const {
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