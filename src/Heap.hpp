#pragma once

#include <array>
#include <mutex>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

/**
 * @brief Fixed priority queue implemented on an array heap
 * @tparam N Fixed size
 * Every element added uses the update key algorithm instead of the traditional heap push
 * The heap can't pop elements because it has fixed size, use the consume function to iterate the elements in the sorted order
*/
template<class T, uint16_t N, class Comparer = std::greater<T>>
class Heap
{
protected:
	Comparer comp;
	uint16_t size;
	std::array<T, N> heap;

protected:
	static constexpr uint16_t left_child(uint16_t parent) {
		return parent * 2 + 1;
	}

	static constexpr uint16_t right_child(uint16_t parent) {
		return parent * 2 + 2;
	}

	constexpr bool valid(uint16_t child) {
		return child < N;
	}

public:
	Heap()
		: comp{},
		size{0}
	{}

private:
    // Private dispatcher
	template<class U>
	void push_internal(U&& value)
	{
		// Don't push new value if less than heap's top
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

		// Replace top with new value and swap further down the heap
		heap[0] = std::forward<U>(value);
		uint16_t index = 0;
		while (valid(index))
		{
			uint16_t min_child = left_child(index);
			if (!valid(min_child))
			{
				break;
			}

			uint16_t other_child = right_child(index);
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
	/**
	 * @brief Copy value to heap
	*/
	virtual void push(const T& value)
	{
		push_internal(value);
	}

	/**
	 * @brief Move value to heap
	*/
	virtual void push(T&& value)
	{
		push_internal(std::move(value));
	}

	/**
	 * @brief Return a sorted vector based on the underlying heap
	*/
	virtual std::vector<T> toSortedVector() const
	{
		std::vector<T> result(heap.begin(), heap.end());
		std::sort_heap(result.begin(), result.end(), comp);

		return result;
	}

	/**
	 * @brief Return the highest priority element (first element)
	*/
	virtual const T& top() const
	{
		return heap.front();
	}

	/**
	 * @brief Return the underlyng array heap
	 * @return 
	*/
	auto& getContainer()
	{
		return heap;
	}

	/**
	 * @brief Iterate the elements in the sorted order 
	 * @param consumer Function to apply to each element
	*/
	virtual void consume(std::function<void(const T&)> consumer)
	{
		std::sort_heap(heap.begin(), heap.end(), comp);
		for (uint16_t i = 0; i < N; i++)
		{
			consumer(heap[i]);
		}
	}
};

