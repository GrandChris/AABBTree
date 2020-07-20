///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTreeLeef.h
// Revision:	 1.0
// Date:		 19.07.2020
// Author:		 Christian Steinbrecher
// Description:  Leef of the binary tree
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AABBTreeBaseNode.h"

#include <memory>


template<typename T>
class AABBTree<T>::Node : public BaseNode {
public:

	// D-Tor
	virtual ~Node();

	// Allocates a new leef and inserts it into the AABBTree
	Leef& createLeef(std::shared_ptr<T> obj,
		glm::vec3 const& pos = { 0.0f,  0.0f,  0.0f },
		glm::vec3 const& min = { -0.5f, -0.5f, -0.5f },
		glm::vec3 const& max = { 0.5f,  0.5f,  0.5f });

	BaseNode* getLeft() const;
	BaseNode* getRight() const;

	void setLeft(BaseNode& node);
	void setRight(BaseNode& node);

	virtual void insert(Leef& leef) override;

	virtual Leef * checkCollision(Leef& leef) override;


	// updates the bounding box of all necessarry parrent nodes
	// calls obj->handleCollision(other) when a collision is found
	void update(Leef & leef, bool isLeft);

	// leef got removed
	void update();

private:
	void calcMinMax(glm::vec3 & min, glm::vec3 & max);


	BaseNode* mLeft = nullptr;
	BaseNode* mRight = nullptr;
};



// #######+++++++ Implementation +++++++#######

#include "AABBTreeLeef.h"

template<typename T>
inline AABBTree<T>::Node::~Node()
{
	// remove itself from the parrent
	if (BaseNode::mParrent != nullptr) {
		if (isLeft) {
			BaseNode::mParrent->mLeft = nullptr;
		}
		else {
			BaseNode::mParrent->mRight = nullptr;
		}
	}

	// delete all children
	delete mLeft; mLeft = nullptr;
	delete mRight mRight = nullptr;

}

template<typename T>
inline typename AABBTree<T>::Leef& AABBTree<T>::Node::createLeef(std::shared_ptr<T> obj, glm::vec3 const& pos, glm::vec3 const& min, glm::vec3 const& max)
{
	auto leef = new Leef(obj, pos, min, max);

	insert(*leef);

	return *leef;
}

template<typename T>
inline typename AABBTree<T>::BaseNode* AABBTree<T>::Node::getLeft() const
{
	return mLeft;
}

template<typename T>
inline typename AABBTree<T>::BaseNode* AABBTree<T>::Node::getRight() const
{
	return mRight;
}

template<typename T>
inline void AABBTree<T>::Node::setLeft(BaseNode& node)
{
	mLeft = &node;
}

template<typename T>
inline void AABBTree<T>::Node::setRight(BaseNode& node)
{
	mRight = &node;
}

template<typename T>
inline void AABBTree<T>::Node::insert(Leef& leef)
{
	leef.setParrent(this);
	BaseNode::mAbsMin = glm::min(BaseNode::mAbsMin, leef.getMin());
	BaseNode::mAbsMax = glm::max(BaseNode::mAbsMax, leef.getMax());

	// insert into existing node
	if (mLeft == nullptr) {
		mLeft = &leef;
		return;
	}
	else if (mRight == nullptr) {
		mRight = &leef;
		return;
	}
	else if (mLeft->volume(leef) <= mRight->volume(leef)) {
		mLeft->insert(leef);
	}
	else {
		mRight->insert(leef);
	}
}

template<typename T>
inline AABBTree<T>::Leef* AABBTree<T>::Node::checkCollision(Leef& leef)
{
	if (mLeft != nullptr && !mLeft->isOutside(isOutside)) {
		return mLeft->checkCollision(leef);
	}
	else if (mRight != nullptr && !mRight->isOutside(isOutside)) {
		return mRight->checkCollision(leef);
	}

	return nullptr;
}

template<typename T>
inline void AABBTree<T>::Node::update()
{
	glm::vec3 min = {};
	glm::vec3 max = {};
	calcMinMax(min, max);

	if (min != getMin() || max != getMax())
	{ // boundaries have changed
		if (BaseNode::mParrent != nullptr) {
			// update parrent node
			BaseNode::mParrent->update();
		}
	}
}

template<typename T>
inline void AABBTree<T>::Node::update(Leef & leef, bool isLeft)
{
	auto callerNode = isLeft ? mLeft : mRight;
	auto otherNode = isLeft ? mRight : mLeft;

	if (otherNode != nullptr && !callerNode.isOutside(otherNode))
	{ // collision detected
		// search for colliding element
		Leef * collidingLeef = checkCollision(leef);
		if (collidingLeef != nullptr) {
			bool remove = leef.handleCollision(&collidingLeef);
			if (remove == true) {
				leef.remove();
				delete leef;
				return;
			}
		}
		else
		{ // no collision found
			// test if bounding box gets smaller if element is inserted into the other bounding box
			if (otherNode->volume(leef) < callerNode->volume(leef)
			{
				// remove element from caller Node and insert it into the other bounding box
				leef.remove();
				insert(leef);
			}
		}
	}

	// calculate new min max
	auto const min = otherNode == nullptr ? callerNode->getMin() : glm::min(callerNode->getMin(), otherNode->getMin());
	auto const max = otherNode == nullptr ? callerNode->getMax() : glm::min(callerNode->getMax(), otherNode->getMax());

	if (min != getMin() || max != getMax())
	{ // boundaries have changed
		if (BaseNode::mParrent != nullptr) {
			// update parrent node
			BaseNode::mParrent->update(mObj, BaseNode::mIsLeft);
		}
	}


}

template<typename T>
inline void AABBTree<T>::Node::calcMinMax(glm::vec3& min, glm::vec3& max)
{
	if (mLeft != nullptr && mRight != nullptr) {
		min = glm::min(mLeft->getMin(), mRight->getMin());
		max = glm::min(mLeft->getMax(), mRight->getMax());
	}
	else if (mLeft != nullptr) {
		min = mLeft->getMin();
		max = mLeft->getMax();
	}
	else if (mRight != nullptr) {
		min = mRight->getMin();
		max = mRight->getMax();
	}
	else {
		min = {};
		max = {};
	}
}
