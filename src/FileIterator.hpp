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
	using FileThreadCB = std::function<void(const path&, uint32_t)>;

private:
	static const boost::filesystem::directory_options opts;
	static bool is_current_path(const path& path);
	static bool is_special_file(const path& path);

public:
	static void sync_iteration(const std::string& root, const FileCB& fileCB);
	static void async_iteration(const std::string& root, uint16_t num_threads, const FileCB& fileCB);
	static void async_iteration_v2(const std::string& root, uint16_t num_threads, const FileThreadCB& fileCB);
};