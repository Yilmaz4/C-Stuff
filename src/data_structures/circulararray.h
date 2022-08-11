#pragma once

#include <iostream>

namespace circulararray {
	typedef size_t index_t;

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
				(*this)[i] = *((type*)list.begin() + i);
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