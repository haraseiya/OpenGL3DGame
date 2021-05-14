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

class Player : public GameObject
{
public:
	Player();
	~Player();

	void UpdateActor(float deltaTime) override;
	void                         FixCollision(class BoxCollider* hitPlayerBox, class BoxCollider* hitBox);
	class SkeletalMeshComponent* GetSkeletalMeshComp();
	const class Animation* GetAnim(PlayerState state);

private:
	class SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider* mHitBox;
	class BoxCollider* mHitTrigger;
	class BoxCollider* mHitGroundBox;
	class BoxCollider* mHitHeadBox;
	class BoxCollider* mAttackBox;				// �v���C���[�̍U�������蔻��{�b�N�X   
	class EffectComponent* mEffect;		

	PlayerState                              mNowState;        // ���݂̃X�e�[�g
	PlayerState                              mNextState;       // ���̃X�e�[�g
	std::vector<class PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	Vector3                                  mVelocityVec;

	static const float m_range;
};