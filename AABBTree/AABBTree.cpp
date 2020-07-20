///////////////////////////////////////////////////////////////////////////////
// File:		 AABBTree.cpp
// Revision:	 1.0
// Date:		 12.07.2020
// Author:		 Christian Steinbrecher
// Description:  Axis Aligned Bounding Box Tree
///////////////////////////////////////////////////////////////////////////////
//
//#include "AABBTree.h"
//
//#include <tuple>
//#include <cassert>
//
//
//
//AABBTree::Leef::Leef(std::shared_ptr<ICollisionHandler> obj)
//    : mObj(std::move(obj))
//{
//    calcAbs();
//}
//
//void AABBTree::Leef::setPosition(glm::vec3 const& pos)
//{
//    mPos = pos;
//    calcAbs();
//}
//
//void AABBTree::Leef::setBoundingBox(glm::vec3 const& min, glm::vec3 const& max)
//{
//    mMin = glm::min(min, max);
//    mMax = glm::max(min, max);
//    calcAbs();
//}
//
//void AABBTree::Leef::remove()
//{
//}
//
//void AABBTree::Leef::insert(Leef & leef)
//{
//    assert(mParrent != nullptr);
//
//    // create new node
//    auto node = new Node();
//    node->setParrent(mParrent);
//
//    if (this == mParrent->getLeft()) {
//        mParrent->setLeft(*node);
//    }
//    else {
//        assert(this == mParrent->getRight());
//        mParrent->setRight(*node);
//    }
//
//    node->insert(*this);
//    node->insert(leef);
//}
//
//void AABBTree::Leef::calcAbs()
//{
//    assert(mMin.x <= mMax.x);
//    assert(mMin.y <= mMax.y);
//    assert(mMin.z <= mMax.z);
//
//    mAbsMin = mPos + mMin;
//    mAbsMax = mPos + mMax;
//}
//
//
//AABBTree::Leef & AABBTree::Node::createLeef(std::shared_ptr<ICollisionHandler> obj, 
//    glm::vec3 const& pos, glm::vec3 const& min, glm::vec3 const& max)
//{
//    auto leef = new Leef(obj);
//    leef->setPosition(pos);
//    leef->setBoundingBox(min, max);
//
//    insert(*leef);
//
//    return *leef;
//}
//
//void AABBTree::Node::insert(Leef & leef)
//{
//    leef.setParrent(this);
//    mAbsMin = glm::min(mAbsMin, leef.getMin());
//    mAbsMax = glm::max(mAbsMax, leef.getMax());
//
//    // insert into existing node
//	if (mLeft == nullptr) {
//		mLeft = &leef;
//        return;
//	}
//	else if (mRight == nullptr) {
//		mRight = &leef;
//        return;
//	}
//	else if (mLeft->volume(leef) <= mRight->volume(leef)) {
//        mLeft->insert(leef);
//	}
//	else {
//        mRight->insert(leef);
//	}
//}
//
//void AABBTree::BaseNode::setParrent(Node* parrent)
//{
//    mParrent = parrent;
//}
//
//float AABBTree::BaseNode::volume(BaseNode const& other) const
//{
//    auto const min = glm::min(mAbsMin, other.getMin());
//    auto const max = glm::max(mAbsMax, other.getMax());
//    
//    auto const d = min - max;
//    auto const A = d.x * d.y * d.z;
//
//    return A;
//}
//
//glm::vec3 AABBTree::BaseNode::getMin() const
//{
//    return mAbsMin;
//}
//
//glm::vec3 AABBTree::BaseNode::getMax() const
//{
//    return mAbsMax;
//}
