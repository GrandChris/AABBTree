///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTree2Leef.h
// Revision:	 1.0
// Date:		 25.07.2020
// Author:		 Christian Steinbrecher
// Description:  A leef of an AABBTree
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AABBTreeNode.h"

template<typename T>
class AABBTree<T>::Leef : public AABBTree<T>::Node {
public:
	using sptr = std::shared_ptr<AABBTree<T>::Leef>;

	Leef(std::shared_ptr<T> collisionHanlder);

	// updates all parrent nodes with the changed min and max values
	// returns a pointer to an CollisionHandler if a collision is detected
	std::shared_ptr<T> update();

	// removes itself from the AABBTree
	void remove();

	// set and get of parrent class
	using AABBTree<T>::Node::getMin;
	using AABBTree<T>::Node::getMax;
	using AABBTree<T>::Node::setMin;
	using AABBTree<T>::Node::setMax;

private:
	virtual bool insertRecursive(AABBTree<T>::Node::sptr& leef) override;
	virtual std::shared_ptr<T> checkCollision(Node const& leef) const override;

	std::shared_ptr<T> mCollisionHandler;
};


// #######+++++++ Implementation +++++++#######

template<typename T>
inline AABBTree<T>::Leef::Leef(std::shared_ptr<T> collisionHanlder)
	: mCollisionHandler(collisionHanlder)
{
}

template<typename T>
inline std::shared_ptr<T> AABBTree<T>::Leef::update()
{
	auto collidingNode = AABBTree<T>::Node::updateParrent();
	if (collidingNode != nullptr)
	{	// possible collision found

		// rebalance the tree
		collidingNode->reinsertParrent(*this);

		// check for real collision
		auto collidingObject = collidingNode->checkCollision(*this);
		if (collidingObject != nullptr) 
		{	// collision found
			return collidingObject;
		}
	}

	return nullptr;
}


template<typename T>
inline void AABBTree<T>::Leef::remove()
{
	AABBTree<T>::Node::remove();
}

template<typename T>
inline bool AABBTree<T>::Leef::insertRecursive(typename AABBTree<T>::Node::sptr& leef)
{
	// can not insert into a leef
	return false;
}

template<typename T>
inline std::shared_ptr<T> AABBTree<T>::Leef::checkCollision(Node const& leef) const
{
	return mCollisionHandler;
}
