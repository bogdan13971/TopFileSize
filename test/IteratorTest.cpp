#include <gtest/gtest.h>
#include <Tree.hpp>
#include <TreeBuilder.hpp>

class NodeIteratorTest : public ::testing::Test
{
public:
	NodeIteratorTest()
		: tree{ TreeBuilder("a")
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
			.build() }
	{}

protected:
	Tree tree;
};

TEST_F(NodeIteratorTest, begin)
{
	auto it = tree.begin();

	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST_F(NodeIteratorTest, end)
{
	auto it = tree.end();

	EXPECT_EQ(it, tree.end());
}

TEST_F(NodeIteratorTest, dereferenceEnd)
{
	auto it = tree.end();

	try
	{
		*it;
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, increment)
{
	auto it = tree.begin();
	++it;

	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");
}

TEST_F(NodeIteratorTest, incrementEnd)
{
	auto it = tree.end();

	try
	{
		++it;
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, decrementEnd)
{
	auto it = tree.end();

	try
	{
		--it;
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, decrementRoot)
{
	auto it = tree.begin();

	try
	{
		--it;
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, decrement)
{
	auto it = endOfSubtree(tree.begin());
	--it;

	EXPECT_EQ(it->getKey(), "n");
	EXPECT_EQ(parent(it)->getKey(), "l");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST_F(NodeIteratorTest, nextSiblingRoot)
{
	auto it = nextSibling(tree.begin());

	EXPECT_EQ(it, tree.end());
}

TEST_F(NodeIteratorTest, nextSibling)
{
	auto it = nextChild(tree.begin());

	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");
}

TEST_F(NodeIteratorTest, nextSiblingNoSibling)
{
	auto d = nextSibling(nextSibling(nextChild(tree.begin())));
	auto it = nextSibling(d);

	EXPECT_EQ(it, tree.end());
}

TEST_F(NodeIteratorTest, nextSiblingEnd)
{
	try
	{
		auto it = nextSibling(tree.end());
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, nextChildRoot)
{
	auto it = nextChild(tree.begin());

	EXPECT_EQ(it->getKey(), "b");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "c");
}

TEST_F(NodeIteratorTest, nextChild)
{
	auto it = nextSibling(nextChild(tree.begin()));

	EXPECT_EQ(it->getKey(), "c");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it)->getKey(), "d");
}

TEST_F(NodeIteratorTest, nextChildNoChild)
{
	auto e = nextChild(nextChild(tree.begin()));
	auto it = nextChild(e);

	EXPECT_EQ(it, tree.end());
}

TEST_F(NodeIteratorTest, nextChildEnd)
{
	try
	{
		auto it = nextChild(tree.end());
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, parentRoot)
{
	auto it = parent(tree.begin());

	EXPECT_EQ(it, tree.end());
}

TEST_F(NodeIteratorTest, parent)
{
	auto it = parent(nextChild(tree.begin()));

	EXPECT_EQ(it->getKey(), "a");
	EXPECT_EQ(parent(it), tree.end());
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST_F(NodeIteratorTest, parentEnd)
{
	try
	{
		auto it = parent(tree.end());
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(NodeIteratorTest, endOfSubtreeRoot)
{
	auto it = endOfSubtree(tree.begin());

	EXPECT_EQ(it->getKey(), "d");
	EXPECT_EQ(parent(it)->getKey(), "a");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST_F(NodeIteratorTest, endOfSubtreeLeaf)
{
	//points to leaf e
	auto e = nextChild(nextChild(tree.begin()));
	auto it = endOfSubtree(e);

	EXPECT_EQ(it->getKey(), "e");
	EXPECT_EQ(parent(it)->getKey(), "b");
	EXPECT_EQ(nextSibling(it)->getKey(), "f");
}

TEST_F(NodeIteratorTest, endOfSubtreeNode)
{
	//points to node f
	auto f = nextSibling(nextChild(nextChild(tree.begin())));
	auto it = endOfSubtree(f);

	EXPECT_EQ(it->getKey(), "q");
	EXPECT_EQ(parent(it)->getKey(), "i");
	EXPECT_EQ(nextSibling(it), tree.end());
}

TEST_F(NodeIteratorTest, endOfSubtreeEnd)
{
	try
	{
		auto it = endOfSubtree(tree.end());
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}
