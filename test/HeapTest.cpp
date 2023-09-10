//#include <gtest/gtest.h>
//#include <fstream>
//#include <algorithm>
//
//#include <Heap.hpp>
//#include <CoarseConcurrentHeap.hpp>
//#include <FineConcurrentHeap.hpp>
//
//static const uint32_t DATA_SIZE = 10'000;
//static const uint16_t HEAP_SIZE = 500;
//
//class HeapTest : public testing::Test
//{
//protected:
//	std::vector<int> data;
//
//	void SetUp()
//	{
//		data.resize(DATA_SIZE);
//
//		std::ifstream f("data.txt");
//		int i = 0;
//		while (i < DATA_SIZE && f >> data[i++]);
//
//		std::sort(data.begin(), data.end(), std::greater<int>());
//	}
//
//	void test(Heap<int, HEAP_SIZE>& heap)
//	{
//		auto& heap_internal = heap.getContainer();
//
//		EXPECT_TRUE(std::is_heap(heap_internal.begin(), heap_internal.end(), std::greater<int>()));
//
//		std::sort_heap(heap_internal.begin(), heap_internal.end(), std::greater<int>());
//
//		EXPECT_TRUE(std::equal(heap_internal.begin(), heap_internal.end(), data.begin(), data.begin() + HEAP_SIZE));
//	}
//};
//
//TEST_F(HeapTest, HeapTestSync)
//{
//	Heap<int, HEAP_SIZE> heap;
//	for (size_t i = 0; i < DATA_SIZE; i++)
//	{
//		heap.push(data[i]);
//	}
//
//	test(heap);
//}
//
//void addDataAsync(std::vector<int>& data, Heap<int, HEAP_SIZE>& heap, size_t num_threads)
//{
//	auto iterate = [&](int start, int end)
//	{
//		for (int i = start; i < end; i++)
//		{
//			heap.push(data[i]);
//		}
//	};
//
//	const size_t load = DATA_SIZE / num_threads;
//	std::vector<std::thread> threads;
//	for (size_t i = 0; i < num_threads; i++)
//	{
//		if (i < num_threads - 1)
//		{
//			threads.emplace_back(iterate, i * load, (i + 1) * load);
//		}
//		else
//		{
//			threads.emplace_back(iterate, i * load, DATA_SIZE);
//		}
//	}
//
//	for (int i = 0; i < num_threads; i++)
//	{
//		threads[i].join();
//	}
//}
//
//TEST_F(HeapTest, HeapTestAsyncCoarseGrained)
//{
//	CoarseConcurrentHeap<int, HEAP_SIZE> heap;
//	
//	addDataAsync(data, heap, 4);
//
//	test(heap);
//}
//
//TEST_F(HeapTest, HeapTestAsyncFineGrained)
//{
//	FineConcurrentHeap<int, HEAP_SIZE> heap;
//
//	addDataAsync(data, heap, 4);
//
//	test(heap);
//}