#pragma once

#include <iostream>
#include <exception>
#include <algorithm>

namespace tree {
	typedef size_t index_t;

	template <typename type>
	concept supports_comparison = requires(type obj1, type obj2) {
		{obj1 > obj2} noexcept -> std::convertible_to<bool>;
		{obj1 < obj2} noexcept -> std::convertible_to<bool>;
	};

	template <typename type> requires tree::supports_comparison<type>
	class BinarySearchTree {
		struct Node {
			Node(type const& data_r, Node* left_p, Node* right_p) {
				if (data)
					*data = data_r;
				left = left_p;
				right = right_p;
			}
			~Node() {
				free((void*)this->data);
			}
			// using malloc here because we will need to copy a local variable to
			// heap and I am not aware of any way of doing this with new operator
			type* data = static_cast<type*>(malloc(sizeof(type)));
			Node* left;
			Node* right;
		};

		Node* root = nullptr;
	protected:
		static type const& max(type const& a, type const& b) noexcept {
			return (a > b) ? a : b;
		}
		inline long height(Node* const& root) const noexcept {
			return (!root) ? -1 : max(height(root->left), height(root->right)) + 1;
		}
	public:
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
		BinarySearchTree(void) = default;
		BinarySearchTree(std::initializer_list<type> const& list) {
			for (auto const& obj : list) {
				insert(obj);
			}
		}
		inline void insert(type const& obj) {
			Node* node = new Node(obj, nullptr, nullptr);
			Node* curr = this->root, * prev = nullptr;
			while (curr) {
				prev = curr;
				curr = (obj > *curr->data) ? curr->right : curr->left;
			}
			if (!prev) {
				this->root = node;
				return;
			}
			((obj > *prev->data) ? prev->right : prev->left) = node;
		}
		inline long height() const noexcept {
			return height(root);
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
		inline bool has(type const& obj) const noexcept {
			Node* root = this->root;
			while (root && *root->data != obj) {
				if (obj > *root->data)
					root = root->right;
				else
					root = root->left;
			}
			return root;
		}
		inline bool is_bst() const noexcept {

		}
	};
}