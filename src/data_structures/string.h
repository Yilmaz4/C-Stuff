#pragma once

#include <iostream>
#include <exception>

namespace string {
	typedef size_t index_t;
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

	class LinkedString final {
		struct Node {
			Node(char data_p, Node* ptr_p) {
				data = data_p;
				ptr = ptr_p;
			}
			char data;
			Node* ptr = nullptr;
		};
		class Iterator {
			Node* addr = nullptr;
		public:
			Iterator(Node* ptr) : addr(ptr) {}

			char& operator * () const {
				return addr->data;
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
		size_t length_v = 0;
	public:
		LinkedString(void) = default;
		LinkedString(const LinkedString& str) {
			for (size_t i = 0; i < str.size(); i++) push_back(str[i]);
		}
		LinkedString(const LinkedString& str, size_t pos, size_t len = string::npos) {
			if (pos > str.size() || pos > len)
				throw string::out_of_range();
			for (size_t i = pos, j = 0; j < ((len == string::npos) ? length_v : len); i++, j++) {
				push_back(str[i]);
			}
		}
		LinkedString(const char* s) {
			size_t i = 0;
			for (char c = *s; c != '\0'; i++, c = *(s + i)) push_back(c);
		}
		LinkedString(const char* s, size_t n) {
			size_t i = 0;
			for (char c = *s; i < n; i++, c = *(s + 1)) push_back(c);
		}
		LinkedString(size_t n, char c) {
			for (size_t i = 0; i < n; i++) push_back(c);
		}
		LinkedString(std::initializer_list<char> il) {
			for (char const& c : il) push_back(c);
		}

		~LinkedString(void) {
			if (!base_addr)
				return;
			for (Node* addr = base_addr, *next = addr->ptr; next; addr = next, next = addr->ptr)
				delete addr;
			delete last_addr;
			last_addr = base_addr = nullptr;
		}

		LinkedString& append(const LinkedString& str) {
			return operator += (str);
		}
		LinkedString& append(const LinkedString& str, size_t subpos, size_t sublen = string::npos) {
			if (subpos > str.size() || subpos > sublen)
				throw string::out_of_range();
			for (size_t i = subpos; subpos < ((sublen == string::npos) ? str.size() : sublen); i++)
				push_back(str[i]);
			return (*this);
		}
		LinkedString& append(const char* s) {
			return operator += (s);
		};
		LinkedString& append(const char* s, size_t n) {
			size_t i = 0;
			for (char c = *s; i < n; i++, c = *(s + 1))
				push_back(c);
			return (*this);
		}
		LinkedString& append(size_t n, char c) {
			for (size_t i = 0; i < n; i++)
				push_back(c);
			return (*this);
		}
		LinkedString& append(std::initializer_list<char> il) {
			return operator += (il);
		}

		LinkedString& assign(const LinkedString& str) {
			return operator = (str);
		}
		LinkedString& assign(const LinkedString& str, size_t subpos, size_t sublen = string::npos) {
			this->~LinkedString();
			if (subpos > str.size() || subpos > sublen)
				throw string::out_of_range();
			for (size_t i = subpos; subpos < ((sublen == string::npos) ? str.size() : sublen); i++) {
				push_back(str[i]);
			}
		}
		LinkedString& assign(const char* s) {
			return operator = (s);
		}
		LinkedString& assign(const char* s, size_t n) {
			this->~LinkedString();
			size_t i = 0;
			for (char c = *s; i < n; i++, c = *(s + 1)) push_back(c);
		}
		LinkedString& assign(size_t n, char c) {
			this->~LinkedString();
			for (size_t i = 0; i < n; i++) push_back(c);
		}
		LinkedString& assign(std::initializer_list<char> il) {
			return operator = (il);
		}

		inline char& at(index_t pos) {
			return operator[](pos);
		}
		inline const char& at(index_t pos) const {
			return operator[](pos);
		}

		inline char& back() {
			return operator[](size() - 1);
		}
		inline const char& back() const {
			return operator[](size() - 1);
		}

		Iterator begin() {
			return Iterator(base_addr);
		}

		inline void clear() noexcept {
			this->~LinkedString();
			base_addr = nullptr;
		}

		__pragma(warning(disable:4715)) // 'string::LinkedString::copy': not all control paths return a value. (It'll eventually return in case of good input)
		size_t copy(char* s, size_t len, size_t pos = 0) const {
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

		LinkedString& erase(size_t pos = 0, size_t len = string::npos) {
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
		
		__pragma(warning(disable:6295)) // Ill-defined for-loop. Loop executes infinitely. (It does not)
		__pragma(warning(disable:6386)) // Buffer overrun while writing to 'matchs'. (There's no buffer overrun)
		size_t find(const LinkedString& str, size_t pos = 0) const noexcept {
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
					return string::npos;
			return start_index;
		}
		size_t find(const char* s, size_t pos = 0) const {
			size_t length = 0;
			for (size_t i = 0; *(s + i) != '\0'; i++)
				length = i + 1;
			length -= pos;
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
					if (operator[](i) == s[j + 1])
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
					if (operator[](i) == s[0])
						matchs[0] = true;
					start_index = i;
				}
			}
			for (size_t i = 0; i < length; i++)
				if (!matchs[i])
					return string::npos;
			return start_index;
		}
		size_t find(const char* s, size_t pos, size_t n) const {
			size_t length = n - pos;
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
					if (operator[](i) == s[j + 1])
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
					if (operator[](i) == s[0])
						matchs[0] = true;
					start_index = i;
				}
			}
			for (size_t i = 0; i < length; i++)
				if (!matchs[i])
					return string::npos;
			return start_index;
		}
		size_t find(char c, size_t pos = 0) const noexcept {
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		size_t find_first_not_of(const LinkedString& str, size_t pos = 0) const noexcept {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) != str[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(const char* s, size_t pos = 0) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_first_not_of(char c, size_t pos = 0) const noexcept {
			// Time complexity: O(n - pos)
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) != c)
					return i;
			return string::npos;
		}

