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
#include <sstream>
#include <cstdlib>

namespace string {
	constexpr size_t npos = -1;

	struct out_of_range : public std::exception {
		virtual const char* what() const noexcept {
			return "String index out of range";
		}
	};
	struct empty_string_error : public std::exception {
		virtual const char* what() const noexcept {
			return "String is empty";
		}
	};

	template <typename char_t> requires std::convertible_to<char_t, char32_t>
	class BasicString final {
		char_t* base = nullptr;
		size_t capacity = 0;
		size_t size = 0;
	public:

	};

	template <typename char_t> requires std::convertible_to<char_t, char32_t>
	class BasicLinkedString final {
		struct Node final {
			Node(const char_t data_p, Node* const ptr_p, Node* const prv_p) {
				data = data_p;
				ptr = ptr_p;
				prv = prv_p;
			}
			char_t data;
			Node* ptr = nullptr;
			Node* prv = nullptr;
		};

		Node* base_addr = nullptr;
		Node* last_addr = nullptr;
		size_t length_v = 0;
	public:
		struct Iterator final {
			Iterator(Node* ptr) : addr(ptr) {}

			char_t& operator * () const {
				return addr->data;
			}
			Iterator& operator ++ () {
				addr = addr->ptr;
				return *this;
			}
			Iterator operator ++ (int) {
				Iterator temp = *this;
				addr = addr->ptr;
				return temp;
			}
			friend bool operator == (const Iterator& a, const Iterator& b) {
				return a.addr == b.addr;
			}
			friend bool operator != (const Iterator& a, const Iterator& b) {
				return a.addr != b.addr;
			}
		private:
			Node* addr = nullptr;
		};

		BasicLinkedString(void) = default;
		BasicLinkedString(const BasicLinkedString& str) {
			for (size_t i = 0; i < str.size(); i++) push_back(str[i]);
		}
		BasicLinkedString(const std::string& str) {
			for (size_t i = 0; i < str.size(); i++) push_back(str[i]);
		}
		BasicLinkedString(const BasicLinkedString& str, size_t pos, size_t len = string::npos) {
			if (pos > str.size() || pos > len)
				throw string::out_of_range();
			for (size_t i = pos, j = 0; j < ((len == string::npos) ? length_v : len); i++, j++) {
				push_back(str[i]);
			}
		}
		BasicLinkedString(const std::string& str, size_t pos, size_t len = string::npos) {
			if (pos > str.size() || pos > len)
				throw string::out_of_range();
			for (size_t i = pos, j = 0; j < ((len == string::npos) ? length_v : len); i++, j++) {
				push_back(str[i]);
			}
		}
		BasicLinkedString(const char_t* s) {
			size_t i = 0;
			for (char_t c = *s; c != '\0'; i++, c = *(s + i)) push_back(c);
		}
		BasicLinkedString(const char_t* s, size_t n) {
			size_t i = 0;
			for (char_t c = *s; i < n; i++, c = *(s + i)) {
				push_back(c);
			}
		}
		BasicLinkedString(size_t n, char_t c) {
			for (size_t i = 0; i < n; i++) push_back(c);
		}
		BasicLinkedString(std::initializer_list<char_t> il) {
			for (char_t const& c : il) push_back(c);
		}

		~BasicLinkedString(void) {
			Node** to_del = new Node*[length_v];
			size_t i = 0;
			for (Node* node = base_addr; node; node = node->ptr, i++)
				to_del[i] = node;
			for (size_t i = 0; i < length_v; i++)
				delete to_del[i];
			last_addr = base_addr = nullptr;
			length_v = 0;
		}

