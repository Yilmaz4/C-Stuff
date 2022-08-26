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

namespace array {
	typedef size_t index_t;

	struct out_of_range : public std::exception {
		virtual const char* what() const noexcept {
			return "Array index out of range";
		}
	};

	template <typename T>
	concept supports_comparison = requires(T obj1, T obj2) {
		{obj1 > obj2} noexcept -> std::convertible_to<bool>;
		{obj1 < obj2} noexcept -> std::convertible_to<bool>;
	};

	template <typename type, size_t length> class Array final {
		class Iterator {
			type* m_ptr = nullptr;
		public:
			Iterator(type* ptr) : m_ptr(ptr) {}

			type& operator * () const {
				return *m_ptr;
			}
			Iterator& operator ++ () {
				m_ptr++;
				return *this;
			}
			Iterator operator ++ (int) {
				Iterator temp = *this;
				m_ptr++;
				return temp;
			}
			friend bool operator == (const Iterator& a, const Iterator& b) {
				return a.m_ptr == b.m_ptr;
			}
			friend bool operator != (const Iterator& a, const Iterator& b) {
				return a.m_ptr != b.m_ptr;
			}
		};

		type* base;
	public:
		explicit Array(void) {
			base = static_cast<type*>(calloc(length, sizeof(type)));
		}
		Array(std::initializer_list<type> const& list) {
			base = static_cast<type*>(calloc(length, sizeof(type)));
			for (size_t i = 0; i < length; i++) {
				operator[](i) = *(list.begin() + i);
			}
		}
		~Array() {
			free((void*)base);
		}
		type& at(size_t n) {
			return operator[](n);
		}
		const type& at(size_t n) const {
			return operator[](n);
		}
		
		type& back() {
			return *(base + length - 1);
		}
		const type& back() const {
			return *(base + length - 1);
		}

		Iterator begin() noexcept {
			return Iterator(base);
		}
		const Iterator begin() const noexcept {
			return Iterator(base);
		}

		type* data() {
			return base;
		}
		const type* data() const {
			return base;
		}

		constexpr bool empty() noexcept {
			return length;
		}

		void fill(const type& val) {
			type* addr = base;
			for (size_t i = 0; i < length; i++) {
				*(addr + i) = val;
			}
		}

		type& front() {
			return *base;
		}
		const type& front() const {
			return *base;
		}

		constexpr size_t max_size() noexcept {
			return length;
		}

		constexpr size_t size() noexcept {
			return length;
		}

		template <typename Array_T> requires std::same_as<Array_T, Array> && requires(Array_T obj) {
			{ obj.max_size() } noexcept -> std::convertible_to<size_t>;
			requires obj.max_size() == length;
		} void swap(Array_T& x) {
			for (size_t i = 0; i < length; i++) {
				type tmp = operator[](i);
				at(i) = x[i];
				x[i] = tmp;
			}
		}

		type& operator [] (size_t n) {
			if (n >= length)
				throw array::out_of_range();
			return *(base + n);
		}
		const type& operator [] (size_t n) const {
			if (n >= length)
				throw array::out_of_range();
			return *(base + n);
		}

		template <typename T, size_t N> friend bool operator == (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] != rhs[i]) {
					return false;
				}
			}
			return true;
		}
		template <typename T, size_t N> friend bool operator != (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] == rhs[i]) {
					return false;
				}
			}
			return true;
		}
		template <typename T, size_t N> requires supports_comparison<T>
		friend bool operator <= (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] != rhs[i] || lhs[i] > rhs[i]) {
					return false;
				}
			}
			return true;
		}
		template <typename T, size_t N> requires supports_comparison<T>
		friend bool operator >= (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] != rhs[i] || lhs[i] < rhs[i]) {
					return false;
				}
			}
			return true;
		}
		template <typename T, size_t N> requires supports_comparison<T>
		friend bool operator <  (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] == rhs[i] || lhs[i] > rhs[i]) {
					return false;
				}
			}
			return true;
		}
		template <typename T, size_t N> requires supports_comparison<T>
		friend bool operator >  (const Array<T, N>& lhs, const Array<T, N>& rhs) {
			for (size_t i = 0; i < N; i++) {
				if (lhs[i] == rhs[i] || lhs[i] < rhs[i]) {
					return false;
				}
			}
			return true;
		}
	};

	template <typename type> class CircularArray final {
		type* base;
		size_t length = 0;
	public:
		explicit CircularArray(const size_t& size) : length(size) {
			base = static_cast<type*>(malloc(length * sizeof type));
		}
		CircularArray(std::initializer_list<type> const& list) {
			length = list.size();
			base = static_cast<type*>(malloc(length * sizeof type));
			for (index_t i = 0; i < length; i++) {
				operator[](i) = *((type*)list.begin() + i);
			}
		}
		~CircularArray() {
			free((void*)base);
		}
		void print() const {
			std::cout << "[";
			for (index_t i = 0; i < length; i++)
				std::cout << (*this)[i] << ((i != length - 1) ? ", " : "");
			std::cout << "]";
		}
		size_t size() const noexcept {
			return length;
		}
		bool empty() const noexcept {
			return !length;
		}
		type& operator [] (index_t index) const {
			return *(base + (index % length));
		}
		operator int* __ptr64() const noexcept {
			return base;
		}
	};
}