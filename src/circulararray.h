#pragma once

template <typename type> class CircularArray {
	type base_addr[];
	size_t size = 0;
public:
	CircularArray(size_t size) : base_addr[size] {

	}
	type operator [] (int index) {
		return *(base_addr + (index % size))
	}
};