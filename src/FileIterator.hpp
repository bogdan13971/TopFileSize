#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <functional>

class FileIterator
{
private:
	using dir_it = boost::filesystem::directory_iterator;
	using rdir_it = boost::filesystem::recursive_directory_iterator;
	using path = boost::filesystem::path;
	
	using FileCB = std::function<void(const path&)>;
	using FileThreadCB = std::function<void(const path&, uint16_t)>;

private:
	static const boost::filesystem::directory_options opts;
	static bool is_current_path(const path& path);
	static bool is_special_file(const path& path);

public:
	/**
	 * @brief Iterate all the files and directories
	 * @param root Root directory to start traversal
	 * @param fileCB Function to apply to each file or directory traversed
	*/
	static void sync_iteration(const std::string& root, const FileCB& fileCB);

	/**
	 * @brief Iterate all the files and directories using multiple threads
	 * @param root Root directory to start traversal
	 * @param fileCB Function to apply to each file or directory traversed
	 * Each thread traverses an entire subtree from the root directory
	*/
	static void async_iteration(const std::string& root, uint16_t num_threads, const FileCB& fileCB);

	/**
	 * @brief Iterate all the files and directories using multiple threads
	 * @param root Root directory to start traversal
	 * @param fileCB Function to apply to each file or directory traversed, uses thread id for extra functionality, thread id mapped to [0, num_threads)
	 * Each thread traverses a subset from the list of total files and directories
	*/
	static void async_iteration_v2(const std::string& root, uint16_t num_threads, const FileThreadCB& fileCB);
};