		BasicLinkedString& append(const BasicLinkedString& str) {
			return operator += (str);
		}
		BasicLinkedString& append(const BasicLinkedString& str, size_t subpos, size_t sublen = string::npos) {
			if (subpos > str.size() || subpos > sublen)
				throw string::out_of_range();
			for (size_t i = subpos; subpos < ((sublen == string::npos) ? str.size() : sublen); i++)
				push_back(str[i]);
			return (*this);
		}
		BasicLinkedString& append(const char_t* s) {
			return operator += (s);
		};
		BasicLinkedString& append(const char_t* s, size_t n) {
			size_t i = 0;
			for (char_t c = *s; i < n; i++, c = *(s + 1))
				push_back(c);
			return (*this);
		}
		BasicLinkedString& append(size_t n, char_t c) {
			for (size_t i = 0; i < n; i++)
				push_back(c);
			return (*this);
		}
		BasicLinkedString& append(std::initializer_list<char_t> il) {
			return operator += (il);
		}

		BasicLinkedString& assign(const BasicLinkedString& str) {
			return operator = (str);
		}
		BasicLinkedString& assign(const BasicLinkedString& str, size_t subpos, size_t sublen = string::npos) {
			this->~BasicLinkedString();
			if (subpos > str.size() || subpos > sublen)
				throw string::out_of_range();
			for (size_t i = subpos; subpos < ((sublen == string::npos) ? str.size() : sublen); i++) {
				push_back(str[i]);
			}
		}
		BasicLinkedString& assign(const char_t* s) {
			return operator = (s);
		}
		BasicLinkedString& assign(const char_t* s, size_t n) {
			this->~BasicLinkedString();
			size_t i = 0;
			for (char_t c = *s; i < n; i++, c = *(s + 1)) push_back(c);
		}
		BasicLinkedString& assign(size_t n, char_t c) {
			this->~BasicLinkedString();
			for (size_t i = 0; i < n; i++) push_back(c);
		}
		BasicLinkedString& assign(std::initializer_list<char_t> il) {
			return operator = (il);
		}

		inline char_t& at(size_t pos) {
			return operator[](pos);
		}
		inline const char_t& at(size_t pos) const {
			return operator[](pos);
		}

		inline char_t& back() {
			return operator[](size() - 1);
		}
		inline const char_t& back() const {
			return operator[](size() - 1);
		}

		Iterator begin() {
			return Iterator(base_addr);
		}
		const Iterator begin() const {
			return Iterator(base_addr);
		}

		inline void clear() noexcept {
			this->~BasicLinkedString();
			base_addr = nullptr;
		}

		__pragma(warning(disable:4715)); // 'string::BasicLinkedString::copy': not all control paths return a value. (It'll eventually return in case of good input)
		size_t copy(char_t* s, size_t len, size_t pos = 0) const {
			if (pos > length_v)
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; i < len + 1 && i < length_v; addr = addr->ptr, i++, j++) {
				if (i == len)
					return i;
				*(s + j) = (*this)[i];
			}
		}

		inline bool empty() const noexcept {
			return length_v;
		}

		Iterator end() {
			return Iterator((last_addr) ? last_addr->ptr : last_addr);
		}
		const Iterator end() const {
			return Iterator((last_addr) ? last_addr->ptr : last_addr);
		}

		BasicLinkedString& erase(size_t pos = 0, size_t len = string::npos) {
			len = ((len == string::npos) ? length_v : len);
			Node* addr = base_addr;
			Node* temp = addr;
			for (size_t i = 0, j = 0; i < len; addr = addr->ptr, i++) {
				if (i == pos)
					temp = addr;
				if (pos < i && j < len) {
					delete addr;
					j++;
				}
				else {
					temp->ptr = addr;
					break;
				}
			}
		}
		
