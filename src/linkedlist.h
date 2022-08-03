#pragma once

#include <iostream>
#include <exception>

typedef unsigned long long index_t;

template <typename type> class SinglyLinkedList {
	struct Node {
		Node(type data, Node* ptr) {
			this->data = (type*)malloc(sizeof(data));
			*this->data = data;
			this->ptr = ptr;
		}
		Node(Node* ptr) {
			this->ptr = ptr;
		}
		type* data;
		Node* ptr;
	};
	class Proxy {
		Node* node = nullptr;
		type* base_addr = nullptr;
	public:
		Proxy(Node* node, type* base_addr) {
			this->node = node;
			this->base_addr = base_addr;
		}
		Proxy& operator = (type obj) noexcept {
			node->data = obj;
			return *this;
		}
	};
	Node* base_addr = nullptr;
	Node* last_addr = nullptr;
	size_t length = 0;
public:
	struct index_error : public std::exception {
		virtual const char* what() const noexcept {
			return "List index out of range";
		}
	};
	struct list_empty_error : public std::exception {
		virtual const char* what() const noexcept {
			return "List is empty";
		}
	};
	struct element_not_found_error : public std::exception {
		virtual const char* what() const noexcept {
			return "Specified element was not found in the list";
		}
	};
	SinglyLinkedList(void) = default;

	inline Node* insert(type const& obj, index_t index) {
		if (!base_addr) {
			base_addr = new Node(obj, nullptr);
			last_addr = base_addr;
			length++;
			return base_addr;
		}
		Node* addr = nullptr;
		switch (index) {
		case 0:
			addr = new Node(obj, base_addr);
			base_addr = addr;
			length++;
			return addr;
		default:
			if (index >= length) {
				addr = new Node(obj, nullptr);
				((Node*)last_addr)->ptr = addr;
				last_addr = addr;
				length++;
				return addr;
			}
			index_t i = 0;
			for (Node* t_addr = this->base_addr; t_addr && i < index + 1; t_addr = t_addr->ptr, i++) {
				if (i == index) {
					addr = new Node(obj, nullptr);
					t_addr->ptr = addr;
				}
				else if (i > index)
					addr->ptr = t_addr;
			}
			length++;
			return addr;
		}
	}
	inline Node* push_back(type const& obj) {
		return this->insert(obj, length);
	}
	inline void erase(index_t index) {
		index_t i = 0;
		for (Node* addr = base_addr; i < index; i++, addr = addr->ptr) {
			if (i == index - 1) {
				Node* temp = addr->ptr;
				addr->ptr = addr->ptr->ptr;
				delete addr->ptr;
			}
		}
	}
	inline void remove(type const& obj) {
		erase(index(obj));
	}
	inline void reverse() {
		if (!base_addr)
			throw list_empty_error();
		Node* prev = nullptr;
		Node* addr = base_addr;
		Node* next = base_addr->ptr;
		for (index_t i = 0; addr; i++, addr = next) {
			next = addr->ptr;
			addr->ptr = prev;
			prev = addr;
		}
		base_addr = prev;
	}
	inline bool has(type const& obj) {
		try {
			index(obj);
		}
		catch (...) {
			return false;
		}
		return true;
	}
	inline index_t index(type const& obj) {
		index_t i = 0;
		for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++) {
			if (*addr->data == obj)
				return i;
		}
		throw element_not_found_error();
	}
	inline type& at(index_t index) {
		return (*this)[index];
	}
	inline size_t size() const noexcept {
		return length;
	}
	inline bool empty() const noexcept {
		return !length;
	}
	type& operator [] (int index) {
		index_t i = 0;
		for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++)
			if (i == index)
				return *addr->data;
		throw index_error();
	}
	friend auto operator << (std::ostream& os, SinglyLinkedList const& obj) -> std::ostream& {
		std::cout << "[";
		index_t i = 0;
		for (Node* addr = obj.base_addr; addr; addr = addr->ptr, i++)
			std::cout << *(addr->data) << ((i != obj.size() - 1) ? ", " : "");
		std::cout << "]";
		return os;
	}
};