#pragma once

#include "Heap.hpp"

/**
 * @brief Fixed concurrent priority queue
 * The update key algorithm is thread-safe by using a lock for each node of the heap
*/
template<class T, size_t N, class Comparer = std::greater<T>>
class FineConcurrentHeap : public Heap<T, N, Comparer>
{
private:
	using UniqueLock = std::unique_lock<std::mutex>;

private:
	mutable std::mutex mtx;
	mutable std::array<std::unique_ptr<std::mutex>, N> locks;

public:
	FineConcurrentHeap()
		: Heap<T, N, Comparer>{}
	{
		for (size_t i = 0; i < N; i++)
		{
			locks[i] = std::make_unique<std::mutex>();
		}

		// Fill underlying array heap to simplify locking for push later
		std::fill(this->heap.begin(), this->heap.end(), std::numeric_limits<T>::min());
	}

	/**
	 * @brief Copy value to heap
	 * Thread-safe
	*/
	void push(const T& value) override
	{
		push_internal(value);
	}

	/**
	 * @brief Move value to heap
	 * Thread-safe
	*/
	void push(T&& value) override
	{
		push_internal(std::move(value));
	}

private:
	// Private dispatcher
	template<class U>
	void push_internal(U&& value)
	{
		// Use size = N from the start to remove extra locking
		//UniqueLock lock(mtx);

		//if (this->size < N)
		//{
		//	this->heap[this->size] = std::forward<U>(value);
		//	this->size++;

		//	if (this->size == N)
		//	{
		//		std::make_heap(this->heap.begin(), this->heap.end(), this->comp);
		//	}

		//	return;
		//}
		//lock.unlock();

		// Same update key algorithm but locking the parent and it's children
		UniqueLock lock_parent(*(locks[0]));
		if (!this->comp(value, this->heap.front()))
		{
			return;
		}

		this->heap[0] = std::forward<U>(value);
		size_t index = 0;
		while (this->valid(index))
		{
			size_t min_child = Heap<T, N, Comparer>::left_child(index);
			if (!this->valid(min_child))
			{
				break;
			}
			UniqueLock lock_min_child(*(locks[min_child]));

			size_t other_child = Heap<T, N, Comparer>::right_child(index);
			if (this->valid(other_child))
			{
				UniqueLock lock_other_child(*(locks[other_child]));

				if (this->comp(this->heap[min_child], this->heap[other_child]))
				{
					min_child = other_child;
					lock_min_child.swap(lock_other_child);
				}

				lock_other_child.unlock();
			}

			if (this->heap[index] < this->heap[min_child])
			{
				break;
			}

			std::swap(this->heap[index], this->heap[min_child]);
			lock_parent.swap(lock_min_child);
			lock_min_child.unlock();

			index = min_child;
		}
	}

public:
	/**
	 * @brief Return a sorted vector based on the underlying heap
	 * Thread-safe
	*/
	std::vector<T> toSortedVector() const override
	{
		for (auto& lock : locks)
		{
			lock->lock();
		}

		auto result = Heap<T, N, Comparer>::toSortedVector();

		for (auto& lock : locks)
		{
			lock->unlock();
		}

		return result;
	}
};