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
#include <algorithm>
#include <cmath>

#include "queue.h"

namespace tree {
	typedef size_t index_t;

	struct not_found_error : public std::exception {
		virtual const char* what() const noexcept {
			return "Search returned no results";
		}
	};
	struct tree_empty_error : public std::exception {
		virtual const char* what() const noexcept {
			return "Tree is empty";
		}
	};
	struct node_cannot_be_deleted : public std::exception {
		virtual const char* what() const noexcept {
			return "The node with given value cannot be deleted from the tree.";
		}
	};

	template <typename type>
	concept supports_comparison = requires(type obj1, type obj2) {
		{obj1 > obj2} noexcept -> std::convertible_to<bool>;
		{obj1 < obj2} noexcept -> std::convertible_to<bool>;
	};
	template <typename type> requires tree::supports_comparison<type>
	class BinarySearchTree final {
		struct Node {
			Node(type const& data_r, Node* left_p, Node* right_p, Node* parent_p) {
				if (data) *data = data_r;
				parent = parent_p;
				left = left_p;
				right = right_p;
			}
			Node(void) {
				this->~Node();
				data = nullptr;
			}
			~Node() {
				free(static_cast<void*>(this->data));
			}
			// using malloc here because we will need to copy a local variable to
			// heap and I am not aware of any way of doing this with new operator
			type* data = static_cast<type*>(malloc(sizeof(type)));
			Node* parent = nullptr;
			Node* left   = nullptr;
			Node* right  = nullptr;
		};

		Node* root = nullptr;

		Node* find(Node* root, type const& obj) const {
			while (root && root->data && *root->data != obj)
				root = ((obj > *root->data) ? root->right : root->left);
			if (!root)
				throw tree::not_found_error();
			return root;
		}
		static type& max_in_subtree(const Node* subtree) {
			if (!subtree)
				throw tree_empty_error();
			for (const Node* root = subtree; true;) {
				if (!root->right)
					return *root->data;
				root = root->right;
			}
		}
		static type& min_in_subtree(const Node* subtree) {
			if (!subtree)
				throw tree_empty_error();
			for (const Node* root = subtree; true;) {
				if (!root->left)
					return *root->data;
				root = root->left;
			}
		}
	public:
		BinarySearchTree(void) = default;
		BinarySearchTree(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				insert(obj);
			}
		}
		void insert(type const& obj) {
			Node* curr = this->root, *prev = nullptr;
			while (curr) {
				prev = curr;
				curr = (obj > *curr->data) ? curr->right : curr->left;
			}
			Node* node = new Node(obj, nullptr, nullptr, prev);
			if (!prev) {
				this->root = node;
				return;
			}
			((obj > *prev->data) ? prev->right : prev->left) = node;
		}
		void remove(type const& obj) {
			Node* node = find(this->root, obj);
			if (!node->right && !node->left) {
				((obj > *node->parent->data) ?
					node->parent->right :
					node->parent->left) = nullptr;
				delete node;
			}
			else if (!node->right || !node->left) {
				Node* &link = ((obj > *node->parent->data) ?
					node->parent->right :
					node->parent->left);
				Node* child = (node->right) ? node->right : node->left;
				link = child;
				child->parent = node->parent;
				delete node;
			}
			else {
				Node* temp;
				free(static_cast<void*>(node->data));
				if (!node->right->left) {
					*node->data = min_in_subtree(node->right);
					temp = node->right;
					node->right = node->right->right;
				}
				else if (node->right->left && !node->left->right) {
					*node->data = max_in_subtree(node->left);
					temp = node->left;
					node->left = node->left->left;
				}
				else {
					throw tree::node_cannot_be_deleted();
				}
				delete temp;
			}
		}
		long depth_of(type const& obj) const {
			Node* node = this->root;
			long i = 0;
			for (; node && *node->data != obj; i++)
				node = ((obj > *node->data) ? node->right : node->left);
			if (!node && this->root)
				throw tree::not_found_error();
			else if (!this->root)
				throw tree::tree_empty_error();
			return i;
		}
		long height_of(type const& obj) const {
			queue::LinkedQueue<Node*> q{ find(this->root, obj) };
			long height = 0;
			for (size_t size = 1; !q.empty(); height++, size = q.size()) {
				for (unsigned __int64 i = 0; i < size; i++) {
					Node* curr = q.front();
					q.pop();
					if (curr->left)
						q.push(curr->left);
					if (curr->right)
						q.push(curr->right);
				}
			}
			return height - 1;
		}
		long height() const noexcept {
			if (!this->root)
				return -1;
			return height_of(*this->root->data);
		}
		bool has(type const& obj) const {
			return find(this->root, obj);
		}
		type& max() const {
			if (!this->root)
				throw tree_empty_error();
			for (Node* root = this->root; true;) {
				if (!root->right)
					return *root->data;
				root = root->right;
			}
		}
		type& min() const {
			if (!this->root)
				throw tree_empty_error();
			for (Node* root = this->root; true;) {
				if (!root->left)
					return *root->data;
				root = root->left;
			}
		}
		friend auto operator << (std::ostream& os, BinarySearchTree const& obj) -> std::ostream& {
			queue::LinkedQueue<Node*> q{ obj.root };
			unsigned __int64 i = 0;
			unsigned __int64 l = 1;
			for (size_t size = 1; !q.empty(); size = q.size()) {
				for (size_t j = 0; j < size; j++, i++) {
					Node* curr = q.front();
					if (curr)
						os << *curr->data;
					else
						os << "NULL";
					os << (i == log2(l) ? [](unsigned __int64* i, unsigned __int64* l) {
							*i = -1; (*l)++; return ';';
						} (&i, &l) : (!q.size() ? '\0' : ',')) << (char)0x20;
					q.pop();
					if (!curr) break;
					q.push((curr && curr->left)  ? curr->left  : nullptr);
					q.push((curr && curr->right) ? curr->right : nullptr);
				}
			}
			return os;
		}
	};
}