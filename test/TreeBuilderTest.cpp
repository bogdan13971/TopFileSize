#include <gtest/gtest.h>
#include <LUT/TreeBuilder.hpp>
#include <LUT/NodeIterator.hpp>

using namespace lut;

TEST(TreeIteratorTest, Root)
{
	TreeBuilder builder("a");
	Tree tree = builder.build();

	auto it = tree.begin();
	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST(TreeBuilderTest, DepthOne)
{
	TreeBuilder builder("a");
	Tree tree = builder.addChild("b")
								.addChild("c")
								.addChild("d")
								.build();

	auto it = tree.begin();
	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");

	++it;
	EXPECT_EQ(it->getKey(), "c");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "d");

	++it;
	EXPECT_EQ(it->getKey(), "d");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST(TreeBuilderTest, OnlyParents)
{
	TreeBuilder builder("a");
	Tree tree = builder.addParent("b")
									.addParent("c")
										.addParent("d")
											.addChild("e")
										.endParent()
									.endParent()
								.endParent()
								.build();

	auto it = tree.begin();
	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "c");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "d");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "e");
	EXPECT_EQ(parent(it)->getKey(), "d");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST(TreeBuilderTest, BinaryComplete)
{
	TreeBuilder builder("a");
	Tree tree = builder
		.addParent("b")
			.addChild("d")
			.addChild("e")
		.endParent()
		.addParent("c")
			.addChild("f")
			.addChild("g")
		.endParent()
		.build();

	auto it = tree.begin();
	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");

	++it;
	EXPECT_EQ(it->getKey(), "d");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it)->getKey(), "e");

	++it;
	EXPECT_EQ(it->getKey(), "e");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "c");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it), tree.end());

	++it;
	EXPECT_EQ(it->getKey(), "f");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it)->getKey(), "g");

	++it;
	EXPECT_EQ(it->getKey(), "g");
	EXPECT_EQ(parent(it)->getKey(), "c");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST(TreeBuilderTest, Arbitrary)
{
	TreeBuilder builder("a");
	Tree tree = builder
		.addParent("b")
			.addChild("e")
			.addParent("f")
				.addChild("h")
				.addParent("i")
					.addChild("u")
					.addChild("p")
					.addChild("q")
				.endParent()
			.endParent()
			.addChild("g")
		.endParent()
		.addParent("c")
			.addChild("j")
			.addParent("k")
				.addChild("m")
			.endParent()
			.addParent("l")
				.addChild("n")
			.endParent()
		.endParent()
		.addChild("d")

		.build();

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
	EXPECT_EQ(it->getKey(), "u");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it)->getKey(), "p");

	++it;
	EXPECT_EQ(it->getKey(), "p");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it)->getKey(), "q");

	++it;
	EXPECT_EQ(it->getKey(), "q");
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