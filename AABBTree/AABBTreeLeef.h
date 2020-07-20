///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTreeLeef.h
// Revision:	 1.0
// Date:		 19.07.2020
// Author:		 Christian Steinbrecher
// Description:  Leef of the binary tree
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AABBTreeBaseNode.h"

template<typename T>
class AABBTree<T>::Leef : public BaseNode {
public:
	Leef(std::shared_ptr<T> obj, glm::vec3 const& pos, glm::vec3 const& min, glm::vec3 const& max);
	void setPosition(glm::vec3 const& pos);	// pos is middle of the bounding box
	void setBoundingBox(glm::vec3 const& min, glm::vec3 const& max);
	void remove();

	bool handleCollision(Leef& other);
private:
	virtual void insert(Leef& leef) override;
	void calcAbs();

	void update();

	// updates the bounding box of all necessarry parrent nodes
	// calls obj->handleCollision(other) when a collision is found
	virtual Leef* checkCollision(Leef& leef) override;


	std::shared_ptr<T> mObj;

	glm::vec3 mPos = { 0.0f,  0.0f,  0.0f };
	glm::vec3 mMin = { -0.5f, -0.5f, -0.5f };
	glm::vec3 mMax = { 0.5f,  0.5f,  0.5f };
};



// #######+++++++ Implementation +++++++#######

#include <cassert>

template<typename T>
inline AABBTree<T>::Leef::Leef(std::shared_ptr<T> obj, glm::vec3 const& pos, glm::vec3 const& min, glm::vec3 const& max)
	: mObj(std::move(obj)),
	  mPos(pos),
	  mMin(min),
	  mMax(max)
{
	calcAbs();
}

template<typename T>
inline void AABBTree<T>::Leef::setPosition(glm::vec3 const& pos)
{
	mPos = pos;
	calcAbs();
	update();
}

template<typename T>
inline void AABBTree<T>::Leef::setBoundingBox(glm::vec3 const& min, glm::vec3 const& max)
{
	mMin = glm::min(min, max);
	mMax = glm::max(min, max);
	calcAbs();
	update();
}

template<typename T>
inline void AABBTree<T>::Leef::remove()
{
	if (BaseNode::mParrent == nullptr) {
		return;
	}

	if (isLeft) {
		BaseNode::mParrent->mLeft = nullptr;
	}
	else {
		BaseNode::mParrent->mRight = nullptr;
	}

	BaseNode::mParrent == nullptr;
}

template<typename T>
inline bool AABBTree<T>::Leef::handleCollision(Leef& other)
{
	if (mObj != nullptr && other.mObj != nullptr) {
		return mObj->handleCollision(other.mObj);
	}
	else if(mObj == nullptr) {
		return true;	// delete 
	}
	else {
		return false;
	}
	
}

template<typename T>
inline void AABBTree<T>::Leef::insert(Leef& leef)
{
	assert(BaseNode::mParrent != nullptr);

	// create new node
	auto node = new Node();
	node->setParrent(BaseNode::mParrent);

	if (this == BaseNode::mParrent->getLeft()) {
		BaseNode::mParrent->setLeft(*node);
	}
	else {
		assert(this == BaseNode::mParrent->getRight());
		BaseNode::mParrent->setRight(*node);
	}

	node->insert(*this);
	node->insert(leef);
}

template<typename T>
inline void AABBTree<T>::Leef::calcAbs()
{
	assert(mMin.x <= mMax.x);
	assert(mMin.y <= mMax.y);
	assert(mMin.z <= mMax.z);

	BaseNode::mAbsMin = mPos + mMin;
	BaseNode::mAbsMax = mPos + mMax;
}

template<typename T>
inline void AABBTree<T>::Leef::update()
{
	if (BaseNode::mParrent != nullptr) {
		BaseNode::mParrent->update(*this, BaseNode::mIsLeft);
	}
}

template<typename T>
inline AABBTree<T>::Leef* AABBTree<T>::Leef::checkCollision(Leef& leef)
{
	return this;
}
