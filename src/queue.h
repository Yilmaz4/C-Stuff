#pragma once

template <typename type> class Queue {
	void* base_addr = nullptr;
	void* last_addr = nullptr;
	size_t length = 0;
protected:
	struct Node {
		Node(type data, Node* ptr) {
			this->data = (type*)malloc(sizeof(data));
			*this->data = data;
			this->ptr = ptr;
		}
		~Node() {
			free((void*)this->data);
		}
		type* data;
		Node* ptr;
	};
public:
	struct queue_empty_error : public std::exception {
		virtual const char* what() const noexcept {
			return "Queue is empty";
		}
	};
	Queue(void) = default;

	inline void enqueue(type obj) {
		base_addr = (void*)new Node(obj, (Node*)base_addr);
		length++;
	}
	inline type dequeue() {
		if (empty()) throw queue_empty_error();

	}
	inline bool empty() {
		return (!length);
	}
	inline size_t size() {
		return length;
	}
};