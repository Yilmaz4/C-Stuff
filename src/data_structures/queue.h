#pragma once

#include <iostream>
#include <exception>

namespace queue {
	typedef size_t index_t;

	template <typename type> class LinkedQueue final {
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

		Node* base_addr = nullptr;
		Node* last_addr = nullptr;
		size_t length = 0;
	public:
		struct queue_empty_error : public std::exception {
			virtual const char* what() const noexcept {
				return "Queue is empty";
			}
		};
		LinkedQueue(void) = default;
		LinkedQueue(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				push(obj);
			}
		}

		void push(type const& obj) {
			length++;
			if (!last_addr) {
				base_addr = last_addr = new Node(obj, nullptr);
				return;
			}
			last_addr->ptr = new Node(obj, nullptr);
			last_addr = last_addr->ptr;
		}
		type& pop() {
			if (empty())
				throw queue_empty_error();
			Node* temp = base_addr;
			type* value = temp->data;
			base_addr = temp->ptr;
			if (!base_addr)
				last_addr = nullptr;
			delete temp;
			length--;
			return *value;
		}
		type& front() const {
			if (empty())
				throw queue_empty_error();
			return *base_addr->data;
		}
		bool empty() const noexcept {
			return !length;
		}
		size_t size() const noexcept {
			return length;
		}
		operator type& () const {
			return front();
		}
		friend auto operator << (std::ostream& os, LinkedQueue const& obj) -> std::ostream& {
			std::cout << "[";
			index_t i = 0;
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr, i++)
				std::cout << *(addr->data) << ((i != obj.size() - 1) ? ", " : "");
			std::cout << "]";
			return os;
		}
	};

	template <typename type, size_t size_multiplier = 4> class ArrayQueue {
		type* base_addr;
		index_t head = NULL;
		index_t tail = NULL;
		size_t length = 0;
	public:
		struct queue_empty_error : public std::exception {
			virtual const char* what() const noexcept {
				return "Queue is empty";
			}
		};
		struct queue_full_error : public std::exception {
			virtual const char* what() const noexcept {
				return "Queue is full";
			}
		};
		struct invalid_size_multiplier_error : public std::exception {
			virtual const char* what() const noexcept {
				return "Size multiplier must be greater than 0";
			}
		};
		explicit ArrayQueue(const size_t size) : length(size) {
			base_addr = new type[size];
		}
		ArrayQueue(std::initializer_list<type> const& list) {
			if (!size_multiplier)
				throw invalid_size_multiplier_error();
			length = list.size() * size_multiplier;
			base_addr = new type[length];
			for (auto const& obj : list)
				enqueue(obj);
		}
		void push(type const& obj) {
			if (!head && tail == length)
				throw queue_full_error();
			base_addr[tail % length] = obj;
			tail++;
		}
		type& pop() {
			if (head == tail)
				throw queue_empty_error();
			type& val = base_addr[head];
			head++;
			return val;
		}
		type& front() const {
			if (head == tail)
				throw queue_empty_error();
			return base_addr[head];
		}
		size_t size() const noexcept {
			return tail - head;
		}
		type* get_array() const noexcept {
			return base_addr;
		}
		operator type& () const {
			return front();
		}
		friend auto operator << (std::ostream& os, ArrayQueue const& obj) -> std::ostream& {
			std::cout << "[";
			for (index_t i = 0; i < obj.length; i++)
				std::cout << obj.base_addr[i] << ((i != obj.length - 1) ? ", " : "");
			std::cout << "]";
			return os;
		}
	};
}