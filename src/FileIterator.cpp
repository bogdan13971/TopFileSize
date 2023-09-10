#include "FileIterator.hpp"
#include <boost/thread.hpp>
#include <boost/asio.hpp>

FileIterator::FileIterator(const std::string& root, const FileCB& fileCallback)
	: root{ root }, 
	fileCallback{ fileCallback }
{}

bool FileIterator::is_current_path(const boost::filesystem::path& path)
{
	return path == boost::filesystem::current_path();
}

bool FileIterator::is_special_file(const boost::filesystem::path& path)
{
	return path.filename().c_str()[0] == '.';
}

void FileIterator::recurisve_iteration(const boost::filesystem::path& path)
{
	const auto opts = boost::filesystem::directory_options::skip_permission_denied | boost::filesystem::directory_options::pop_on_error;
 
	for (auto rdir = rdir_it(path, opts); rdir != rdir_it(); ++rdir)
	{
		//don't reccur if in current exe directory
		if (is_current_path(rdir->path()))
		{
			rdir.disable_recursion_pending();
			continue;
		}

		//don't reccur in hidden directories but skip hidden files
		if (is_special_file(rdir->path()))
		{
			if (boost::filesystem::is_directory(rdir->path()))
			{
				rdir.disable_recursion_pending();
			}
			continue;
		}

		fileCallback(rdir->path());
	}
}


void FileIterator::sync_iteration()
{
	recurisve_iteration(root);
}

void FileIterator::async_iteration(uint16_t num_threads)
{
	const auto opts = boost::filesystem::directory_options::skip_permission_denied | boost::filesystem::directory_options::pop_on_error;
	boost::asio::thread_pool pool(num_threads);

	for (auto dir = dir_it(root, opts); dir != dir_it(); ++dir)
	{
		//skip if in current exe directory or hidden file
		if (is_current_path(dir->path()) 
			|| is_special_file(dir->path()))
		{
			continue;
		}

		fileCallback(dir->path());
		if (boost::filesystem::is_directory(dir->path()))
		{
			boost::asio::post(pool, [this, path = dir->path()]()
			{
				recurisve_iteration(path);
			});
		}
	}

	pool.join();
}

void FileIterator::async_iteration_v2(uint16_t num_threads)
{
	const auto INITIAL_CAPACITY = 1'000u;
	const auto opts = boost::filesystem::directory_options::skip_permission_denied | boost::filesystem::directory_options::pop_on_error;
	boost::asio::thread_pool pool(num_threads);

	std::vector<boost::filesystem::path> files;
	files.reserve(INITIAL_CAPACITY);

	for (auto rdir = rdir_it(root, opts); rdir != rdir_it(); ++rdir)
	{
		//skip if in current exe directory or hidden file
		if (is_current_path(rdir->path())
			|| is_special_file(rdir->path()))
		{
			continue;
		}

		files.push_back(rdir->path());
	}

	auto thread_func = [&](size_t start, size_t end) {
		for (auto i = start; i < end; i++)
		{
			fileCallback(files[i]);
		}
	};

	const size_t load = files.size() / num_threads;
	for (size_t i = 0; i < num_threads; i++)
	{
		auto start_index = i * load;
		auto end_index = start_index + load;
		if (i == num_threads - 1)
		{
			end_index = files.size();
		}

		boost::asio::post(pool, [start_index, end_index, thread_func]() {
			thread_func(start_index, end_index);
		});
	}

	pool.join();
}