		size_t find_first_of(const LinkedString& str, size_t pos = 0) const noexcept {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) == str[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(const char* s, size_t pos = 0) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(const char* s, size_t pos, size_t n) const {
			// Time complexity: O((n - pos) * n2) n2 = parameter "n"
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_first_of(char c, size_t pos = 0) const noexcept {
			// Time complexity: O(n - pos)
			// Space complexity: O(1)
			Node* addr = base_addr;
			for (size_t i = pos; addr; addr = addr->ptr, i++)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		size_t find_last_not_of(const LinkedString& str, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) != str[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(const char* s, size_t pos = string::npos) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) != s[j])
						return i;
			return string::npos;
		}
		size_t find_last_not_of(char c, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				if (operator[](i) != c)
					return i;
			return string::npos;
		}

		size_t find_last_of(const LinkedString& str, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < str.size(); j++)
					if (operator[](i) == str[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(const char* s, size_t pos = string::npos) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; *(s + j) != '\0'; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(const char* s, size_t pos, size_t n) const {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				for (size_t j = 0; j < n; j++)
					if (operator[](i) == s[j])
						return i;
			return string::npos;
		}
		size_t find_last_of(char c, size_t pos = string::npos) const noexcept {
			Node* addr = base_addr;
			for (size_t i = (pos == string::npos) ? length_v : pos; i >= 0; addr = addr->ptr, (i != 0) ? i-- : i = -1)
				if (operator[](i) == c)
					return i;
			return string::npos;
		}

		inline char& front() {
			if (!base_addr)
				throw string::empty_string_error();
			return base_addr->data;
		}
		inline const char& front() const {
			if (!base_addr)
				throw string::empty_string_error();
			return base_addr->data;
		}

		LinkedString& insert(size_t pos, const LinkedString& str);
		LinkedString& insert(size_t pos, const LinkedString& str, size_t subpos, size_t sublen = string::npos);
		LinkedString& insert(size_t pos, const char* s);
		LinkedString& insert(size_t pos, const char* s, size_t n);
		LinkedString& insert(size_t pos, size_t n, char c);

		inline size_t length() const {
			return length_v;
		}

		void pop_back() {

		}
		
		void push_back(char c) {
			Node* node = new Node(c, nullptr);
			if (!last_addr)
				last_addr = base_addr = node;
			else if (base_addr && last_addr)
				last_addr = last_addr->ptr = node;
			length_v++;
		}

		LinkedString& replace(size_t pos, size_t len, const LinkedString& str) {
			if (len > str.size())
				len = str.size();
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && addr; addr = addr->ptr, i++, j++)
				operator[](i) = str[j];
			return (*this);
		}
		LinkedString& replace(size_t pos, size_t len, const LinkedString& str, size_t subpos, size_t sublen = string::npos) {
			if (len > str.size())
				len = str.size();
			if (sublen == string::npos)
				sublen = str.size();
			Node* addr = base_addr;
			for (size_t i = pos, j = subpos; j < len && j < sublen && addr; addr = addr->ptr, i++, j++)
				operator[](i) = str[j];
			return (*this);
		}
		LinkedString& replace(size_t pos, size_t len, const char* s) {
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && s[j] != '\0' && addr; addr = addr->ptr, i++, j++)
				operator[](i) = *(s + j);
			return (*this);
		}
		LinkedString& replace(size_t pos, size_t len, const char* s, size_t n) {
			if (len > n)
				len = n;
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && addr; addr = addr->ptr, i++, j++)
				operator[](i) = *(s + j);
			return (*this);
		}
		LinkedString& replace(size_t pos, size_t len, size_t n, char c) {
			Node* addr = base_addr;
			for (size_t i = pos, j = 0; j < len && j < n && addr; addr = addr->ptr, i++, j++)
				operator[](i) = c;
			return (*this);
		}

