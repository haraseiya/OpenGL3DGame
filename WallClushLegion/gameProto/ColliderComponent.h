#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class BoxCollider;
class WallCollider;

enum class ColliderTypeEnum
{
	Box,
	Wall,
	Sphere,
	Line,
};

class ColliderComponent : public Component
{
public:
	ColliderComponent(class GameObject* owner, ColliderTypeEnum type, int updateOrder = 100);
	virtual ~ColliderComponent() {};

	Tag GetTag();
	void SetInfo(const CollisionInfo& info) { mInfo = info; }
	void ClearInfo();

	ColliderTypeEnum GetColliderType() { return mColliderType; }
	CollisionInfo& GetCollisionInfo() { return mInfo; }

	virtual void OnUpdateWorldTransform() {};

	// Double-Dispatch �p�^�[�����g�p���ďՓ˔���g�ݍ��킹���s��
	virtual bool CollisionDetection(ColliderComponent* other) = 0;

	// Double-Dispatch �p�^�[���FCollider���m�̂ӂ�܂����`
	virtual bool Check(class BoxCollider* other);
	virtual bool Check(class WallCollider* other);

protected:
	Tag              mTag;                // �����蔻��O���[�v�^�O
	ColliderTypeEnum mColliderType;       // �Փ˃^�C�v
	CollisionInfo    mInfo;               // �Փˎ����

	friend class PhysicsWorld;
};