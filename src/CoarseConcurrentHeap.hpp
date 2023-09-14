#pragma once

#include "Heap.hpp"

/**
 * @brief Fixed concurrent priority queue
 * The update key algorithm is thread-safe by using a single lock for the entire heap
*/
template<class T, size_t N, class Comparer = std::greater<T>>
class CoarseConcurrentHeap : public Heap<T, N, Comparer>
{
private:
	using Lock = std::lock_guard<std::mutex>;

private:
	mutable std::mutex mtx;

public:
	CoarseConcurrentHeap()
		: Heap<T, N, Comparer>{}
	{}

	/**
	 * @brief Copy value to heap
	 * Thread-safe
	*/
	void push(const T& value) override
	{
		Lock lock(mtx);
		Heap<T, N, Comparer>::push(value);
	}

	/**
	 * @brief Move value to heap
	 * Thread-safe
	*/
	void push(T&& value) override
	{
		Lock lock(mtx);
		Heap<T, N, Comparer>::push(std::move(value));
	}

	/**
	 * @brief Return a sorted vector based on the underlying heap
	 * Thread-safe
	*/
	std::vector<T> toSortedVector() const override
	{
		Lock lock(mtx);
		return Heap<T, N, Comparer>::toSortedVector();
	}
};