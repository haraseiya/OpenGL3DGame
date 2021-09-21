#include "InstancedMeshComponent.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// インスタンスオブジェクトの総数
const int InstancedMeshComponent::mAmount = 100;

InstancedMeshComponent::InstancedMeshComponent(GameObject* owner,Tag tag, const int& amount)
	: Component(owner)
	, mOwner(owner)
{
	// レンダークラスにインスタンスメッシュコンポーネントを追加
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);

	// ゲームオブジェクト総数分のメモリ確保
	mModelMatrices.reserve(mAmount);

	//// インスタンス配列生成
	//glGenBuffers(1,&mBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	//glBufferData(GL_ARRAY_BUFFER, mAmount * sizeof(GameObject), &mModelMatrices[0], GL_STATIC_DRAW);

	//// 頂点配列をアクティブに
	//mVertexArray = mMesh->GetVertexArray();
	//mVertexArray->SetActive();
}

InstancedMeshComponent::~InstancedMeshComponent()
{
	// インスタンスメッシュコンポーネントを破棄
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
}

void InstancedMeshComponent::Update(float deltaTime)
{
	// 頂点配列を取得
	mVertexArray = mMesh->GetVertexArray();
	mVertexArray->SetActive();

	// 頂点数分配列を回す
	for (unsigned int i = 0; i < mVertexArray->GetNumIndices(); i++)
	{
		glDrawElementsInstanced(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, 0, mAmount);
	}
}

void InstancedMeshComponent::Draw(Shader* shader)
{
	// メッシュが存在しなければ返す
	if (!mMesh) return;

	// ワールド変換をセット
	shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

	// スペキュラ強度セット
	shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

	// ルミナンス強度セット
	shader->SetFloatUniform("uLuminance", mMesh->GetLuminace());
}
