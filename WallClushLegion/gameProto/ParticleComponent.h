#pragma once

#include "Component.h"
#include "Math.h"

class Shader;

class ParticleComponent:public Component 
{
public:
	typedef enum PARTICLE_BLEND_ENUM
	{
		PARTICLE_BLEND_ENUM_ALPHA = 0,
		PARTICLE_BLEND_ENUM_ADD,
		PARTICLE_BLEND_ENUM_MULT
	}PARTICLE_ENUM;

	ParticleComponent(GameObject* oener,const Vector3& offset = Vector3(1,1,1),float scale=1.0f,int updateOrder=100);
	virtual ~ParticleComponent();

	virtual void Update(float deltaTime)override{}
	void Draw(Shader* shader);

private:
	Vector3 m_offset;	// 親オブジェクトクラスと画像を描画する位置の差
	Vector3 m_color;	// 画像に乗算する色
	PARTICLE_ENUM m_blendType;	// ブレンドタイプ

	float m_scale;		// 大きさ
	float m_alpha;		// 透明度
	int m_textureID;	// テクスチャID
	int m_drawOrder;	// 描画順
	bool m_reverce;		// 反転を行うか
	bool m_visible;		// 描画を行うか

	static Matrix4 m_billboardMat;
	static Vector3 m_cameraWorldPos;

	// ゲッターセッター
public:
	int GetTextureID() { return m_textureID; }
	void SetTextureID(int texId) { m_textureID = texId; }

	PARTICLE_ENUM GetBlendType() { return m_blendType; }

	void SetColor(const Vector3& color) { m_color = color; }
	void SetAlpha(float alpha) { m_alpha = alpha; }
	void SetScale(float scale) { m_scale = scale; }

	void SetBillboardMat(const Matrix4& mat) { m_billboardMat = mat; }
	void SetBlendMode(PARTICLE_ENUM blendType) { m_blendType = blendType; }

	void SetVisible(bool _visible) { m_visible = _visible; }

	/*
	@brief　描画をするかどうかを取得する
	@return	true : 描画する , false : 描画しない
	*/
	bool GetVisible() const { return m_visible; }

	/*
	@param _drawOrder 描画順
	*/
	void SetDrawOrder(int drawOrder) { m_drawOrder = drawOrder; }

	/*
	@return 描画順
	*/
	int GetDrawOrder() { return m_drawOrder; }

	/*
	@param _offset 親オブジェクトの座標と描画位置の差
	*/
	void SetOffSet(Vector3 offset) { m_offset = offset; };

	/*
	@param _flag 反転を行うか
	*/
	void SetReverce(bool flag) { m_reverce = flag; }
};

