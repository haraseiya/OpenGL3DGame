#pragma once
#include "Component.h"
#include "Math.h"

#include <vector>

class GameObject;
class Shader;
class Mesh;
class VertexArray;
class InstanceMeshManager;

class InstanceMeshComponent : public Component
{
public:
	InstanceMeshComponent(GameObject* owner,Mesh* mesh,const int& amount);
	~InstanceMeshComponent();

	void Update(float deltaTime) override;
	void Draw(Shader* shader);

private:
	//unsigned int mBuffer;

	static const int mAmount;
	std::vector<GameObject*> mModelMatrices;

	Mesh* mMesh;
	VertexArray* mVertexArray;
	GameObject* mOwner;

	Shader* mShader;
	InstanceMeshManager* mInstanceManager;
};