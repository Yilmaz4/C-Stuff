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
#include <iomanip>
#include <exception>
#include <sstream>

namespace matrix {
	template <typename type> class Matrix {
		struct Proxy {
			Proxy(type* base) : base(base) {}
			operator int* __ptr64 () const noexcept {
				return base;
			}
			operator type& () const {
				return *base;
			}
		private:
			type* base = nullptr;
		};
		type* base = nullptr;
		size_t h = 0ull;
		size_t w = 0ull;
	public:
		Matrix(void) = default;
		Matrix(const size_t h, const size_t w) : h(h), w(w) {
			base = static_cast<type*>(malloc(h * w * sizeof type));
		}
		Matrix(const size_t h, const size_t w, const type& fill) : h(h), w(w) {
			base = static_cast<type*>(malloc(h * w * sizeof type));
			for (size_t i = 0; i < h; i++) {
				for (size_t j = 0; h < w; j++) {
					*(base + (i * w) * j) = fill;
				}
			}
		}
		Matrix(std::initializer_list<std::initializer_list<type>> const& matrix) {
			h = matrix.size();
			w = matrix.begin()->size();
			base = static_cast<type*>(malloc(h * w * sizeof type)); // requires assignment operator to not be overloaded
			// base = new type[h * w]; // requires a default constructor
			for (size_t i = 0; i < h; i++) {
				for (size_t j = 0; j < w; j++) {
					*(base + (i * w) + j) = *(reinterpret_cast<const type*>((*(matrix.begin() + i)).begin() + j));
				}
			}
		}
		~Matrix() {
			free(static_cast<void*>(base));
			base = nullptr;
		}
		type& at(size_t x, size_t y) const {
			return *(base + (x * w) + y);
		}
		size_t size() const noexcept {
			return h * w;
		}
		size_t height() const noexcept {
			return h;
		}
		size_t width() const noexcept {
			return w;
		}
		Proxy operator [] (size_t index) const {
			return Proxy(index * w + base);
		}
		friend auto operator << (std::ostream& os, Matrix const& obj) -> std::ostream& {
			std::streamsize max = 0;
			for (size_t i = 0; i < obj.h; i++) {
				for (size_t j = 0; j < obj.w; j++) {
					std::stringstream ss; 
					ss << obj.at(i, j);
					std::streamsize res = ss.str().size();
					max = ((res > max) ? res : max);
				}
			}
			os << std::string(max + 3, ' ');
			for (size_t i = 0; i < obj.w; i++)
				os << i << std::setw(max + 2);
			os << std::endl << std::endl;
			for (size_t i = 0; i < obj.h; i++) {
				os << std::setw(0) << i << std::setw(max + 3);
				for (size_t j = 0; j < obj.w; j++)
					os << obj.at(i, j) << ((j + 1 != obj.w) ? ',' : '\0') << std::setw(max + 1);
				os << std::endl;
			}
			return os;
		}
	};
}