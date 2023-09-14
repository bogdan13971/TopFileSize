#pragma once

#include "Heap.hpp"

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

		std::fill(this->heap.begin(), this->heap.end(), std::numeric_limits<T>::min());
	}

	void push(const T& value) override
	{
		push_internal(value);
	}

	void push(T&& value) override
	{
		push_internal(std::move(value));
	}

private:
	template<class U>
	void push_internal(U&& value)
	{
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

		UniqueLock lock_parent(*(locks[0]));
		if (!this->comp(value, this->heap.front()))
		{
			return;
		}

		// log h
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