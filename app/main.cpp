#include <iostream>

#include "TopFileSize.hpp"

int main(int argc, char* argv[])
{
	const char* default_path = "D:\\projects\\boost_1_82_0";

	const char* path = argv[1];
	int mode = atoi(argv[2]);

	int num_threads = 1;
	if (argc == 4)
	{
		num_threads = atoi(argv[3]);
	}

	std::cout << "Running for \n" <<
		"path " << path << "\n" <<
		"mode " << mode << "\n" <<
		"threads " << num_threads << "\n";

	if (mode >= 2 && argc == 3)
	{
		std::cout << "Bad argc\n";
		return -1;
	}

	TopFileSize topFileSize{ std::string(argv[1]) };

	switch (mode)
	{
	case 1:
	{
		topFileSize.printTopFilesSync();
		break;
	}
	case 2:
	{
		topFileSize.printTopFilesAsync(atoi(argv[3]));
		break;
	}
	case 3:
	{
		topFileSize.printTopFilesAsync(atoi(argv[3]), true);
		break;
	}
	case 4:
	{
		topFileSize.printTopFilesAsync_v2(atoi(argv[3]));
		break;
	}
	default:
		std::cout << "Wrong mode\n";
		return -1;
	}

	return 0;
}
