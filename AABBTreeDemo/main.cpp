///////////////////////////////////////////////////////////////////////////////
// File:		 main.cpp
// Revision:	 1.0
// Date:		 25.07.2020
// Author:		 Christian Steinbrecher
// Description:  Creates some elements and collides them
///////////////////////////////////////////////////////////////////////////////


#include <ParticleRenderer.h>
#include <TwoVertexCubeShader.h>
#include <DynamicPointRenderObject.h>

#include <AABBTree.h>

#include <random>



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

	void update(float const t) {
		mColor = { 0, 0.5f, 0 };

		if (mLeef == nullptr) {
			return;
		}

		glm::vec3 distance = mPos2 - mPos1;
		glm::vec3 pos = mPos1 + sin(t) * distance;

		mLeef->setMin(pos - size);
		mLeef->setMax(pos + size);

		auto collidingObject = mLeef->update();
		if (collidingObject != nullptr)
		{
			mColor = { 0.5f, 0, 0 };
			collidingObject->handleCollision(*this);
		}
	}

	void setPos1(glm::vec3 const& pos) {
		mPos1 = pos;
	}
	glm::vec3 getPos1() const {
		return mPos1;
	}

	void setPos2(glm::vec3 const& pos) {
		mPos2 = pos;
	}
	glm::vec3 getPos2() const {
		return mPos2;
	}

	void setColor(glm::vec3 const& color) {
		mColor = color;
	}
	glm::vec3 getColor() const {
		return mColor;
	}

	void setAABBTreeLeef(MyAABBTree::Leef::sptr leef) {
		mLeef = std::move(leef);
		
		mLeef->setMin(mPos1 - size);
		mLeef->setMax(mPos1 + size);
	}

private:
	glm::vec3 mPos1 = {};
	glm::vec3 mPos2 = {};
	glm::vec3 mColor = { 0.5f, 0.5f, 0.5f };

	glm::vec3 const size = { 0.5f, 0.5f, 0.5f };

	 MyAABBTree::Leef::sptr mLeef;
};


class Rand
{
public:
	float operator()() {
		return floor(distribution(generator));
	}

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(-40.0f, 40.0f);
};


int main()
{
	MyAABBTree::Node root;

	std::vector<Object1::sptr> obects;

	Rand rand;
	for (size_t i = 0; i < 40000; ++i) {
		auto obj1 = std::make_shared<Object1>();
		obj1->setPos1({ rand(), rand(), rand() });
		obj1->setPos2({ rand(), rand(), rand() });
		
		auto leef = std::make_shared<MyAABBTree::Leef>(obj1);
		obj1->setAABBTreeLeef(leef);
		root.insert(leef);

		obects.push_back(std::move(obj1));
	}
	/*{

		auto obj1 = std::make_shared<Object1>();
		obj1->setPos1({ 0, 0, 0 });
		obj1->setPos2({ 1, 1, 1 });

		auto leef = std::make_shared<MyAABBTree::Leef>(obj1);
		obj1->setAABBTreeLeef(leef);
		root.insert(leef);

		obects.push_back(std::move(obj1));
	}
	{

		auto obj1 = std::make_shared<Object1>();
		obj1->setPos1({ -3, -3, -3 });
		obj1->setPos2({ -2, -2, -2 });

		auto leef = std::make_shared<MyAABBTree::Leef>(obj1);
		obj1->setAABBTreeLeef(leef);
		root.insert(leef);

		obects.push_back(std::move(obj1));
	}
	{

		auto obj1 = std::make_shared<Object1>();
		obj1->setPos1({ 3, 3, 3 });
		obj1->setPos2({ -4, -4, -3 });

		auto leef = std::make_shared<MyAABBTree::Leef>(obj1);
		obj1->setAABBTreeLeef(leef);
		root.insert(leef);

		obects.push_back(std::move(obj1));
	}*/



	using ShaderType = TwoVertexCubeShader;
	using RenderObj = DynamicPointRenderObject<ShaderType>;
	using Vertices = std::vector<RenderObj::Vertex>;


	auto app = ParticleRenderer::createVulkan();
	auto obj = RenderObj::createVulkan();

	Vertices vertices(root.size()+10000);

	for (auto& elem : obects) {
			elem->update(0);

	}

	
	float t = 0;
	auto lbd = [&]()
	{
		for (auto& elem : obects) {
			//if (t > 1) {
				elem->update(t);
			//}
			
		}
		t += 0.0001f;

		size_t const rooSize = root.size();
		size_t const verticesSize = vertices.size();
		assert(rooSize <= verticesSize);
		size_t i = 0;
		root.getAllMinMax([&](glm::vec3 const& min, glm::vec3 const& max) {
			vertices[i].pos1 = min;
			vertices[i].pos2 = max;
			vertices[i].color = { 0.5f, 0.5f, 0.5f };
			++i;
			});
		for (; i < vertices.size(); ++i) {
			vertices[i].pos1 = { 0.0f, 0.0f, 0.0f };
			vertices[i].pos2 = { 0.0f, 0.0f, 0.0f };
			vertices[i].color = { 0.5f, 0.5f, 0.5f };
		}

		float const x =  0 + 160* sin(t*0.7);
		float const y =  0 + 160* cos(t*0.7);
		float const z = 40.0f;

		app->setView({ x, y, z }, { 0, 0, 0 });

		return vertices;
	};

	obj->setVertices(lbd, vertices.size());
	obj->setPosition({ 0.0f, 0.0f, 0.0f });

	app->add(std::move(obj));
	app->setView({ 15.0, 5.0f, 4.0f });


	app->run();


	return 0;
}