///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTree2Node.h
// Revision:	 1.0
// Date:		 25.07.2020
// Author:		 Christian Steinbrecher
// Description:  A node of an AABBTree
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AABBTree.h"

#include <glm/glm.hpp>

#include <tuple>
#include <ostream>


template<typename T>
class AABBTree<T>::Node {
public:
	using sptr = std::shared_ptr<Node>;

	// D-Tor
	virtual ~Node() = default;

	// Inserts a node as a leef into the tree
	virtual bool insert(sptr leef);

	// First object which as a collision with the given node
	virtual std::shared_ptr<T> checkCollision(Node const & leef) const;

	// returns the amount of Nodes inside the tree, including Leefs
	size_t size() const;

	void print(std::ostream& ost) const;

	// returns the height of the deepest leef;
	size_t height() const;

	// calls func with func(min, max) for every node in the tree
	template<typename TFunc>
	void getAllMinMax(TFunc func) const;

	// do not use
	void reinsertParrent(Node& node);

protected:
	virtual bool insertRecursive(sptr& leef);

	// returns parrent node with possible collision
	// returns nullptr if there is no collision
	sptr update(Node* child);
	sptr updateParrent();
	sptr remove();

	glm::vec3 getMin() const;
	glm::vec3 getMax() const;

	void setMin(glm::vec3 const& min);
	void setMax(glm::vec3 const& max);

	// Calculate volumne including the other node
	float volume(Node const& other) const;

	// Calculate if the other node is inside or outside this node
	bool isInside(Node const& other) const;
	bool isOutside(Node const& other) const;

	bool isInside(glm::vec3 const& min, glm::vec3 const& max) const;
	bool isOutside(glm::vec3 const& min, glm::vec3 const& max) const;

	// returns the node, on which insertion would result into the smalles volume
	sptr getSmallesVolumeNode(Node & node);



private:
	std::tuple<glm::vec3, glm::vec3> calculateChildrenMinMax() const;

	// prints a nodes with a given depth
	template<typename TFunc>
	void print(std::ostream& ost, size_t const depth, size_t const height, TFunc func) const;

	AABBTree<T>::Node * mParrent = nullptr;
	sptr mLeft = nullptr;
	sptr mRight = nullptr;
	bool mIsLeft = true;	// if it is left or right inside the tree

	glm::vec3 mMin = {};  // Minimum Edge of the bounding box in world coordinates
	glm::vec3 mMax = {};  // Maximum Edge of the bounding box in world coordinates

	
};
















// #######+++++++ Implementation +++++++#######

#include <cassert>
#include <algorithm>
#include <iomanip>
#include <functional>

template<typename T>
inline bool AABBTree<T>::Node::insertRecursive(sptr& leef)
{
	if (leef == nullptr) {
		return true;
	}

	leef->mParrent = this;
	if (mLeft == nullptr && mRight == nullptr) {
		mMin = leef->mMin;
		mMax = leef->mMax;
	}
	else
	{
		mMin = glm::min(mMin, leef->mMin);
		mMax = glm::max(mMax, leef->mMax);
	}



	// insert into existing node
	if (mLeft == nullptr) 
	{	// left node is free
		mLeft = leef;
	}
	else if (mRight == nullptr) 
	{	// right node is free
		mRight = leef;
	}
	else if (mLeft->volume(*leef) <= mRight->volume(*leef)) 
	{	// left node results in a smaller area
		if (!mLeft->insert(leef)) 
		{	// left node in the tree is already a leef. Create a new node
			auto left = mLeft;
			mLeft = std::make_shared<Node>();
			mLeft->insert(left);
			mLeft->insert(leef);
		}
	}
	else 
	{	// right node results in a smaller area
		if (!mRight->insert(leef))
		{	// left node in the tree is already a leef. Create a new node
			auto right = mRight;
			mRight = std::make_shared<Node>();
			mRight->insert(right);
			mRight->insert(leef);
		}
	}

	return true;
}

