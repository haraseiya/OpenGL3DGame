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
	Vector3 m_offset;	// �e�I�u�W�F�N�g�N���X�Ɖ摜��`�悷��ʒu�̍�
	Vector3 m_color;	// �摜�ɏ�Z����F
	PARTICLE_ENUM m_blendType;	// �u�����h�^�C�v

	float m_scale;		// �傫��
	float m_alpha;		// �����x
	int m_textureID;	// �e�N�X�`��ID
	int m_drawOrder;	// �`�揇
	bool m_reverce;		// ���]���s����
	bool m_visible;		// �`����s����

	static Matrix4 m_billboardMat;
	static Vector3 m_cameraWorldPos;

	// �Q�b�^�[�Z�b�^�[
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
	@brief�@�`������邩�ǂ������擾����
	@return	true : �`�悷�� , false : �`�悵�Ȃ�
	*/
	bool GetVisible() const { return m_visible; }

	/*
	@param _drawOrder �`�揇
	*/
	void SetDrawOrder(int drawOrder) { m_drawOrder = drawOrder; }

	/*
	@return �`�揇
	*/
	int GetDrawOrder() { return m_drawOrder; }

	/*
	@param _offset �e�I�u�W�F�N�g�̍��W�ƕ`��ʒu�̍�
	*/
	void SetOffSet(Vector3 offset) { m_offset = offset; };

	/*
	@param _flag ���]���s����
	*/
	void SetReverce(bool flag) { m_reverce = flag; }
};

