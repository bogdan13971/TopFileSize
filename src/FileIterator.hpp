#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <functional>

class FileIterator
{
private:
	using dir_it = boost::filesystem::directory_iterator;
	using rdir_it = boost::filesystem::recursive_directory_iterator;
	using FileCB = std::function<void(const boost::filesystem::path&)>;

private:
	const std::string root;
	FileCB fileCallback;

	void recurisve_iteration(const boost::filesystem::path& path);
	bool is_current_path(const boost::filesystem::path& path);
	bool is_special_file(const boost::filesystem::path& path);

public:
	FileIterator(const std::string& root, const FileCB& fileCallback);

	void sync_iteration();
	void async_iteration(uint16_t num_threads);
	void async_iteration_v2(uint16_t num_threads);
};