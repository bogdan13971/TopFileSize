#include "TopFileSize.hpp"

#include <iostream>
#include <memory>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include "FileIterator.hpp"
#include "FineConcurrentHeap.hpp"
#include "CoarseConcurrentHeap.hpp"

std::string TopFileSize::getRedableSize(size_t sizeInBytes)
{
	constexpr size_t _1GB = 1 << 30;
	constexpr size_t _1MB = 1 << 20;
	constexpr size_t _1KB = 1 << 10;

	boost::format format("%6.2f%s");

	if (sizeInBytes > _1GB)
	{
		return boost::str(format % double(sizeInBytes / _1GB) % " GB");
	}
	else if (sizeInBytes > _1MB)
	{
		return boost::str(format % double(sizeInBytes / _1MB) % " MB");
	}

	return boost::str(format % double(sizeInBytes / _1KB) % " KB");
}

bool TopFileSize::FileComparer::operator()(const SizeNamePair& p1, const SizeNamePair& p2)
{
	return p1.first > p2.first;
}

TopFileSize::FileConsumer::FileConsumer(HeapType& heap)
	: heap{ heap }
{}

void TopFileSize::FileConsumer::operator()(const boost::filesystem::path& path)
{
	if (boost::filesystem::is_regular_file(path))
	{
		const auto size = boost::filesystem::file_size(path);
		auto& name = path.string();// filename().string();
		heap.push(std::make_pair(size, std::move(name)));
	}
}

TopFileSize::TopFileSize(const std::string& root)
	: root{ root }
{}

void TopFileSize::printTopFilesSync()
{
	HeapType heap;
	FileConsumer consumer(heap);
	FileIterator fileIterator;

	fileIterator.sync_iteration(root, consumer);

	printTop(heap);
}

void TopFileSize::printTopFilesAsync(uint16_t num_threads, bool fineGrained)
{
	std::unique_ptr<HeapType> heap;
	if (fineGrained)
	{
		heap = std::make_unique<FineConcurrentHeap<SizeNamePair, TOP_SIZE, FileComparer>>();
	}
	else
	{
		heap = std::make_unique<CoarseConcurrentHeap<SizeNamePair, TOP_SIZE, FileComparer>>();
	}

	FileConsumer consumer(*heap);
	FileIterator fileIterator;

	fileIterator.async_iteration(root, num_threads, consumer);

	printTop(*heap);
}

void TopFileSize::printTopFilesAsync_v2(uint16_t num_threads)
{
	HeapType heap;
	FileConsumer consumer(heap);
	FileIterator fileIterator;

	std::vector<std::vector<SizeNamePair>> data(num_threads);

	auto thread_consumer = [&data](const auto& path, auto threadId)
	{
		if (boost::filesystem::is_regular_file(path))
		{
			const auto size = boost::filesystem::file_size(path);
			auto& name = path.string();// filename().string();
			data[threadId].push_back(std::make_pair(size, std::move(name)));
		}
	};

	fileIterator.async_iteration_v2(root, num_threads, thread_consumer);

	for (auto& row : data)
	{
		for (auto&& p : row)
		{
			heap.push(std::move(p));
		}
	}

	printTop(heap);
}

void TopFileSize::printTop(HeapType& heap)
{
	heap.consume([](const SizeNamePair& data) {
		std::cout << getRedableSize(data.first) << " " << data.second << "\n";
	});
}