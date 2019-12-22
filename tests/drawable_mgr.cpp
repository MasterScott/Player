#include "drawable_list.h"
#include "drawable_mgr.h"
#include "doctest.h"

TEST_SUITE_BEGIN("DrawableMgr");

namespace {

class TestSprite : public Drawable {
	public:
		TestSprite(bool global) : Drawable(TypeSprite, 0, global) { DrawableMgr::Register(this); }
		void Draw(Bitmap&) override {}
};

}

TEST_CASE("LocalSet") {
	DrawableMgr::SetLocalList(nullptr);

	REQUIRE_EQ(DrawableMgr::GetLocalListPtr(), nullptr);
	{
		DrawableList list;

		REQUIRE_FALSE(list.IsDirty());

		DrawableMgr::SetLocalList(&list);

		REQUIRE(list.IsDirty());

		REQUIRE_EQ(DrawableMgr::GetLocalListPtr(), &list);
		REQUIRE_EQ(&DrawableMgr::GetLocalList(), &list);
		REQUIRE_NE(&DrawableMgr::GetGlobalList(), &list);
	}
	REQUIRE_EQ(DrawableMgr::GetLocalListPtr(), nullptr);
}

TEST_CASE("Drawables") {
	DrawableList list;
	DrawableMgr::SetLocalList(&list);

	REQUIRE(list.IsDirty());
	list.Sort();
	REQUIRE_FALSE(list.IsDirty());

	auto& glist = DrawableMgr::GetGlobalList();

	auto global = std::make_unique<TestSprite>(true);

	REQUIRE_EQ(glist.size(), 1L);
	REQUIRE_EQ(*glist.begin(), global.get());
	REQUIRE_FALSE(glist.IsDirty());
	REQUIRE_FALSE(list.IsDirty());

	REQUIRE_EQ(list.size(), 0L);

	auto local = std::make_unique<TestSprite>(false);

	REQUIRE_EQ(glist.size(), 1L);
	REQUIRE_EQ(*glist.begin(), global.get());
	REQUIRE_FALSE(glist.IsDirty());
	REQUIRE_FALSE(list.IsDirty());

	global->SetZ(10);

	REQUIRE(glist.IsDirty());
	REQUIRE_FALSE(list.IsDirty());

	local->SetZ(10);

	REQUIRE(glist.IsDirty());
	REQUIRE(list.IsDirty());

	global.reset();

	REQUIRE_EQ(glist.size(), 0L);

	REQUIRE_EQ(list.size(), 1L);
	REQUIRE_EQ(*list.begin(), local.get());

	local.reset();

	REQUIRE_EQ(glist.size(), 0L);

	REQUIRE_EQ(list.size(), 0L);
}

TEST_SUITE_END();