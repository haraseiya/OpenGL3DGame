#pragma once
#include "GameObject.h"

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

	PLAYER_STATE_NUM,   // ���A�j���[�V������
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void UpdateActor(float deltaTime) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

private:
	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;				// �v���C���[�̍U�������蔻��{�b�N�X   
	EffectComponent* mEffect;		

	PlayerState                              mNowState;        // ���݂̃X�e�[�g
	PlayerState                              mNextState;       // ���̃X�e�[�g
	std::vector<PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	Vector3                                  mVelocityVec;

	static const float m_range;
};