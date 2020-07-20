///////////////////////////////////////////////////////////////////////////////
// File:		 main.cpp
// Revision:	 1.0
// Date:		 12.07.2020
// Author:		 Christian Steinbrecher
// Description:  Renders a demo tree
///////////////////////////////////////////////////////////////////////////////


#include "AABBTree.h"
//#include "AABBTreeNode.h"

#include <iostream>

using namespace std;

class A;
class B;
class C;


class CollisionHandler
{
public:
	virtual void handleCollision(CollisionHandler& other) = 0;

	virtual void handleCollision(A & other) = 0;
	virtual void handleCollision(B & other) = 0;
	virtual void handleCollision(C & other) = 0;
};

using MyAABBTree = AABBTree<CollisionHandler>;


class A : public CollisionHandler
{
public:
	virtual void handleCollision(CollisionHandler& other) {
		other.handleCollision(*this);
	}

	virtual void handleCollision(A& other) override {

	}
	virtual void handleCollision(B& other) override {

	}
	virtual void handleCollision(C& other) override {

	}

private:

};



int main()
{
	MyAABBTree::Node root;

	auto a = std::make_shared<A>();
	auto leef = root.createLeef(a);

	leef.remove();


	return 0;
}