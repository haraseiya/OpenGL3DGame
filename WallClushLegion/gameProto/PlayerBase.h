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
	RunForward, // �O����
	RunRight,	// �E����
	RunLeft,	// ������
	RunBackward,// ��둖��
	LookAround, // ����
	Attack1,    // �U���P
	Attack2,    // �U���Q
	Attack3,    // �U���R
	GetDamage,  // �_���[�W�󂯂�
	Roar,       // �i����
	Stun,       // �C��
	Die,        // ���S
	Hold,		// �e���\����
	StateNum	// ��Ԃ̑���
};

// �V�[�����Ƃ̃v���C���[
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,	// �^�C�g���V�[��
	PLAYER_SELECTSCENE,		// �Z���N�g�V�[��
	PLAYER_GAMESCENE,		// �Q�[���V�[��
	PLAYER_RESULT,			// ���U���g�V�[��
};

// �v���C���[�̏��
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,		// �ҋ@
	PLAYER_STATE_RUN_FORWARD,	// ����
	PLAYER_STATE_RUN_RIGHT,		// �E����
	PLAYER_STATE_RUN_LEFT,		// ������
	PLAYER_STATE_RUN_BACKWARD,	// ��둖��
	PLAYER_STATE_DIE,			// ���S
	PLAYER_STATE_VICTORY,		// �����|�[�Y
	PLAYER_STATE_NUM,			// ���A�j���[�V������
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;
class SceneBase;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;

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

	// HP�̃Q�b�^�[�Z�b�^�[
	int GetHitPoint() { return mHitPoint; }
	void SetHitPoint(int hp) { mHitPoint = hp; }

	bool GetDeadAnimFlag() { return mIsDeadAnimFinish; }
	void SetDeadAnimFlag(bool isDeadAnimFinish) { mIsDeadAnimFinish = isDeadAnimFinish; }

protected:
	GameObject* mTarget;
	AABB mPlayerBox;
	Mesh* mMesh;
	SceneBase* mSceneBase;

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

	PlayerBehaviorComponent* mPlayerBehavior;	// �v���C���[�̂ӂ�܂�

	Vector3 mVelocityVec;

	static const float mRange;
	static const float mAnimationSpeed;
	static const float mInterval;
	static const float mSpecialShotInterval;

	float mShootTimer;
	float mSpecialShotTimer;

	bool mAimMode;

	int mHitPoint;
	bool mIsDeadAnimFinish;
};

