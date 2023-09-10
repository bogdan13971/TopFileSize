#include <FileIterator.hpp>
#include <Heap.hpp>
#include <iostream>
#include <CoarseConcurrentHeap.hpp>
#include <FineConcurrentHeap.hpp>
#include <boost/format.hpp>

std::string getRedableSize(size_t sizeInBytes)
{
	constexpr float _1GB = 1 << 30;
	constexpr float _1MB = 1 << 20;
	constexpr float _1KB = 1 << 10;

	boost::format format("%6.2f%s");

	if (sizeInBytes > _1GB)
	{
		return boost::str(format % (sizeInBytes / _1GB) % " GB");
	}
	else if (sizeInBytes > _1MB)
	{
		return boost::str(format % (sizeInBytes / _1MB) % " MB");
	}

	return boost::str(format % (sizeInBytes / _1KB) % " KB");
}

static const int HEAP_SIZE = 10;
using Data = std::pair<size_t, std::string>;

struct Comparer
{
	bool operator()(const Data& p1, const Data& p2)
	{
		return p1.first > p2.first;
	}
};

using HeapSimple = Heap<Data, HEAP_SIZE, Comparer>;
using CoarseHeapSimple = CoarseConcurrentHeap<Data, HEAP_SIZE, Comparer>;
using FineHeapSimple = FineConcurrentHeap<Data, HEAP_SIZE, Comparer>;

struct Consumer
{
	Consumer(HeapSimple& heap)
		: heap{ heap }
	{}

	void operator()(const boost::filesystem::path& path)
	{
		if (boost::filesystem::is_regular_file(path))
		{
			const auto size = boost::filesystem::file_size(path);
			const auto name = path.string();// filename().string();
			heap.push(std::make_pair(size, name));
		}
	}

	HeapSimple& heap;
};

struct Consumer_v2
{
	Consumer_v2(std::vector<Data>& data)
		: data{ data }
	{}

	void operator()(const boost::filesystem::path& path)
	{
		if (boost::filesystem::is_regular_file(path))
		{
			const auto size = boost::filesystem::file_size(path);
			const auto name = path.string();// filename().string();
			data.push_back(std::make_pair(size, name));
		}
	}

	std::vector<Data>& data;
};

void print(const HeapSimple& heap)
{
	auto sorted = heap.toSortedVector();
	system("cls");
	for (const auto& p : sorted)
	{
		std::cout << p.second << " " << getRedableSize(p.first) << "\n";
	}
}

struct Watcher
{
	Watcher(HeapSimple& heap)
		:heap{ heap },
		alive{ true }
	{}

	void operator()()
	{
		while (alive)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			print(heap);
		}
	}

	void stop()
	{
		alive = false;
	}

	bool alive;
	HeapSimple& heap;
};

void test_sync()
{
	HeapSimple heap{ Comparer() };
	Consumer consumer(heap);

	int count = 0;
	auto watcher = [&consumer, &count, &heap](const auto& path) {

		consumer(path);

		count++;
		if (count % 1500 == 0)
		{
			print(heap);
		}
	};

	FileIterator fileIterator("D:\\projects\\boost_1_82_0", watcher);

	fileIterator.sync_iteration();
}

void test_async_coarse(int num_threads)
{
	CoarseHeapSimple heap{ Comparer() };
	Consumer consumer(heap);

	Watcher watcher(heap);
	std::thread t(std::ref(watcher));

	FileIterator fileIterator("D:\\projects\\boost_1_82_0", consumer);

	fileIterator.async_iteration_v2(num_threads);

	watcher.stop();
	t.join();
}

void test_async_fine(int num_threads)
{
	FineHeapSimple heap{ Comparer() };
	Consumer consumer(heap);

	Watcher watcher(heap);
	std::thread t(std::ref(watcher));

	FileIterator fileIterator("D:\\projects\\boost_1_82_0", consumer);

	fileIterator.async_iteration_v2(num_threads);

	watcher.stop();
	t.join();
}

void test_async_iter_sync_process(int num_threads)
{
	HeapSimple heap{ Comparer() };

	std::vector<std::vector<Data>> data(num_threads);
	std::vector<Consumer_v2> consumers(num_threads);
	for(int i)
}

int main()
{
	//test_sync();

	//test_async_coarse(4);

	test_async_fine(4);

	return 0;
}