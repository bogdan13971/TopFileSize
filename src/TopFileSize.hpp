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

	static std::string getRedableSize(size_t sizeInBytes);

	void printTopFilesSync();
	void printTopFilesAsync(uint16_t num_threads, bool fineGrained = false);
	void printTopFilesAsync_v2(uint16_t num_threads);
};