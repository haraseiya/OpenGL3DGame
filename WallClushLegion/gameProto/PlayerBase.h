#pragma once

#include "GameObject.h"
#include "BoxCollider.h"

// �v���C���[�N���X���enum ��ԂɕύX����������statePlayerName���ύX
enum class PlayerStateEnum : unsigned char
{
	Invalid,    // �����ȃX�e�[�g
	Spawn,      // �o��
	Idle,       // �ҋ@���
	Walk,       // ����
	Turn,       // �^�[��
	Run,        // ����
	LookAround, // ����
	Attack1,    // �U���P
	Attack2,    // �U���Q
	Attack3,    // �U���R
	GetDamage,  // �_���[�W�󂯂�
	Roar,       // �i����
	Stun,       // �C��
	Die,        // ���S
	StateNum	// ��Ԃ̑���
};

// �V�[�����Ƃ̃v���C���[
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,
	PLAYER_GAMESCENE,
	PLAYER_RESULT,
};

// �v���C���[�̏��
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,   // �ҋ@
	PLAYER_STATE_RUN,   // ����
	PLAYER_STATE_NUM,   // ���A�j���[�V������
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other) override;

	// �L�������ǂݍ��݊֘A
	virtual void LoadModel() = 0;
	virtual void LoadSkeleton() = 0;
	virtual void LoadAnimation() = 0;

	// �ӂ�܂���ǉ�
	virtual void BehaviorResister() = 0;

	// �����蔻���ǉ�
	virtual void SetCollider() = 0;

	// �X�e�[�g�����擾����w���p�[�֐�
	const char* GetPlayerStateEnumName(PlayerStateEnum state);

protected:
	GameObject* mTarget;
	AABB mPlayerBox;
	Mesh* mMesh;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;			
	EffectComponent* mEffect;

	PlayerState mNowState;        // ���݂̃X�e�[�g
	PlayerState mNextState;       // ���̃X�e�[�g
	std::vector<PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	PlayerBehaviorComponent* mPlayerBehavior;

	Vector3 mVelocityVec;

	static const float mRange;
	static const float mAnimationSpeed;
	float mShootTimer;
	bool mAimMode;
};