		__pragma(warning(disable:6295)); // Ill-defined for-loop. Loop executes infinitely. (It does not)
		__pragma(warning(disable:6386)); // Buffer overrun while writing to 'matchs'. (There's no buffer overrun)
		size_t find(const BasicLinkedString& str, size_t pos = 0) const noexcept {
			if (pos > length_v)
				return string::npos;
			size_t length = str.size() - pos;
			bool* matchs = new bool[length];
			for (size_t i = 0; i < length; i++)
				matchs[i] = false;
			size_t start_index = string::npos;
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++) {
				auto j = string::npos;
				for (size_t k = length - 1; k >= 0; (k != 0) ? k-- : k = -1) {
					if (k == string::npos || *(matchs + k)) {
						j = k;
						break;
					}
				}
				bool all_true = true;
				for (size_t k = 0; k < length; k++) {
					if (!matchs[k]) {
						all_true = false;
						break;
					}
				}
				if (j != string::npos && !all_true) {
					if (operator[](i) == str[j + 1])
						matchs[j + 1] = true;
					else {
						for (size_t i = 0; i < length; i++) {
							matchs[i] = false;
						}
					}
				}
				else if (all_true)
					return start_index;
				else {
					if (operator[](i) == str[0])
						matchs[0] = true;
					start_index = i;
				}
			}
			for (size_t i = 0; i < length; i++)
				if (!matchs[i])
					start_index = string::npos;
			delete[] matchs;
			return start_index;
		}
		size_t find(const char_t* s, size_t pos = 0) const {
			return this->find(static_cast<BasicLinkedString>(s), pos);
		}
		size_t find(const char_t* s, size_t pos, size_t n) const {
			return this->find(BasicLinkedString(s, n), pos);
		}
		size_t find(char_t c, size_t pos = 0) const noexcept {
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		size_t find_first_not_of(const BasicLinkedString& str, size_t pos = 0) const noexcept {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) != str[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(const char_t* s, size_t pos = 0) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(const char_t* s, size_t pos, size_t n) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(char_t c, size_t pos = 0) const noexcept {
			// Time complexity: O(n - pos)
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) != c)
					return i;
			return string::npos;
		}

		size_t find_first_of(const BasicLinkedString& str, size_t pos = 0) const noexcept {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) == str[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(const char_t* s, size_t pos = 0) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(const char_t* s, size_t pos, size_t n) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(char_t c, size_t pos = 0) const noexcept {
			// Time complexity: O(n - pos)
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		size_t find_last_not_of(const BasicLinkedString& str, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) != str[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(const char_t* s, size_t pos = string::npos) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(const char_t* s, size_t pos, size_t n) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(char_t c, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				if (operator[](i) != c)
					return i;
			return string::npos;
		}

		size_t find_last_of(const BasicLinkedString& str, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) == str[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(const char_t* s, size_t pos = string::npos) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(const char_t* s, size_t pos, size_t n) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(char_t c, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		inline char_t& front() {
			if (!base_addr)
				throw string::empty_string_error();
			return base_addr->data;
		}
		inline const char_t& front() const {
			if (!base_addr)
				throw string::empty_string_error();
			return base_addr->data;
		}

		BasicLinkedString& insert(size_t pos, const BasicLinkedString& str) {
			if (pos > length_v)
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = 0; i < pos && addr; i++)
				addr = addr->ptr;
			str = str.reverse();
			for (auto const& c : str)
				((addr->prv) ? addr->prv->ptr : base_addr) = new Node(c, ((addr->prv) ? addr->prv->ptr : base_addr), addr->prv);
			return *this;
		}
		BasicLinkedString& insert(size_t pos, const BasicLinkedString& str, size_t subpos, size_t sublen = string::npos) {
			return insert(pos, str.substr(subpos, sublen));
		}
		BasicLinkedString& insert(size_t pos, const char_t* s) {
			return insert(pos, static_cast<BasicLinkedString>(s));
		}
		BasicLinkedString& insert(size_t pos, const char_t* s, size_t n) {
			return insert(pos, BasicLinkedString(s, n));
		}
		BasicLinkedString& insert(size_t pos, size_t n, char_t c) {
			return insert(pos, BasicLinkedString(n, c));
		}

		inline size_t length() const {
			return length_v;
		}

		void pop_back() {
			Node* new_last = last_addr->prv;
			delete last_addr;
			last_addr = new_last;
		}
		
		void push_back(char_t c) {
			if (!last_addr)
				last_addr = base_addr = new Node(c, nullptr, nullptr);
			else if (base_addr && last_addr)
				last_addr = last_addr->ptr = new Node(c, nullptr, last_addr);
			length_v++;
		}

		BasicLinkedString& replace(size_t pos, size_t len, const BasicLinkedString& str) {
			if (len > str.size())
				len = str.size();
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && addr; addr = addr->ptr, i++, j++)
				operator[](i) = str[j];
			return (*this);
		}
		BasicLinkedString& replace(size_t pos, size_t len, const BasicLinkedString& str, size_t subpos, size_t sublen = string::npos) {
			if (len > str.size())
				len = str.size();
			if (sublen == string::npos)
				sublen = str.size();
			Node* addr = base_addr;
			for (size_t i = pos, j = subpos; j < len && j < sublen && addr; addr = addr->ptr, i++, j++)
				operator[](i) = str[j];
			return (*this);
		}
		BasicLinkedString& replace(size_t pos, size_t len, const char_t* s) {
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && s[j] != '\0' && addr; addr = addr->ptr, i++, j++)
				operator[](i) = *(s + j);
			return (*this);
		}
		BasicLinkedString& replace(size_t pos, size_t len, const char_t* s, size_t n) {
			if (len > n)
				len = n;
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && addr; addr = addr->ptr, i++, j++)
				operator[](i) = *(s + j);
			return (*this);
		}
		BasicLinkedString& replace(size_t pos, size_t len, size_t n, char_t c) {
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && j < n && addr; addr = addr->ptr, i++, j++)
				operator[](i) = c;
			return (*this);
		}

		__pragma(warning(disable:6011)); // Dereferencing NULL pointer 'to_del'.
		void resize(size_t n) noexcept {
			if (n >= length_v)
				return;
			else if (!n) {
				this->~BasicLinkedString();
				return;
			}
			Node*  addr = base_addr;
			Node** to_del = (Node**)calloc(length_v - n, sizeof(Node*));
			for (size_t i = 0, j = -1; addr; addr = addr->ptr, i++) {
				if (i + 1 == n)
					addr->ptr = nullptr;
				if (n < i)
					to_del[++j] = addr;
			}
			for (size_t i = 0; i < size() - n; i++) {
				delete to_del[i];
			}
		}
		void resize(size_t n, char_t c) noexcept {
			if (n >= length_v)
				for (size_t i = 0; i < length_v - n; i++)
					push_back(c);
			else if (!n) {
				this->~BasicLinkedString();
				return;
			}
			Node* addr = base_addr;
			Node** to_del = (Node**)calloc(length_v - n, sizeof(Node*));
			for (size_t i = 0, j = -1; addr; addr = addr->ptr, i++) {
				if (i + 1 == n)
					addr->ptr = nullptr;
				if (n < i)
					to_del[++j] = addr;
			}
			for (size_t i = 0; i < size() - n; i++) {
				delete to_del[i];
			}
		}

		void reverse() noexcept {
			if (!base_addr)
				return;
			Node* prev = nullptr;
			Node* addr = base_addr;
			Node* next = base_addr->ptr;
			for (size_t i = 0; addr; i++, addr = next) {
				next = addr->ptr;
				addr->ptr = prev;
				prev = addr;
			}
			base_addr = prev;
		}
		BasicLinkedString reverse() const noexcept {
			if (!base_addr)
				return *this;
			BasicLinkedString str = *this;
			Node* prev = nullptr;
			Node* addr = str.base_addr;
			Node* next = str.base_addr->ptr;
			for (size_t i = 0; addr; i++, addr = next) {
				next = addr->ptr;
				addr->ptr = prev;
				prev = addr;
			}
			str.base_addr = prev;
			return str;
		}
		
		size_t rfind(const BasicLinkedString& str, size_t pos = string::npos) const noexcept {
			pos = ((pos == string::npos) ? length_v : pos);
			size_t length = str.size();
			bool* matchs = new bool[length];
			for (size_t i = 0; i < length; i++)
				matchs[i] = false;
			size_t start_index = string::npos;
			Node* addr = last_addr;
			for (size_t i = pos - 1; addr; addr = addr->prv, i--) {
				auto j = string::npos;
				for (size_t k = length - 1; k >= 0; (k != 0) ? k-- : k = -1) {
					if (k == string::npos || *(matchs + k)) {
						j = k;
						break;
					}
				}
				bool all_true = true;
				for (size_t k = 0; k < length; k++) {
					if (!matchs[k]) {
						all_true = false;
						break;
					}
				}
				if (j != string::npos && !all_true) {
					if (operator[](i) == str[length - 1 - (j + 1)])
						matchs[j + 1] = true;
					else {
						for (size_t i = 0; i < length; i++) {
							matchs[i] = false;
						}
					}
				}
				else if (all_true)
					break;
				else {
					if (operator[](i) == str[length - 1])
						matchs[0] = true;
					start_index = i;
				}
			}
			delete[] matchs;
			return start_index - (length - 1);
		}
		size_t rfind(const char_t* s, size_t pos = string::npos) const {
			return this->rfind(static_cast<BasicLinkedString>(s), pos);
		}
		size_t rfind(const char_t* s, size_t pos, size_t n) const {
			return this->rfind(BasicLinkedString(s, n), pos);
		}
		size_t rfind(char c, size_t pos = string::npos) const noexcept {
			pos = ((pos > length_v) ? length_v - 1 : pos);
			Node* addr = last_addr;
			for (size_t i = pos; addr && i < length_v; addr = addr->prv, i--)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		inline size_t size() const noexcept {
			return length_v;
		}

		void substr(size_t pos = 0, size_t len = string::npos) {
			if (pos > length_v)
				throw string::out_of_range();
			len = ((len == string::npos) ? length_v : len);
			Node*  addr = base_addr;
			Node*  last = nullptr;
			Node** to_del = new Node*[length_v - len - pos + 1];
			for (size_t i = 0, j = 0; addr; addr = addr->ptr, i++) {
				if (i == pos)
					base_addr = addr;
				else if (i == len + pos)
					last = addr;
				if (i < pos || i >= len + pos) {
					to_del[j] = addr;
					j++;
				}
			}
			last->prv->ptr = nullptr;
			for (size_t i = 0; i < length_v - len - pos - 1; i++)
				delete to_del[i];
			length_v = len;
		}
		BasicLinkedString substr(size_t pos = 0, size_t len = string::npos) const {
			return BasicLinkedString(*this, pos, len);
		}

		void swap(BasicLinkedString& str) {
			BasicLinkedString copy = str;
			str.assign(*this);
			operator = (copy);
		}

		char_t& operator [] (size_t pos) {
			if (pos >= size())
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = 0; i != pos; i++, addr = addr->ptr);
			return addr->data;
		}
		const char_t& operator [] (size_t pos) const {
			if (pos >= size())
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = 0; i != pos; i++, addr = addr->ptr);
			return addr->data;
		}

		BasicLinkedString& operator += (const BasicLinkedString& str) {
			for (size_t i = 0; i < str.size(); i++)
				push_back(str[i]);
			return (*this);
		}
		BasicLinkedString& operator += (const char_t* s) {
			size_t i = 0;
			for (char_t c = *s; c != '\0'; i++, c = *(s + i))
				push_back(c);
			return (*this);
		}
		BasicLinkedString& operator += (char_t c) {
			push_back(c);
			return (*this);
		}
		BasicLinkedString& operator += (std::initializer_list<char_t> il) {
			for (char_t const& c : il)
				push_back(c);
			return (*this);
		}

		BasicLinkedString& operator = (const BasicLinkedString& str) {
			this->~BasicLinkedString();
			for (size_t i = 0; i < str.size(); i++) push_back(str[i]);
			return (*this);
		}
		BasicLinkedString& operator = (const char_t* s) {
			this->~BasicLinkedString();
			size_t i = 0;
			for (char_t c = *s; c != '\0'; i++, c = *(s + i)) push_back(c);
			return (*this);
		}
		BasicLinkedString& operator = (char_t c) {
			this->~BasicLinkedString();
			base_addr = new Node(c, nullptr, nullptr);
			return (*this);
		}
		BasicLinkedString& operator = (std::initializer_list<char_t> il) {
			this->~BasicLinkedString();
			for (char_t const& c : il) push_back(c);
			return (*this);
		}

		friend auto operator << (std::ostream& os, BasicLinkedString const& obj) -> std::ostream& {
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr)
				os << addr->data;
			return os;
		}

		friend auto operator >> (std::istream& is, BasicLinkedString& obj) -> std::istream& {
			obj.~BasicLinkedString();
			for (char_t c = '\0'; is.get(c) && c != '\n'; )
				obj.push_back(c);
			return is;
		}

		friend BasicLinkedString operator + (const BasicLinkedString& lhs, const BasicLinkedString& rhs) {
			BasicLinkedString new_str{ lhs };
			for (size_t i = 0; i < rhs.length_v; i++)
				new_str.push_back(rhs[i]);
			return new_str;
		}
		friend BasicLinkedString operator + (const BasicLinkedString& rhs, const char_t* lhs) {
			return operator + (rhs, static_cast<BasicLinkedString>(lhs));
		}
		friend BasicLinkedString operator + (const char_t* rhs, const BasicLinkedString& lhs) {
			return operator + (lhs, static_cast<BasicLinkedString>(rhs));
		}
		friend BasicLinkedString operator + (const BasicLinkedString& rhs, char_t lhs) {
			return operator + (rhs, BasicLinkedString(1, lhs));
		}
		friend BasicLinkedString operator + (char_t rhs, const BasicLinkedString& lhs) {
			return operator + (lhs, BasicLinkedString(1, rhs));
		}

		friend bool operator == (const BasicLinkedString& lhs, const BasicLinkedString& rhs) noexcept {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] != rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator == (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator == (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator == (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator == (static_cast<BasicLinkedString>(rhs), lhs);
		}

		friend bool operator != (const BasicLinkedString& lhs, const BasicLinkedString& rhs) noexcept {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] == rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator != (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator != (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator != (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator != (static_cast<BasicLinkedString>(rhs), lhs);
		}

		friend bool operator <= (const BasicLinkedString& lhs, const BasicLinkedString& rhs) {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] > rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator <= (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator <= (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator <= (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator <= (static_cast<BasicLinkedString>(rhs), lhs);
		}

		friend bool operator <  (const BasicLinkedString& lhs, const BasicLinkedString& rhs) {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] >= rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator <  (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator < (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator <  (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator < (static_cast<BasicLinkedString>(rhs), lhs);
		}

		friend bool operator >= (const BasicLinkedString& lhs, const BasicLinkedString& rhs) {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] < rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator >= (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator >= (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator >= (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator >= (static_cast<BasicLinkedString>(rhs), lhs);
		}
		
		friend bool operator >  (const BasicLinkedString& lhs, const BasicLinkedString& rhs) {
			for (size_t i = 0; i < lhs.length_v; i++) {
				if (lhs[i] <= rhs[i]) {
					return false;
				}
			}
			return true;
		}
		friend bool operator >  (const char_t* lhs, const BasicLinkedString& rhs) {
			return operator > (static_cast<BasicLinkedString>(lhs), rhs);
		}
		friend bool operator >  (const BasicLinkedString& lhs, const char_t* rhs) {
			return operator > (static_cast<BasicLinkedString>(rhs), lhs);
		}

		inline operator std::string () {
			std::string str;
			for (size_t i = 0; i < length_v; i++)
				str.push_back(operator[](i));
			return str;
		}
	};

	typedef BasicLinkedString<char>     LinkedString;
	typedef BasicLinkedString<char16_t> u16LinkedString;
	typedef BasicLinkedString<char32_t> u32LinkedString;
	typedef BasicLinkedString<wchar_t>  wLinkedString;
}

using namespace string;

namespace std {
	__pragma(warning(disable:6001)); // Using uninitialized memory '*s' (it's literally initialized lol what)
	double stod(const LinkedString& str, size_t* idx = nullptr) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); it++, i++)
			s[i] = *it;
		double result = strtod(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	double stod(const wLinkedString& str, size_t* idx = nullptr) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); it++, i++)
			s[i] = *it;
		double result = wcstod(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	float stof(const LinkedString& str, size_t* idx = nullptr) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		float result = strtof(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	float stof(const wLinkedString& str, size_t* idx = nullptr) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		float result = wcstof(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	int stoi(const LinkedString& str, size_t* idx = nullptr, int base = 10) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		int result = strtol(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	int stoi(const wLinkedString& str, size_t* idx = nullptr, int base = 10) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		int result = wcstol(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	long stol(const LinkedString& str, size_t* idx = nullptr, int base = 10) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long result = strtol(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	long stol(const wLinkedString& str, size_t* idx = nullptr, int base = 10) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long result = wcstol(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	long double stold(const LinkedString& str, size_t* idx = nullptr) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long double result = strtold(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	long double stold(const wLinkedString& str, size_t* idx = nullptr) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long double result = wcstold(s, &p);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	long long stoll(const LinkedString& str, size_t* idx = nullptr, int base = 10) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long long result = strtoll(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	long long stoll(const wLinkedString& str, size_t* idx = nullptr, int base = 10) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		long long result = wcstoll(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	unsigned long stoul(const LinkedString& str, size_t* idx = nullptr, int base = 10) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		unsigned long result = strtoul(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	unsigned long stoul(const wLinkedString& str, size_t* idx = nullptr, int base = 10) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		unsigned long result = wcstoul(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}

	unsigned long long stoull(const LinkedString& str, size_t* idx = nullptr, int base = 10) {
		char* s = new char[str.size()];
		char* p = nullptr;
		size_t i = 0;
		for (LinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		unsigned long long result = strtoull(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	unsigned long long stoull(const wLinkedString& str, size_t* idx = nullptr, int base = 10) {
		wchar_t* s = new wchar_t[str.size()];
		wchar_t* p = nullptr;
		size_t i = 0;
		for (wLinkedString::Iterator it = str.begin(); it != str.end(); i++, i++)
			s[i] = *it;
		unsigned long long result = wcstoull(s, &p, base);
		if (idx) *idx = str.find(*p);
		delete[] s;
		return result;
	}
	__pragma(warning(enable:6001));

	template <typename type> requires std::integral<type> || std::floating_point<type>
	LinkedString to_string(type val) {
		std::stringstream ss;
		ss << val;
		return static_cast<LinkedString>(ss.str().c_str());
	}

	template <typename type> requires std::integral<type> || std::floating_point<type>
	wLinkedString to_wstring(type val) {
		std::stringstream ss;
		ss << val;
		return static_cast<wLinkedString>(ss.str().c_str());
	}

	void swap(LinkedString& x, LinkedString& y) {
		x.swap(y);
	}

	istream& getline(istream& is, LinkedString& str) {
		is >> str;
		return is;
	}
	istream& getline(istream& is, LinkedString& str, char delim) {
		str.~BasicLinkedString();
		for (char c = '\0'; is.get(c) && c != '\n' && c != delim; )
			str.push_back(c);
		return is;
	}
}