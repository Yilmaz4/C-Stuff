#pragma once

#include <iostream>
#include <exception>
#include <algorithm>

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

		inline Node* find(Node* root, type const& obj, bool check = true) const {
			while (root && root->data && *root->data != obj)
				root = ((obj > *root->data) ? root->right : root->left);
			if (check) {
				if (!root && this->root)
					throw tree::not_found_error();
				else if (!this->root)
					throw tree::tree_empty_error();
			}
			return root;
		}
		static inline type& max_in_subtree(const Node* subtree) {
			if (!subtree)
				throw tree_empty_error();
			for (const Node* root = subtree; true;) {
				if (!root->right)
					return *root->data;
				root = root->right;
			}
		}
		static inline type& min_in_subtree(const Node* subtree) {
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
		inline void insert(type const& obj) {
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
		inline void remove(type const& obj) {
			Node* node = find(this->root, obj, true);
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
				} else if (node->right->left && !node->left->right) {
					*node->data = max_in_subtree(node->left);
					temp = node->left;
					node->left = node->left->left;
				} else {
					throw tree::node_cannot_be_deleted();
				}
				delete temp;
			}
		}
		inline long depth_of(type const& obj) const {
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
		inline long height_of(type const& obj) const {
			queue::LinkedQueue<Node*> q{ find(this->root, obj, true) };
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
		inline long height() const noexcept {
			return height_of(*this->root->data);
		}
		inline bool has(type const& obj) const noexcept {
			try {
				return find(this->root, obj);
			}
			catch (...) {
				return false;
			}
			
		}
		inline type& max() const {
			if (!this->root)
				throw tree_empty_error();
			for (Node* root = this->root; true;) {
				if (!root->right)
					return *root->data;
				root = root->right;
			}
		}
		inline type& min() const {
			if (!this->root)
				throw tree_empty_error();
			for (Node* root = this->root; true;) {
				if (!root->left)
					return *root->data;
				root = root->left;
			}
		}
	};
}