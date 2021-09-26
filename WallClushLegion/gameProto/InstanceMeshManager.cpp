#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "InstanceBase.h"

const unsigned int InstanceMeshManager::mMarixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(InstanceBase* instance,unsigned int maxInstance)
	: mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceBase::InstanceType::InstanceTypeNum))
	, mMaxInstance(maxInstance)
{
	// インスタンスシェーダー読み込み
	mInstanceShader = new Shader();
	mInstanceShader->Load("Shaders/InstanceMesh.vert", "Shaders/InstanceMesh.frag");
	mInstances.push_back(instance);

	// 頂点配列の準備
	PreparationVAO();
	SetInstanceMesh();
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh()
{
	for (int i = 0; i < mInstances.size(); i++)
	{
		mVAO = mInstances[i]->GetMesh()->GetVertexArray()->GetVAO();
		mIndexNum = mInstances[i]->GetMesh()->GetVertexArray()->GetNumIndices();

		glGenBuffers(1, &mInstanceVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);

		glBufferData(GL_ARRAY_BUFFER, mMaxInstance * mMatrix4Size, mBufferMatrices, GL_DYNAMIC_DRAW);
		glBindVertexArray(mVAO);

		// 頂点アトリビュートに行列をセット
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(1 * sizeof(float) * 4));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(2 * sizeof(float) * 4));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(3 * sizeof(float) * 4));

		// 引数１：頂点配列のインデックスを指定
		// 引数２：スロットでの汎用属性の更新分で渡されるインスタンスの数を指定する
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
}

// VAOの準備
void InstanceMeshManager::PreparationVAO()
{
	// メッシュがなければ
	//if (!mInstances.empty()) return;
	mBufferMatrices = new float[mMaxInstance * mMarixElemNum];

	// インスタンス種類の総数
	for (int i = 0; i < mMaxInstance; i++)
	{
		Matrix4 mat = mInstances[i]->GetWorldTransform();

		// 行列の行と列を転置する
		mat.Transpose();
		memcpy(&(mInstances[mMarixElemNum]), mat.GetAsFloatPtr(), mMatrix4Size);
	}

	// 行列バッファにコピー
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mMaxInstance * mMatrix4Size, &mBufferMatrices[0]);
}

void InstanceMeshManager::SetShader()
{
	// ビュー、プロジェクション行列
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view*proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// mInstances.size()が合ってるか不明
	for (int i = 0; i < mMaxInstance; i++)
	{
		// テクスチャのセット
		mTexture = mInstances[i]->GetMesh()->GetTexture(0);
		if (mTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// インスタンシング描画
		glBindVertexArray(mVAO);
		glDrawElementsInstanced(GL_TRIANGLES, mIndexNum, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mMaxInstance));
		glBindVertexArray(0);
	}
}
