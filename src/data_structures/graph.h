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

#include "linkedlist.h"

#define EDGE_LIST        0x0
#define ADJACENCY_MATRIX 0x1
#define ADJACENCY_LIST   0x2

using namespace linkedlist;

namespace graph {
	template<class T>
	using less_than_t = decltype(std::declval<T>() < std::declval<T>());

	template<class T>
	constexpr bool has_less_than = is_detected<less_than_t, T>::value;

	template <typename type> class Graph {
		struct Vertex {
			Vertex(type const& data_p, size_t index_p, size_t cost_p = 0) {
				*data = data_p;
				cost = cost_p;
				index = index_p;
			}
			type* data = static_cast<type*>(malloc(sizeof type));
			size_t degree = 0;
			size_t index;
			size_t cost = 0;
		};
		// adjacency list
		SinglyLinkedList<SinglyLinkedList<size_t>> edges;
		SinglyLinkedList<Vertex*> vertices;
	public:
		Graph(void) = default;
		void create_vertex(const type& data, const std::initializer_list<const Vertex*>& adj_vertices = {}, size_t cost = 0) {
			Vertex* vertex = new Vertex(data, vertices.size(), cost);
			vertices.push_back(vertex);
			if (adj_vertices.size()) {
				link(*vertex, adj_vertices);
			}
		}
		void link(const Vertex& vertex, const Vertex& target) {
			try {
				edges.at(vertex.index).push_back(target.index);
			}
			catch (index_error& exc) {
				edges.push_back(SinglyLinkedList{ target.index });
			}
		}
		void link(const Vertex& vertex, const std::initializer_list<const Vertex*>& targets) {
			for (size_t i = 0; i < targets.size(); i++) {
				try {
					edges.at(vertex.index).push_back(targets[i]->index);
				}
				catch (index_error& exc) {
					edges.push_back(SinglyLinkedList{ targets[i]->index });
				}
			}
		}
		const Vertex& get_vertex(const type& data) const {
			size_t begin = 0;
			size_t end = vertices.size();
			do {
				const Vertex& mid = vertices.operator[](floor((begin + end) / 2));
				if (*mid->data == data)
					return mid;
				else if (*mid->data > data)
					begin = mid;
				else if (*mid->data < data)
					end = mid;

			} while (1);
		}
	};
}