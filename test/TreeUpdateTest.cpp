#include <gtest/gtest.h>
#include <Tree.hpp>
#include <TreeBuilder.hpp>

class TreeUpdateTest : public ::testing::Test
{
public:
	TreeUpdateTest()
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

TEST_F(TreeUpdateTest, updateSubtreeEnd)
{
	try
	{
		tree.update(tree.end());
	}
	catch (const std::out_of_range& e) {
		EXPECT_EQ(e.what(), std::string("node index out of range"));
	}
	catch (...) {
		FAIL() << "Expected std::out_of_range";
	}
}

TEST_F(TreeUpdateTest, updateSubtreeNode)
{
	auto a = tree.begin();
	auto b = nextChild(a);
	auto e = nextChild(b);
	auto f = nextSibling(e);
	auto h = nextChild(f);
	auto i = nextSibling(h);
	auto u = nextChild(i);
	auto p = nextSibling(u);
	auto q = nextSibling(p);

	tree.update(f);

	EXPECT_EQ(a->getValue(), 6);
	EXPECT_EQ(a->isLoaded(), false);

	EXPECT_EQ(b->getValue(), 6);
	EXPECT_EQ(b->isLoaded(), false);

	EXPECT_EQ(e->getValue(), 0);
	EXPECT_EQ(e->isLoaded(), false);

	EXPECT_EQ(f->getValue(), 6);
	EXPECT_EQ(f->isLoaded(), true);

	EXPECT_EQ(h->getValue(), 1);
	EXPECT_EQ(h->isLoaded(), true);

	EXPECT_EQ(i->getValue(), 4);
	EXPECT_EQ(i->isLoaded(), true);

	EXPECT_EQ(u->getValue(), 1);
	EXPECT_EQ(u->isLoaded(), true);

	EXPECT_EQ(p->getValue(), 1);
	EXPECT_EQ(p->isLoaded(), true);

	EXPECT_EQ(q->getValue(), 1);
	EXPECT_EQ(q->isLoaded(), true);
}

TEST_F(TreeUpdateTest, updateSubtreeNodeAlreadyUpdated)
{
	auto a = tree.begin();
	auto b = nextChild(a);
	auto e = nextChild(b);
	auto f = nextSibling(e);
	auto h = nextChild(f);
	auto i = nextSibling(h);
	auto u = nextChild(i);
	auto p = nextSibling(u);
	auto q = nextSibling(p);

	tree.update(i);

	EXPECT_EQ(a->getValue(), 4);
	EXPECT_EQ(a->isLoaded(), false);

	EXPECT_EQ(b->getValue(), 4);
	EXPECT_EQ(b->isLoaded(), false);

	EXPECT_EQ(e->getValue(), 0);
	EXPECT_EQ(e->isLoaded(), false);

	EXPECT_EQ(f->getValue(), 4);
	EXPECT_EQ(f->isLoaded(), false);

	EXPECT_EQ(h->getValue(), 0);
	EXPECT_EQ(h->isLoaded(), false);

	EXPECT_EQ(i->getValue(), 4);
	EXPECT_EQ(i->isLoaded(), true);

	EXPECT_EQ(u->getValue(), 1);
	EXPECT_EQ(u->isLoaded(), true);

	EXPECT_EQ(p->getValue(), 1);
	EXPECT_EQ(p->isLoaded(), true);

	EXPECT_EQ(q->getValue(), 1);
	EXPECT_EQ(q->isLoaded(), true);

	tree.update(b);

	EXPECT_EQ(a->getValue(), 9);
	EXPECT_EQ(a->isLoaded(), false);

	EXPECT_EQ(b->getValue(), 9);
	EXPECT_EQ(b->isLoaded(), true);

	EXPECT_EQ(e->getValue(), 1);
	EXPECT_EQ(e->isLoaded(), true);

	EXPECT_EQ(f->getValue(), 6);
	EXPECT_EQ(f->isLoaded(), true);

	EXPECT_EQ(h->getValue(), 1);
	EXPECT_EQ(h->isLoaded(), true);

	EXPECT_EQ(i->getValue(), 4);
	EXPECT_EQ(i->isLoaded(), true);

	EXPECT_EQ(u->getValue(), 1);
	EXPECT_EQ(u->isLoaded(), true);

	EXPECT_EQ(p->getValue(), 1);
	EXPECT_EQ(p->isLoaded(), true);

	EXPECT_EQ(q->getValue(), 1);
	EXPECT_EQ(q->isLoaded(), true);
}

TEST_F(TreeUpdateTest, updateSubtreeNodeIncluded)
{
	auto a = tree.begin();
	auto b = nextChild(a);
	auto e = nextChild(b);
	auto f = nextSibling(e);
	auto h = nextChild(f);
	auto i = nextSibling(h);
	auto u = nextChild(i);
	auto p = nextSibling(u);
	auto q = nextSibling(p);

	tree.update(b);

	EXPECT_EQ(a->getValue(), 9);
	EXPECT_EQ(a->isLoaded(), false);

	EXPECT_EQ(b->getValue(), 9);
	EXPECT_EQ(b->isLoaded(), true);

	EXPECT_EQ(e->getValue(), 1);
	EXPECT_EQ(e->isLoaded(), true);

	EXPECT_EQ(f->getValue(), 6);
	EXPECT_EQ(f->isLoaded(), true);

	EXPECT_EQ(h->getValue(), 1);
	EXPECT_EQ(h->isLoaded(), true);

	EXPECT_EQ(i->getValue(), 4);
	EXPECT_EQ(i->isLoaded(), true);

	EXPECT_EQ(u->getValue(), 1);
	EXPECT_EQ(u->isLoaded(), true);

	EXPECT_EQ(p->getValue(), 1);
	EXPECT_EQ(p->isLoaded(), true);

	EXPECT_EQ(q->getValue(), 1);
	EXPECT_EQ(q->isLoaded(), true);

	tree.update(i);

	EXPECT_EQ(a->getValue(), 9);
	EXPECT_EQ(a->isLoaded(), false);

	EXPECT_EQ(b->getValue(), 9);
	EXPECT_EQ(b->isLoaded(), true);

	EXPECT_EQ(e->getValue(), 1);
	EXPECT_EQ(e->isLoaded(), true);

	EXPECT_EQ(f->getValue(), 6);
	EXPECT_EQ(f->isLoaded(), true);

	EXPECT_EQ(h->getValue(), 1);
	EXPECT_EQ(h->isLoaded(), true);

	EXPECT_EQ(i->getValue(), 4);
	EXPECT_EQ(i->isLoaded(), true);

	EXPECT_EQ(u->getValue(), 1);
	EXPECT_EQ(u->isLoaded(), true);

	EXPECT_EQ(p->getValue(), 1);
	EXPECT_EQ(p->isLoaded(), true);

	EXPECT_EQ(q->getValue(), 1);
	EXPECT_EQ(q->isLoaded(), true);
}