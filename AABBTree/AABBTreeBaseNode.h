///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTreeBaseNode.cpp
// Revision:	 1.0
// Date:		 19.07.2020
// Author:		 Christian Steinbrecher
// Description:  Abstract base node class
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AABBTree.h"

#include <glm/glm.hpp>
#include <memory>

template<typename T>
class AABBTree<T>::BaseNode {
public:
	// D-Tor
	virtual ~BaseNode();

	void setParrent(Node* parrent);

	virtual void insert(Leef& leef) = 0;
	virtual Leef * checkCollision(Leef & leef) = 0;

	float volume(BaseNode const& other) const;
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;

protected:

	bool isInside(BaseNode const& other) const;
	bool isOutside(BaseNode const& other) const;

	bool isInside(glm::vec3 const& min, glm::vec3 const& max) const;
	bool isOutside(glm::vec3 const& min, glm::vec3 const& max) const;
	
	Node* mParrent = nullptr;
	bool mIsLeft = true;	// if it is left or right inside the tree

	glm::vec3 mAbsMin = {};  // Minimum Edge of the bounding box in world coordinates
	glm::vec3 mAbsMax = {};  // Maximum Edge of the bounding box in world coordinates
};



// #######+++++++ Implementation +++++++#######

template<typename T>
inline AABBTree<T>::BaseNode::~BaseNode()
{

}

template<typename T>
inline void AABBTree<T>::BaseNode::setParrent(Node* parrent)
{
	mParrent = parrent;
}

template<typename T>
inline float AABBTree<T>::BaseNode::volume(BaseNode const& other) const
{
	auto const min = glm::min(mAbsMin, other.getMin());
	auto const max = glm::max(mAbsMax, other.getMax());

	auto const d = min - max;
	auto const A = d.x * d.y * d.z;

	return A;
}

template<typename T>
inline glm::vec3 AABBTree<T>::BaseNode::getMin() const
{
	return mAbsMin;
}

template<typename T>
inline glm::vec3 AABBTree<T>::BaseNode::getMax() const
{
	return mAbsMax;
}

template<typename T>
inline bool AABBTree<T>::BaseNode::isInside(BaseNode const & other) const
{
	return isInside(other.getMin(), other.getMax());
}

template<typename T>
inline bool AABBTree<T>::BaseNode::isOutside(BaseNode const & other) const
{
	return isOutside(other.getMin(), other.getMax());
}

template<typename T>
inline bool AABBTree<T>::BaseNode::isInside(glm::vec3 const& min, glm::vec3 const& max) const
{
	return  min.x >= mAbsMin.x &&
		    min.y >= mAbsMin.y &&
		    min.z >= mAbsMin.z &&
		    max.x <= mAbsMax.x &&
		    max.y <= mAbsMax.y &&
		    max.z <= mAbsMax.z;
}

template<typename T>
inline bool AABBTree<T>::BaseNode::isOutside(glm::vec3 const& min, glm::vec3 const& max) const
{
	return  max.x < mAbsMin.x ||
			max.y < mAbsMin.y || 
			max.z < mAbsMin.z ||
			min.x > mAbsMax.x ||
		    min.y > mAbsMax.y ||
		    min.z > mAbsMax.z;
}

