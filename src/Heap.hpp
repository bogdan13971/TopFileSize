#pragma once

#include <array>
#include <mutex>
#include <memory>
#include <algorithm>

template<class T, size_t N, class Comparer = std::greater<T>>
class Heap
{
protected:
	Comparer comp;
	size_t size;
	std::array<T, N> heap;

protected:
	static constexpr size_t left_child(size_t parent) {
		return parent * 2 + 1;
	}

	static constexpr size_t right_child(size_t parent) {
		return parent * 2 + 2;
	}

	constexpr bool valid(size_t child) {
		return child < N;
	}

public:
	Heap()
		: Heap(std::greater<T>())
	{}

	Heap(const Comparer& comp) 
		: comp{comp},
		size{0}
	{}

private:
	template<class U>
	void push_internal(U&& value)
	{
		if (size < N)
		{
			heap[size] = std::forward<U>(value);
			size++;

			if (size == N)
			{
				std::make_heap(heap.begin(), heap.end(), comp);
			}

			return;
		}

		if (!comp(value, heap.front()))
		{
			return;
		}

		// log h
		heap[0] = std::forward<U>(value);
		size_t index = 0;
		while (valid(index))
		{
			size_t min_child = left_child(index);
			if (!valid(min_child))
			{
				break;
			}

			size_t other_child = right_child(index);
			if (valid(other_child) && comp(heap[min_child], heap[other_child]))
			{
				min_child = other_child;
			}

			if (heap[index] < heap[min_child])
			{
				break;
			}

			std::swap(heap[index], heap[min_child]);
			index = min_child;
		}
	}

public:
	virtual void push(const T& value)
	{
		push_internal(value);
	}

	virtual void push(T&& value)
	{
		push_internal(std::move(value));
	}

	virtual std::vector<T> toSortedVector() const
	{
		std::vector<T> result(heap.begin(), heap.end());
		std::sort(result.begin(), result.end(), comp);

		return result;
	}

	virtual const T& top() const
	{
		return heap.front();
	}

	auto& getContainer()
	{
		return heap;
	}
};

