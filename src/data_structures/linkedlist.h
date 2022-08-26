/*
Copyright © 2017-2022 Yılmaz Alpaslan

Permission is hereby granted, free of charge to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NOINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <iostream>
#include <exception>

namespace linkedlist {
	typedef size_t index_t;

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

	template <typename type> class SinglyLinkedList final {
		struct Node {
			Node(type const& data, Node* ptr) {
				if (this->data)
					*this->data = data;
				this->ptr = ptr;
			}
			Node(Node* ptr) {
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

		Node* base_addr = nullptr;
		Node* last_addr = nullptr;
		size_t length = 0;
	public:
		SinglyLinkedList(void) = default;
		SinglyLinkedList(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				push_back(obj);
			}
		}
		~SinglyLinkedList() {
			for (Node* next = nullptr, *addr = base_addr; addr; (next == addr) ? (addr = next) : (addr = nullptr)) {
				if (addr->ptr)
					next = addr->ptr;
				delete addr;
			}
		}

		Node* insert(type const& obj, index_t index) {
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
					last_addr->ptr = addr;
					last_addr = addr;
					length++;
					return addr;
				}
				Node* curr = this->base_addr;
				for (index_t i = 0; curr && i < index; curr = curr->ptr, i++) {
					if (i == index - 1) {
						addr = new Node(obj, curr->ptr);
						curr->ptr = addr;
					}
				}
				length++;
				return addr;
			}
		}
		Node* push_back(type const& obj) {
			return this->insert(obj, length);
		}
		void erase(index_t const index) {
			Node* next = base_addr->ptr;
			Node* curr = base_addr;
			Node* prev = nullptr;
			for (index_t i = 0; i <= index; i++) {
				if (i == index) {
					delete curr;
					if (prev)
						prev->ptr = next;
					else
						base_addr = next;
					return;
				}
				prev = curr;
				curr = curr->ptr;
				next = next->ptr;
			}
		}
		void remove(type const& obj) {
			erase(index(obj));
		}
		Iterator begin() {
			return Iterator(base_addr);
		}
		Iterator end() {
			return Iterator((last_addr) ? last_addr->ptr : last_addr);
		}
		void merge(SinglyLinkedList<type> const& list) noexcept {
			Node* addr = list.base_addr;
			for (size_t i = 0, j = 0; j < list.size(); i += 2, j++, addr = addr->ptr) {
				insert(*addr->data, i + 1);
			}
		}
		void reverse() {
			if (!base_addr)
				throw linkedlist::list_empty_error();
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
		bool has(type const& obj) {
			try {
				index(obj);
			}
			catch (...) {
				return false;
			}
			return true;
		}
		index_t index(type const& obj) {
			index_t i = 0;
			for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++) {
				if (*addr->data == obj)
					return i;
			}
			throw linkedlist::element_not_found_error();
		}
		type& at(const size_t index) const {
			return (*this)[index];
		}
		size_t size() const noexcept {
			return length;
		}
		bool empty() const noexcept {
			return !length;
		}
		type& operator [] (index_t index) {
			index_t i = 0;
			for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++)
				if (i == index)
					return *addr->data;
			throw linkedlist::index_error();
		}
		SinglyLinkedList& operator = (SinglyLinkedList const& obj) {
			for (size_t i = 0; i < obj.size(); i++) {
				try {
					operator[](i) = obj[i];
				}
				catch (linkedlist::index_error& exc) {
					insert(obj[i]);
				}
			}
		}
		friend auto operator << (std::ostream& os, SinglyLinkedList const& obj) -> std::ostream& {
			os << "[";
			index_t i = 0;
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr, i++)
				os << *(addr->data) << ((i != obj.size() - 1) ? ", " : "");
			os << "]";
			return os;
		}
	};

	template <typename type> class DoublyLinkedList final {
		struct Node {
			Node(type const& data, Node* ptr, Node* prv) {
				if (this->data)
					*this->data = data;
				this->ptr = ptr;
				this->prv = prv;
			}
			Node(Node* ptr) {
				this->ptr = ptr;
			}
			~Node() {
				free((void*)this->data);
			}
			// using malloc here because we will need to copy a local variable to
			// heap and I am not aware of any way of doing this with new operator
			type* data = static_cast<type*>(malloc(sizeof(type)));
			Node* ptr;
			Node* prv;
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

		Node* base_addr = nullptr;
		Node* last_addr = nullptr;
		size_t length = 0;
	public:
		DoublyLinkedList(void) = default;
		DoublyLinkedList(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				push_back(obj);
			}
		}
		~DoublyLinkedList() {
			for (Node* next = nullptr, addr = base_addr; addr; (next == addr) ? (addr = next) : (addr = nullptr)) {
				if (addr->ptr)
					next = addr->ptr;
				delete addr;
			}
		}

		Node* insert(type const& obj, index_t index) {
			if (!base_addr) {
				base_addr = new Node(obj, nullptr, nullptr);
				last_addr = base_addr;
				length++;
				return base_addr;
			}
			Node* addr = nullptr;
			switch (index) {
			case 0:
				addr = new Node(obj, base_addr, nullptr);
				base_addr = addr;
				length++;
				return addr;
			default:
				if (index >= length) {
					addr = new Node(obj, nullptr, last_addr);
					last_addr->ptr = addr;
					last_addr = addr;
					length++;
					return addr;
				}
				Node* curr = this->base_addr;
				for (index_t i = 0; curr && i < index; curr = curr->ptr, i++) {
					if (i == index - 1) {
						length++;
						return curr->ptr = new Node(obj, curr->ptr, curr->prv);
					}
				}
			}
		}
		Node* push_back(type const& obj) {
			return this->insert(obj, length);
		}
		void erase(index_t const index) {
			Node* next = base_addr->ptr;
			Node* curr = base_addr;
			Node* prev = nullptr;
			for (index_t i = 0; i <= index; i++) {
				if (i == index) {
					delete curr;
					if (prev)
						prev->ptr = next;
					else
						base_addr = next;
					return;
				}
				prev = curr;
				curr = curr->ptr;
				next = next->ptr;
			}
		}
		void remove(type const& obj) {
			erase(index(obj));
		}
		Iterator begin() {
			return Iterator(base_addr);
		}
		Iterator end() {
			return Iterator((last_addr) ? last_addr->ptr : last_addr);
		}
		void merge(DoublyLinkedList<type> const& list) noexcept {
			Node* addr = list.base_addr;
			for (size_t i = 0, j = 0; j < list.size(); i += 2, j++, addr = addr->ptr) {
				insert(*addr->data, i + 1);
			}
		}
		void reverse() {
			if (!base_addr)
				throw linkedlist::list_empty_error();
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
		bool has(type const& obj) {
			try {
				index(obj);
			}
			catch (...) {
				return false;
			}
			return true;
		}
		index_t index(type const& obj) {
			index_t i = 0;
			for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++) {
				if (*addr->data == obj)
					return i;
			}
			throw linkedlist::element_not_found_error();
		}
		type& at(index_t const index) const {
			return (*this)[index];
		}
		size_t size() const noexcept {
			return length;
		}
		bool empty() const noexcept {
			return !length;
		}
		type& operator [] (index_t index) {
			index_t i = 0;
			for (Node* addr = this->base_addr; addr; addr = addr->ptr, i++)
				if (i == index)
					return *addr->data;
			throw linkedlist::index_error();
		}
		DoublyLinkedList& operator = (DoublyLinkedList const& obj) {
			for (size_t i = 0; i < obj.size(); i++) {
				try {
					operator[](i) = obj[i];
				}
				catch (linkedlist::index_error& exc) {
					insert(obj[i]);
				}
			}
		}
		friend auto operator << (std::ostream& os, DoublyLinkedList const& obj) -> std::ostream& {
			os << "[";
			index_t i = 0;
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr, i++)
				os << *(addr->data) << ((i != obj.size() - 1) ? ", " : "");
			os << "]";
			return os;
		}
	};
}