#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "InstanceBase.h"

const unsigned int InstanceMeshManager::mMarixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(std::vector<InstanceBase*> instances,unsigned int maxInstanceNum)
	: mMaxInstance(maxInstanceNum)
	, mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
{
	// インスタンスシェーダー読み込み
	mInstanceShader = new Shader();
	mInstanceShader->Load("Shaders/InstanceMesh.vert", "Shaders/InstanceMesh.frag");

	// 行列バッファの作成
	mBufferMatrices = new Matrix4[mMaxInstance * mMarixElemNum];

	for (int i = 0; i < instances.size(); i++)
	{
		Matrix4 model=Matrix4::Identity;
		mInstances = instances;
		model = mInstances[i]->GetWorldTransform();
		mBufferMatrices[i] = model;
	}

	// インスタンスの頂点配列を生成
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMaxInstance * mMatrix4Size, &mBufferMatrices[0], GL_STATIC_DRAW);

	for (int i = 0; i < mInstances.size(); i++)
	{
		// VAOの取得
		glBindVertexArray(mInstances[i]->GetMesh()->GetVertexArray()->GetVAO());

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

	//// セット
	//SetInstanceMesh();
	//PreparationVAO();
	//SetShader();
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh()
{
}

//// VAOの準備
//void InstanceMeshManager::PreparationVAO()
//{
//	// インスタンス種類の総数
//	const int instanceTypeNum = static_cast<int>(InstanceType::InstanceTypeNum);
//	for (int i=0;i<instanceTypeNum;i++)
//	{
//		int num = 0;
//		for (auto g : mInstance)
//		{
//			Matrix4 mat = g->GetWorldTransform();
//			
//			// 行列の行と列を転置する
//			mat.Transpose();
//			memcpy(&(mInstance[num*mMarixElemNum]),mat.GetAsFloatPtr(),mMatrix4Size);
//
//
//			// オブジェクトナンバー
//			++num;
//		}
//	}
//
//	// インスタンス種類ごとに行列バッファをコピー
//	for (int i = 0; i < instanceTypeNum; i++)
//	{
//		// 行列バッファにコピー
//		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, mMaxInstance * mMatrix4Size, &mModel[0]);
//	}
//}

void InstanceMeshManager::SetShader()
{
	// ビュー、プロジェクション行列
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view*proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// インスタンス描画
	for (int i = 0; i < mInstances.size(); i++)
	{
		// テクスチャのセット
		mTexture = mInstances[i]->GetMesh()->GetTexture(0);
		if (mTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// インスタンシング描画
		glBindVertexArray(mInstances[i]->GetMesh()->GetVertexArray()->GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, mInstances[i]->GetMesh()->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mMaxInstance));
		glBindVertexArray(0);
	}
}
