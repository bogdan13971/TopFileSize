#include <iostream>

#include "TopFileSize.hpp"

int main(int argc, char* argv[])
{
	if (argc == 1 || !strcmp(argv[1], "-h"))
	{
		std::cout << 
			"usage: FileSizeExplorer path mode [num_threads]\n"\
					"modes: \n"\
					" 1 sync file iteration - SLOW, single threaded\n"\
					" 2 async file iteration coarse grained heap - SLOWEST\n"\
					" 3 async file iteration fine grained heap - SLOW, uses less memory\n"\
					" 4 async file iteration with sync heap - FASTEST, uses more memory";

		return 0;
	}

	if (argc > 4)
	{
		std::cout << "Too many arguments \n";
		return -1;
	}

	const char* path = argv[1];
	if (argc == 2)
	{
		std::cout << "Not enough arguments \n";
		return -1;
	}

	int mode = atoi(argv[2]);

	if (mode == 1 && argc >= 2)
	{
		std::cout << "Bad arguments for mode 1\n";
		return -1;
	}

	if (mode >= 2 && argc == 3) 
	{
		std::cout << "Bad arguments for mode \n";
		return -1;
	}

	int num_threads = 0;
	if (argc == 4)
	{
		num_threads = atoi(argv[3]);
	}

	std::cout << "Running for \n" <<
		"path " << path << "\n" <<
		"mode " << mode << "\n";

	if (argc == 4)
	{
		std::cout << "threads " << num_threads << "\n";
	}

	std::cout << "\n";

	TopFileSize topFileSize{ path };

	try {
		switch (mode)
		{
		case 1:
		{
			topFileSize.printTopFilesSync();
			break;
		}
		case 2:
		{
			topFileSize.printTopFilesAsync(num_threads);
			break;
		}
		case 3:
		{
			topFileSize.printTopFilesAsync(num_threads, true);
			break;
		}
		case 4:
		{
			topFileSize.printTopFilesAsync_v2(num_threads);
			break;
		}
		default:
			std::cout << "Wrong mode\n";
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: \n"<< e.what() << "\n";
		return -1;
	}

	return 0;
}
