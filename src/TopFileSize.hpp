#pragma once

#include <string>
#include "Heap.hpp"

namespace boost
{
namespace filesystem
{
	class path;
}
}

/**
 * @brief Sorts all the files inside a directory (recursively) in descending order based on file size
*/
class TopFileSize
{
private:
	static constexpr uint16_t TOP_SIZE = 10;

	using SizeNamePair = std::pair<size_t, std::string>;

	struct FileComparer
	{
		bool operator()(const SizeNamePair&, const SizeNamePair&);
	};

	using HeapType = Heap<SizeNamePair, TOP_SIZE, FileComparer>;

	struct FileConsumer
	{
		FileConsumer(HeapType& heap);

		void operator()(const boost::filesystem::path& path);
		
	private:
		HeapType& heap;
	};

private:
	const std::string root;

	static void printTop(HeapType& heap);

public:
	TopFileSize(const std::string& root);

	/**
	 * @brief Convert raw file size to human readable form
	*/
	static std::string getRedableSize(size_t sizeInBytes);

	/**
	 * @brief Print all files in root directory sorted by file size
	 * Directory traversal is done on a single thread and each file is added into a fixed min-heap.
	*/
	void printTopFilesSync();

	/**
	 * @brief Print all files in root directory sorted by file size
	 * @fineGrained Use the fine grained locked concurrent heap
	 * Directory traversal is done on multiple threads and each thread adds the file to a concurrent min-heap.
	*/
	void printTopFilesAsync(uint16_t num_threads, bool fineGrained = false);

	/**
	 * @brief Print all files in root directory sorted by file size
	 * Directory traversal is done on multiple threads but a single thread adds each file to a fixed min-heap.
	 * This is the fastest algorithm so far.
	*/
	void printTopFilesAsync_v2(uint16_t num_threads);
};