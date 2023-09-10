#include "DirectoryTree.hpp"

#include <LUT/TreeBuilder.hpp>
#include <boost/filesystem.hpp>

lut::Tree buildDirectoryTree(
	const std::string& root,
	const std::function<std::string(const boost::filesystem::path&)>& keyFunctor
)
{
	namespace bf = boost::filesystem;
	
	lut::TreeBuilder builder(keyFunctor(bf::path(root)));

	//dir points to first child of path
	bf::recursive_directory_iterator dir(root);
	bf::recursive_directory_iterator end;

	int lastDepth = dir.depth();

	while (dir != end)
	{
		//close parents when returning back
		while (lastDepth > dir.depth())
		{
			lastDepth--;
			builder.endParent();
		}

		if (bf::is_directory(dir.status()))
		{
			lastDepth++;
			builder.addParent(keyFunctor(dir->path()));
		}
		else
		{
			builder.addChild(keyFunctor(dir->path()));
		}

		++dir;
	}

	while (lastDepth > 0)
	{
		builder.endParent();
		--lastDepth;
	}

	return builder.build();
}
