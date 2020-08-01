///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTreeMNode.h
// Revision:	 1.0
// Date:		 01.08.2020
// Author:		 Christian Steinbrecher
// Description:  A node of an AABBTreeM
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
	virtual std::shared_ptr<T> checkCollision(Node const& leef) const;

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
	sptr getSmallesVolumeNode(Node& node);



private:
	std::tuple<glm::vec3, glm::vec3> calculateChildrenMinMax() const;

	// prints a nodes with a given depth
	template<typename TFunc>
	void print(std::ostream& ost, size_t const depth, size_t const height, TFunc func) const;

	AABBTree<T>::Node* mParrent = nullptr;
	sptr mLeft = nullptr;
	sptr mRight = nullptr;
	bool mIsLeft = true;	// if it is left or right inside the tree

	glm::vec3 mMin = {};  // Minimum Edge of the bounding box in world coordinates
	glm::vec3 mMax = {};  // Maximum Edge of the bounding box in world coordinates


};


