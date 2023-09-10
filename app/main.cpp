#include "FileExplorer.hpp"
#include <boost/format.hpp>

int main()
{

	auto stem = [](auto path)
	{
		return path.string();
	};

	auto filesize = [](auto path) -> int
	{
		if (boost::filesystem::is_directory(path))
		{
			return 0;
		}
		return boost::filesystem::file_size(path);
	};

	const std::string& root = "D:\\repos\\Cache\\Cache";

	FileExplorer fe(root, stem, filesize);

	fe.init();
	fe.start();

	return 0;
}
