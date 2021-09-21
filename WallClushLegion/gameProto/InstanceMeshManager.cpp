#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(Mesh* mesh,unsigned int maxInstanceNum)
	: mMaxInstance(maxInstanceNum)
	, mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
	, mMesh(mesh)
{
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh(Mesh* mesh, unsigned int maxInstanceNum)
{
	// インスタンスの頂点配列を生成
	glGenBuffers(1, &mInstanceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);

	// 行列バッファをInstanceVAOとバインド
	glBufferData(GL_ARRAY_BUFFER, mMatrix4Size * mMaxInstance,/*行列バッファの先頭データ*/ , GL_STATIC_DRAW);
	glBindVertexArray(mesh->GetVertexArray()->GetNumVerts()/*メッシュVAO*/);

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

// VAOの準備
void InstanceMeshManager::PreparationVAO()
{
	// インスタンスタイプの総数
	const int instanceTypeNum = static_cast<int>(InstanceType::InstanceTypeNum);
	for (int i=0;i<instanceTypeNum;i++)
	{
		int num = 0;
		for (int i=0;i<)
		{

		}
	}
}

void InstanceMeshManager::SetShader()
{
	// ビュー、プロジェクション行列
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	// インスタンスシェーダー
	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view * proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// 全種類のインスタンス描画
	for (int i = 0; i < mInstanceTypeNum; i++)
	{
		// テクスチャのセット
		Texture* t = mMesh->GetTexture(0);
		if (t)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// インスタンシング描画
		glBindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES,,GL_UNSIGNED_INT,0,static_cast<GLsizei>());
		glBindVertexArray(0);
	}
}
