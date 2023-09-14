#include "FileIterator.hpp"
#include <boost/thread.hpp>
#include <boost/asio.hpp>

bool FileIterator::is_current_path(const boost::filesystem::path& path)
{
	return path == boost::filesystem::current_path();
}

bool FileIterator::is_special_file(const boost::filesystem::path& path)
{
	return path.filename().c_str()[0] == '.' || path.filename().c_str()[0] == '_';
}

void FileIterator::sync_iteration(const std::string& root, const FileIterator::FileCB& fileCB)
{
	for (auto rdir = rdir_it(root, opts); rdir != rdir_it(); ++rdir)
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

		fileCB(rdir->path());
	}
}

void FileIterator::async_iteration(const std::string& root, uint16_t num_threads, const FileIterator::FileCB& fileCB)
{
	boost::asio::thread_pool pool(num_threads);

	for (auto dir = dir_it(root, opts); dir != dir_it(); ++dir)
	{
		//skip if in current exe directory or hidden file
		if (is_current_path(dir->path()) 
			|| is_special_file(dir->path()))
		{
			continue;
		}

		fileCB(dir->path());
		if (boost::filesystem::is_directory(dir->path()))
		{
			boost::asio::post(pool, [root = std::move(dir->path().string()), cb = fileCB]()
			{
				sync_iteration(root, cb);
			});
		}
	}

	pool.join();
}

void FileIterator::async_iteration_v2(const std::string& root, uint16_t num_threads, const FileIterator::FileThreadCB& fileCB)
{
	const auto INITIAL_CAPACITY = 1'000u;

	std::vector<boost::filesystem::path> files;
	files.reserve(INITIAL_CAPACITY);

	for (auto rdir = rdir_it(root, opts); rdir != rdir_it(); ++rdir)
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

		files.push_back(rdir->path());
	}

	std::vector<std::thread> threads;

	const size_t load = files.size() / num_threads;
	for (size_t i = 0; i < num_threads; i++)
	{
		auto start_index = i * load;
		auto end_index = start_index + load;
		if (i == num_threads - 1)
		{
			end_index = files.size();
		}

		threads.emplace_back([start_index, end_index, threadId = i, &files, &fileCB]() {
			for (size_t i = start_index; i < end_index; i++)
			{
				fileCB(files[i], threadId);
			}
		});
	}

	for (int i = 0; i < num_threads; i++)
	{
		threads[i].join();
	}
}

const boost::filesystem::directory_options FileIterator::opts = 
		boost::filesystem::directory_options::skip_permission_denied 
		| boost::filesystem::directory_options::pop_on_error;