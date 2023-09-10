#include <gtest/gtest.h>
#include "DirectoryTree.hpp"
#include <LUT/NodeIterator.hpp>

#include <boost/filesystem.hpp>

using namespace lut;

TEST(DirectoryTree, ArbitraryDirectory)
{
	auto filename = [](auto path) {
		return path.stem().string();
	};

	boost::filesystem::path cwd(__FILE__);
	Tree tree = buildDirectoryTree((cwd / ".." / "a").string(), filename);

	auto it = tree.begin();
	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");

	++it;
	EXPECT_EQ(it->getKey(), "e");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it)->getKey(), "f");

	++it;
	EXPECT_EQ(it->getKey(), "f");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it)->getKey(), "g");

	++it;
	EXPECT_EQ(it->getKey(), "h");
	EXPECT_EQ(parent(it)->getKey(), "f");
	EXPECT_EQ(nextSibling(it)->getKey(), "i");

	++it;
	EXPECT_EQ(it->getKey(), "i");
	EXPECT_EQ(parent(it)->getKey(), "f");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "p");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it)->getKey(), "q");

	++it;
	EXPECT_EQ(it->getKey(), "q");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it)->getKey(), "u");

	++it;
	EXPECT_EQ(it->getKey(), "u");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "g");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "c");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "d");

	++it;
	EXPECT_EQ(it->getKey(), "j");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it)->getKey(), "k");

	++it;
	EXPECT_EQ(it->getKey(), "k");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it)->getKey(), "l");

	++it;
	EXPECT_EQ(it->getKey(), "m");
	EXPECT_EQ(parent(it)->getKey(), "k");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "l");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "n");
	EXPECT_EQ(parent(it)->getKey(), "l");
	EXPECT_EQ(nextSibling(it), tree.end());
}
