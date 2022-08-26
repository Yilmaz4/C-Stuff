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

namespace queue {
	typedef size_t index_t;
	
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
		LinkedQueue(void) = default;
		LinkedQueue(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				push(obj);
			}
		}

		type& back() {
			if (!length)
				throw queue::queue_empty_error();
			return *last_addr->data;
		}
		const type& back() const {
			if (!length)
				throw queue::queue_empty_error();
			return *last_addr->data;
		}

		bool empty() const {
			return !length;
		}

		type& front() {
			if (!length)
				throw queue::queue_empty_error();
			return *base_addr->data;
		}
		const type& front() const {
			if (!length)
				throw queue::queue_empty_error();
			return *base_addr->data;
		}

		void pop() {
			if (empty())
				throw queue::queue_empty_error();
			length--;
			base_addr = base_addr->ptr;
			if (!base_addr)
				last_addr = nullptr;
			delete base_addr;
		}

		void push(const type& obj) {
			length++;
			if (!last_addr) {
				base_addr = last_addr = new Node(obj, nullptr);
				return;
			}
			last_addr->ptr = new Node(obj, nullptr);
			last_addr = last_addr->ptr;
		}

		size_t size() const {
			return length;
		}

		void swap(LinkedQueue<type>& x) noexcept {

		}
		
		operator type& () const {
			return front();
		}

		template <typename T> friend bool operator == (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}
		template <typename T> friend bool operator != (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}
		template <typename T> requires supports_comparison<T>
		friend bool operator <= (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}
		template <typename T> requires supports_comparison<T>
		friend bool operator >= (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}
		template <typename T> requires supports_comparison<T>
		friend bool operator <  (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}
		template <typename T> requires supports_comparison<T>
		friend bool operator >  (const LinkedQueue<T>& lhs, const LinkedQueue<T>& rhs) {
			
		}

		friend auto operator << (std::ostream& os, LinkedQueue const& obj) -> std::ostream& {
			os << "[";
			index_t i = 0;
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr, i++)
				os << *(addr->data) << ((i != obj.size() - 1) ? ", " : "");
			os << "]";
			return os;
		}
	};

	template <typename type, size_t size_multiplier = 4> class ArrayQueue final {
		type* base_addr;
		index_t head = NULL;
		index_t tail = NULL;
		size_t length = 0;
	public:
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
			os << "[";
			for (index_t i = 0; i < obj.length; i++)
				os << obj.base_addr[i] << ((i != obj.length - 1) ? ", " : "");
			os << "]";
			return os;
		}
	};
}