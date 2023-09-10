#pragma once

#include "Heap.hpp"

template<class T, size_t N, class Comparer = std::greater<T>>
class CoarseConcurrentHeap : public Heap<T, N, Comparer>
{
private:
	using Lock = std::lock_guard<std::mutex>;

private:
	mutable std::mutex mtx;

public:
	CoarseConcurrentHeap()
		: CoarseConcurrentHeap(std::greater<T>())
	{}

	CoarseConcurrentHeap(const Comparer& comp)
		: Heap<T, N, Comparer>(comp)
	{}

	void push(const T& value) override
	{
		Lock lock(mtx);
		Heap<T, N, Comparer>::push(value);
	}

	void push(T&& value) override
	{
		Lock lock(mtx);
		Heap<T, N, Comparer>::push(std::move(value));
	}

	std::vector<T> toSortedVector() const override
	{
		Lock lock(mtx);
		return Heap<T, N, Comparer>::toSortedVector();
	}
};