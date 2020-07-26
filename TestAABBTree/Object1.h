#pragma once



#include <AABBTree.h>



class Object1;

class CollisionHandler {
public:
	virtual void handleCollision(Object1& other) = 0;
};


using MyAABBTree = AABBTree<CollisionHandler>;


class Object1 : public CollisionHandler {
public:
	using sptr = std::shared_ptr<Object1>;

	virtual void handleCollision(Object1& other) override {

	}

	void update() {
		mColor = { 0, 0.5f, 0 };

		if (mLeef == nullptr) {
			return;
		}

		mLeef->setMin(mPos - size);
		mLeef->setMax(mPos + size);

		auto collidingObject = mLeef->update();
		if (collidingObject != nullptr)
		{
			mColor = { 0.5f, 0, 0 };
			collidingObject->handleCollision(*this);
		}
	}

	void setPos(glm::vec3 const& pos) {
		mPos = pos;

		update();
	}
	glm::vec3 getPos() const {
		return mPos;
	}

	void setColor(glm::vec3 const& color) {
		mColor = color;
	}
	glm::vec3 getColor() const {
		return mColor;
	}

	void setAABBTreeLeef(MyAABBTree::Leef::sptr leef) {
		mLeef = std::move(leef);

		mLeef->setMin(mPos - size);
		mLeef->setMax(mPos + size);
	}

private:
	glm::vec3 mPos = {};
	glm::vec3 mColor = { 0.5f, 0.5f, 0.5f };

	glm::vec3 const size = { 0.5f, 0.5f, 0.5f };

	MyAABBTree::Leef::sptr mLeef;
};
