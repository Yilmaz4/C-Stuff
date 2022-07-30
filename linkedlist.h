#pragma once

#include <iostream>
#include <format>
#include <exception>

typedef unsigned long long index_t;

template <typename type> class SinglyLinkedList {
	void* base_addr = nullptr;
	void* last_addr = nullptr;
	size_t length = 0;
protected:
	struct Node {
		Node(type data, Node* ptr) {
			this->data = data;
			this->ptr = ptr;
		}
		Node(Node* ptr) {
			this->ptr = ptr;
		}
		type data;
		Node* ptr;
	};
	struct Proxy {
		Proxy(Node* node) {
			this->node = node;
		}
		Node* node;
		Proxy& operator = (type obj) noexcept {
			node->data = obj;
			return *this;
		}
	};
public:
	struct index_error : public std::exception {
		virtual const char* what() const throw() {
			return "List index out of range";
		}
	};
	SinglyLinkedList(void) = default;

	void* insert(type obj, index_t index) {
		if (!base_addr) {
			base_addr = new Node(obj, nullptr);
			last_addr = base_addr;
			length++;
			return base_addr;
		}
		Node* addr = nullptr;
		switch (index) {
		case 0:
			addr = new Node(obj, (Node*)base_addr);
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
			for (void* t_addr = this->base_addr; t_addr && i < index + 1; t_addr = ((Node*)t_addr)->ptr, i++) {
				if (i == index) {
					addr = new Node(obj, nullptr);
					((Node*)t_addr)->ptr = addr;
				}
				else if (i > index)
					addr->ptr = (Node*)t_addr;
			}
			length++;
			return addr;
		}
	}
	void* append(type obj) {
		return this->insert(obj, length);
	}
	void remove(type obj) {
		Node* temp_addr = nullptr;
		for (Node* addr = (Node*)(this->base_addr); addr; addr = addr->ptr) {
			if (addr->data == obj) {
				delete addr;
				continue;
			}
			if (temp_addr && temp_addr->ptr != addr) {
				temp_addr->ptr = addr;
				break;
			}
			temp_addr = addr;
		}
		length--;
	}
	void reverse() {
		Node* prev = nullptr;
		Node* addr = (Node*)base_addr;
		Node* next = ((Node*)base_addr)->ptr;
		for (index_t i = 0; addr; i++, addr = next) {
			next = addr->ptr;
			addr->ptr = prev;
			prev = addr;
		}
		base_addr = prev;
	}
	void recursively_reverse(Node* p = (Node*)base_addr) {
		if (p->ptr == nullptr) {
			base_addr = p;
			return;
		}
		recursively_reverse(p->ptr);
		Node* q = p->ptr;
		q->ptr = p;
		p->ptr = nullptr;
	}
	index_t& index(type obj) {
		index_t i = 0;
		for (void* addr = this->base_addr; addr; addr = ((Node*)addr)->ptr, i++) {
			Node* addr_obj = ((Node*)addr);
			if (addr_obj->data == obj)
				return i;
		}
	}
	Proxy at(index_t index) {
		if (index >= length) {
			Node* node = new Node(nullptr);
			((Node*)last_addr)->ptr = node;
			last_addr = node;
			return Proxy(node);
		}
		Node* addr = (Node*)base_addr;
		for (index_t i = 0; i < index && addr->ptr; i++)
			addr = addr->ptr;
		return Proxy(addr);
	}
	size_t get_length() const {
		return length;
	}
	type operator [] (int index) {
		index_t i = 0;
		for (void* addr = this->base_addr; addr; addr = ((Node*)addr)->ptr, i++)
			if (i == index) return ((Node*)addr)->data;
		throw index_error();
	}
	friend auto operator << (std::ostream& os, SinglyLinkedList const& obj) -> std::ostream& {
		std::cout << "[";
		index_t i = 0;
		for (void* addr = obj.base_addr; addr; addr = ((Node*)addr)->ptr, i++)
			std::cout << ((Node*)addr)->data << ((i == obj.get_length() - 1) ? "" : ", ");
		std::cout << "]";
		return os;
	}
};