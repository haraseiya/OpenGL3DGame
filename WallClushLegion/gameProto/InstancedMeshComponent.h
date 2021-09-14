#pragma once
#include "Component.h"
#include "Math.h"

class GameObject;
class Shader;
class Mesh;
class VertexArray;

class InstancedMeshComponent : public Component
{
public:
	InstancedMeshComponent(GameObject* owner,Tag tag,const int& amount);
	~InstancedMeshComponent();

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
};