		__pragma(warning(disable:6011)) // Dereferencing NULL pointer 'to_del'.
		void resize(size_t n) noexcept {
			if (n >= length_v)
				return;
			else if (!n) {
				this->~LinkedString();
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
		void resize(size_t n, char c) noexcept {
			if (n >= length_v)
				for (size_t i = 0; i < length_v - n; i++)
					push_back(c);
			else if (!n) {
				this->~LinkedString();
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

		inline size_t size() const noexcept {
			return length_v;
		}

		LinkedString substr(size_t pos = 0, size_t len = string::npos) const {
			return LinkedString(*this, pos, len);
		}

		void swap(LinkedString& str) {
			LinkedString copy = str;
			str.assign(*this);
			operator = (copy);
		}

		char& operator [] (size_t pos) {
			if (pos >= size())
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = 0; i != pos; i++, addr = addr->ptr);
			return addr->data;
		}
		const char& operator [] (size_t pos) const {
			if (pos >= size())
				throw string::out_of_range();
			Node* addr = base_addr;
			for (size_t i = 0; i != pos; i++, addr = addr->ptr);
			return addr->data;
		}

		LinkedString& operator += (const LinkedString& str) {
			for (size_t i = 0; i < str.size(); i++)
				push_back(str[i]);
			return (*this);
		}
		LinkedString& operator += (const char* s) {
			size_t i = 0;
			for (char c = *s; c != '\0'; i++, c = *(s + i))
				push_back(c);
			return (*this);
		}
		LinkedString& operator += (char c) {
			push_back(c);
			return (*this);
		}
		LinkedString& operator += (std::initializer_list<char> il) {
			for (char const& c : il)
				push_back(c);
			return (*this);
		}

		LinkedString& operator = (const LinkedString& str) {
			this->~LinkedString();
			for (size_t i = 0; i < str.size(); i++) push_back(str[i]);
			return (*this);
		}
		LinkedString& operator = (const char* s) {
			this->~LinkedString();
			size_t i = 0;
			for (char c = *s; c != '\0'; i++, c = *(s + i)) push_back(c);
			return (*this);
		}
		LinkedString& operator = (char c) {
			this->~LinkedString();
			base_addr = new Node(c, nullptr);
			return (*this);
		}
		LinkedString& operator = (std::initializer_list<char> il) {
			this->~LinkedString();
			for (char const& c : il) push_back(c);
			return (*this);
		}

		friend auto operator << (std::ostream& os, LinkedString& obj) -> std::ostream& {
			for (Node* addr = obj.base_addr; addr; addr = addr->ptr)
				std::cout << addr->data;
			return os;
		}

		friend auto operator >> (std::istream& is, LinkedString& obj) -> std::istream& {
			obj.~LinkedString();
			for (char c; is.get(c) && c != '\n'; )
				obj.push_back(c);
			return is;
		}

		friend LinkedString operator + (const LinkedString& lhs, const LinkedString& rhs) {
			LinkedString new_str{ lhs };
			for (size_t i = 0; i < rhs.length_v; i++)
				new_str.push_back(rhs[i]);
			return new_str;
		}
		friend LinkedString operator + (const LinkedString& rhs, const char* lhs) {
			return operator + (rhs, static_cast<LinkedString>(lhs));
		}
		friend LinkedString operator + (const char* rhs, const LinkedString& lhs) {
			return operator + (lhs, static_cast<LinkedString>(rhs));
		}
		friend LinkedString operator + (const LinkedString& rhs, char lhs) {
			return operator + (rhs, LinkedString(1, lhs));
		}
		friend LinkedString operator + (char rhs, const LinkedString& lhs) {
			return operator + (lhs, LinkedString(1, rhs));
		}

		inline operator std::string () {
			std::string str;
			for (size_t i = 0; i < length_v; i++)
				str.push_back(operator[](i));
			return str;
		}
	};
}