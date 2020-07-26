///////////////////////////////////////////////////////////////////////////////
// File:		 test.cpp
// Revision:	 1.0
// Date:		 19.07.2020
// Author:		 Christian Steinbrecher
// Description:  Test of AABBTree
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include "Object1.h"

using namespace std;

TEST(AABBTree, Insert) {

	auto obj1 = std::make_shared<Object1>();

	MyAABBTree::Node root;

	auto leef1 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef1->setMin({ 1, 0, 0 });
	leef1->setMax({ 2, 1, 1 });

	auto leef2 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef2->setMin({ 2, 0, 0 });
	leef2->setMax({ 3, 1, 1 });

	auto leef3 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef3->setMin({ 3, 0, 0 });
	leef3->setMax({ 4, 1, 1 });

	auto leef4 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef4->setMin({ 4, 0, 0 });
	leef4->setMax({ 5, 1, 1 });

	auto leef5 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef5->setMin({ 6, 0, 0 });
	leef5->setMax({ 7, 1, 1 });

	auto leef6 = std::make_shared<MyAABBTree::Leef>(obj1);
	leef6->setMin({ 8, 0, 0 });
	leef6->setMax({ 9, 1, 1 });

	root.insert(leef1);
	root.insert(leef2);
	root.insert(leef3);
	//root.insert(leef4);
	//root.insert(leef5);
	//root.insert(leef6);

	root.print(cout);

	//size_t i = 0;
	//root.getAllMinMax([&](glm::vec3 const& min, glm::vec3 const& max) 
	//	{
	//		switch(i) {
	//			case 0:
	//				EXPECT_EQ(min, glm::vec3( 0,0,0 ));
	//				EXPECT_EQ(max, glm::vec3(3, 1, 1));
	//				break;
	//			case 1:
	//				EXPECT_EQ(min, glm::vec3(1, 0, 0));
	//				EXPECT_EQ(max, glm::vec3(2, 1, 1));
	//				break;
	//			case 2:
	//				EXPECT_EQ(min, glm::vec3(1, 0, 0));
	//				EXPECT_EQ(max, glm::vec3(3, 1, 1));
	//				break;
	//			case 3:
	//				EXPECT_EQ(min, glm::vec3(1, 0, 0));
	//				EXPECT_EQ(max, glm::vec3(2, 1, 1));
	//				break;
	//			case 4:
	//				EXPECT_EQ(min, glm::vec3(2, 0, 0));
	//				EXPECT_EQ(max, glm::vec3(3, 1, 1));
	//				break;
	//			default:
	//				EXPECT_EQ(true, false);
	//				break;
	//		};
	//		++i;
	//	});
}