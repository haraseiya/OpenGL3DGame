#pragma once
#include "PlayerBase.h"

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
	PLAYER_STATE_ATTACK,
	PLAYER_STATE_REVIVE,
	PLAYER_STATE_NUM,   // ���A�j���[�V������
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class ChantEffect;

class Player1 : public PlayerBase
{
public:
	Player1();
	~Player1();

	void UpdateActor(float deltaTime) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;

	// �L�������ǂݍ��݊֘A
	void LoadModel()override;
	void LoadSkeleton()override;
	void LoadAnimation()override;

private:
	GameObject* mTarget;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;				// �v���C���[�̍U�������蔻��{�b�N�X   
	EffectComponent* mEffect;		
	ChantEffect* mChantEffect;

	PlayerState                              mNowState;        // ���݂̃X�e�[�g
	PlayerState                              mNextState;       // ���̃X�e�[�g
	std::vector<PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	Vector3                                  mVelocityVec;

	static const float m_range;
	float mShootTimer;
	bool mAimMode;
	static const float mInterval;
};