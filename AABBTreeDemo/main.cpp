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

	void update() {
		mColor = { 0, 0.5f, 0 };

		if (mLeef == nullptr) {
			return;
		}

		mLeef->setMin(mPos - size);
		mLeef->setMax(mPos + size);

		auto collidingObject = mLeef->update();
		if (collidingObject != nullptr)
		{
			mColor = { 0.5f, 0, 0 };
			collidingObject->handleCollision(*this);
		}
	}

	void setPos(glm::vec3 const& pos) {
		mPos = pos;

		update();
	}
	glm::vec3 getPos() const {
		return mPos;
	}

	void setColor(glm::vec3 const& color) {
		mColor = color;
	}
	glm::vec3 getColor() const {
		return mColor;
	}

	void setAABBTreeLeef(MyAABBTree::Leef::sptr leef) {
		mLeef = std::move(leef);
		
		mLeef->setMin(mPos - size);
		mLeef->setMax(mPos + size);
	}

private:
	glm::vec3 mPos = {};
	glm::vec3 mColor = { 0.5f, 0.5f, 0.5f };

	glm::vec3 const size = { 0.5f, 0.5f, 0.5f };

	 MyAABBTree::Leef::sptr mLeef;
};



int main()
{
	MyAABBTree::Node root;

	std::vector<Object1::sptr> obj1s;

	for (size_t i = 0; i < 6; ++i) {
		auto obj1 = std::make_shared<Object1>();
		obj1->setPos({ i * 0.25f, i *0.5f, i });
		auto leef = std::make_shared<MyAABBTree::Leef>(obj1);
		obj1->setAABBTreeLeef(leef);
		root.insert(leef);

		obj1s.push_back(std::move(obj1));
	}

	using ShaderType = TwoVertexCubeShader;
	using RenderObj = DynamicPointRenderObject<ShaderType>;
	using Vertices = std::vector<RenderObj::Vertex>;


	auto app = ParticleRenderer::createVulkan();
	auto obj = RenderObj::createVulkan();

	Vertices vertices(root.size());

	

	auto lbd = [&]()
	{
		size_t const rooSize = root.size();
		size_t const verticesSize = vertices.size();
		assert(rooSize == verticesSize);
		size_t i = 0;
		root.getAllMinMax([&](glm::vec3 const& min, glm::vec3 const& max) {
			vertices[i].pos1 = min;
			vertices[i].pos2 = max;
			vertices[i].color = { 0.5f, 0.5f, 0.5f };
			++i;
			});

		return vertices;
	};

	obj->setVertices(lbd, vertices.size());
	obj->setPosition({ 0.0f, 0.0f, 0.0f });

	app->add(std::move(obj));
	app->setView({ 15.0, 5.0f, 4.0f });


	app->run();


	return 0;
}