template<typename T>
inline bool AABBTree<T>::Node::insert(sptr leef)
{
	return insertRecursive(leef);
}

template<typename T>
inline std::shared_ptr<T> AABBTree<T>::Node::checkCollision(Node const& leef) const
{
	return std::shared_ptr<T>();
}

template<typename T>
inline size_t AABBTree<T>::Node::size() const
{
	size_t size = 1;
	if (mLeft != nullptr) {
		size += mLeft->size();
	}
	if (mRight != nullptr) {
		size += mRight->size();
	}

	return size;
}



template<typename T>
inline size_t AABBTree<T>::Node::height() const
{
	size_t height = 1;
	size_t heightLeft  = mLeft  != nullptr ? mLeft->height()  : 0;
	size_t heightRight = mRight != nullptr ? mRight->height() : 0;
	height += std::max(heightLeft, heightRight);

	return height;
}

template<typename T>
inline typename AABBTree<T>::Node::sptr AABBTree<T>::Node::update(Node* child)
{
	auto [min, max] = calculateChildrenMinMax();

	if (min != mMin || max != mMax) 
	{	// Node is outdated
		mMin = min;
		mMax = max;

		// check for collision
		if (mLeft != nullptr && mRight != nullptr) {
			if (!mLeft->isOutside(*mRight)) {
				updateParrent();

				// returns the collision node
				if (mLeft.get() != child)
				{	// Left is other node
					return mLeft;
				}
				else 
				{  // right is other node
					return mRight;
				}
			}
		}

		return updateParrent();
	}
	else {
		return nullptr;
	}
}

template<typename T>
inline typename AABBTree<T>::Node::sptr AABBTree<T>::Node::updateParrent()
{
	if (mParrent != nullptr) {
		return mParrent->update(this);
	}
	else {
		return nullptr;
	}
}

template<typename T>
inline typename AABBTree<T>::Node::sptr AABBTree<T>::Node::remove()
{
	if (mParrent == nullptr) {
		return nullptr;
	}

	AABBTree<T>::Node::sptr thisNode = nullptr;
	if (mParrent->mLeft.get() == this) 
	{	// this is left
		assert(mParrent->mLeft.get() == this);
		thisNode = mParrent->mLeft;
		mParrent->mLeft = nullptr;
	}
	else
	{	// this is right
		assert(mParrent->mRight.get() == this);
		thisNode = mParrent->mRight;
		mParrent->mRight = nullptr;
	}

	mParrent->update(nullptr);
	mParrent = nullptr;

	return thisNode;	// return as shared pointer to not yet kill this object
}

template<typename T>
inline glm::vec3 AABBTree<T>::Node::getMin() const
{
	return mMin;
}

template<typename T>
inline glm::vec3 AABBTree<T>::Node::getMax() const
{
	return mMax;
}

template<typename T>
inline void AABBTree<T>::Node::setMin(glm::vec3 const& min)
{
	mMin = min;
}

template<typename T>
inline void AABBTree<T>::Node::setMax(glm::vec3 const& max)
{
	mMax = max;
}

template<typename T>
inline float AABBTree<T>::Node::volume(Node const& other) const
{
	auto const min = glm::min(mMin, other.mMin);
	auto const max = glm::max(mMax, other.mMax);

	auto const d = min - max;
	auto const V = std::abs(d.x * d.y * d.z);

	return V;
}

template<typename T>
inline bool AABBTree<T>::Node::isInside(Node const& other) const
{
	return isInside(other.mMin, other.mMax);
}

template<typename T>
inline bool AABBTree<T>::Node::isOutside(Node const& other) const
{
	return isOutside(other.mMin, other.mMax);
}

template<typename T>
inline bool AABBTree<T>::Node::isInside(glm::vec3 const& min, glm::vec3 const& max) const
{
	return  min.x >= mMin.x &&
		    min.y >= mMin.y &&
		    min.z >= mMin.z &&
		    max.x <= mMax.x &&
		    max.y <= mMax.y &&
		    max.z <= mMax.z;
}

