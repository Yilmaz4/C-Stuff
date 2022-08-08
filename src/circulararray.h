#pragma once

#include <iostream>

namespace circulararray {
	typedef size_t index_t;

	template <typename type> class CircularArray {
		type* base_addr;
		size_t length = 0;
	public:
		explicit CircularArray(const size_t size) : length(size) {
			base_addr = new type[size];
		}
		CircularArray(std::initializer_list<type> const& list) {
			length = list.size();
			base_addr = new type[length];
			for (index_t i = 0; i < length; i++)
				(*this)[i] = *((type*)list.begin() + i);
		}
		~CircularArray() {
			delete[] base_addr;
		}
		inline void print() const {
			std::cout << "[";
			for (index_t i = 0; i < length; i++)
				std::cout << (*this)[i] << ((i != length - 1) ? ", " : "");
			std::cout << "]";
		}
		inline size_t size() const noexcept {
			return length;
		}
		inline bool empty() const noexcept {
			return !length;
		}
		type& operator [] (index_t index) const noexcept {
			return *(base_addr + (index % length));
		}
		operator int* __ptr64() const noexcept {
			return base_addr;
		}
	};
}