template<typename T>
inline bool AABBTree<T>::Node::isOutside(glm::vec3 const& min, glm::vec3 const& max) const
{
	return  max.x < mMin.x ||
		    max.y < mMin.y ||
		    max.z < mMin.z ||
		    min.x > mMax.x ||
		    min.y > mMax.y ||
		    min.z > mMax.z;
}

template<typename T>
inline typename AABBTree<T>::Node::sptr AABBTree<T>::Node::getSmallesVolumeNode(Node& node)
{
	if (mLeft == nullptr) {
		return mRight;
	}
	else if (mRight == nullptr) {
		return mLeft;
	}
	else {
		if (mLeft->volume(node) < mRight->volume(node)) {
			return mLeft;
		}
		else {
			return mRight;
		}
	}
}

template<typename T>
inline void AABBTree<T>::Node::reinsertParrent(Node& node)
{
	if (mParrent != nullptr && mParrent->mLeft != nullptr && mParrent->mRight != nullptr) {
		auto smallerNode = mParrent->getSmallesVolumeNode(node);
		if (smallerNode.get() != &node) 
		{	// reinsert node
			auto sptrNode = node.remove();
			mParrent->insert(sptrNode);
		}
	}
}


template<typename T>
inline std::tuple<glm::vec3, glm::vec3> AABBTree<T>::Node::calculateChildrenMinMax() const
{
	glm::vec3 min = {};
	glm::vec3 max = {};

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

	return { min, max };
}



template<typename T>
template<typename TFunc>
inline void AABBTree<T>::Node::getAllMinMax(TFunc func) const
{
	func(mMin, mMax);

	if (mLeft != nullptr) {
		mLeft->getAllMinMax(func);
	}
	if (mRight != nullptr) {
		mRight->getAllMinMax(func);
	}
}


size_t PotenzSum(size_t const base, size_t const exponent) 
{
	if (exponent == 0) {
		return 0;
	}

	size_t sum = 1;
	size_t potenz = 1;


	for (size_t i = 0; i < exponent-1; ++i) {
		potenz *= base;
		sum += potenz;
	}

	return sum;
}


template<typename T>
inline void AABBTree<T>::Node::print(std::ostream& ost) const
{
	size_t const dataWidth = 8;
	std::string const space(dataWidth/2, ' ');

	size_t const h = height();



	for (size_t i = 0; i < h; ++i)
	{
		for (size_t j = 0; j < PotenzSum(2, (h - i-1)); ++j) {
			ost << space;
		}

		print(ost, i, h, &AABBTree<T>::Node::getMin);
		ost << std::endl;

		for (size_t j = 0; j < PotenzSum(2, (h - i - 1)); ++j) {
			ost << space;
		}

		print(ost, i, h, &AABBTree<T>::Node::getMax);
		ost << std::endl;

		ost << std::endl;
	}

}

template<typename T>
template<typename TFunc>
inline void AABBTree<T>::Node::print(std::ostream& ost, size_t const depth, size_t const height, TFunc func) const
{
	size_t const dataWidth = 8;
	std::string const space(dataWidth, ' ');

	if (depth == 0) {
		auto memberfunc = std::bind(func, this);

		ost << " " << memberfunc(this).x << " " << memberfunc(this).y << " " << memberfunc(this).z << "  ";
		return;
	}
	else if (depth == 1) {
		if (mLeft == nullptr) {
			ost << space;
		}
		if (mRight == nullptr) {
			ost << space;
		}
	}


	if (mLeft != nullptr) {
		mLeft->print(ost, depth - 1, height - 1, func);
	}
	if (depth == 1) {
		for (size_t j = 0; j < PotenzSum(2, (height - depth - 1)); ++j) {
			ost << space;
		}
	}

	if (mRight != nullptr) {
		mRight->print(ost, depth - 1, height - 1, func);
	}

}
