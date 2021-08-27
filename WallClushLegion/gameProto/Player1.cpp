#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"
#include "ChantEffect.h"
#include "Bullet.h"

#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateRevive.h"

const float cAnimationSpeed = 0.5f;
const float Player::m_range = 10.0f;
const float Player::mInterval = 0.1f;

Player::Player()
	: GameObject(Tag::Player)
	, mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mShootTimer(0.0f)
{

}

Player::~Player()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void Player::UpdateActor(float deltaTime)
{
	const bool canChangeState = mNowState != mNextState;

	// �X�e�[�g�O������X�e�[�g�ύX�����������H
	if (canChangeState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// �X�e�[�g���s
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// �X�e�[�g��������X�e�[�g�ύX���������H
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// �G�����݂��Ȃ��Ȃ�Aim���[�h��~
	if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	{
		mAimMode = false;
		return;
	}
	if (!mAimMode)
	{
		mTarget = GAMEINSTANCE.GetEnemyActor();
	}

	// �^�[�Q�b�g���w��
	Vector3 aimPos, aimDir;
	aimPos = mTarget->GetPosition();

	//���g����G�Ɍ����������x�N�g�����v�Z
	aimDir = aimPos - mPosition;
	aimDir.z = 0.0f;

	// �v���[���[�Ə\������������Ȃ������ύX
	if (aimDir.LengthSq() > 0.5f)
	{
		aimDir.Normalize();
		//mDirection = aimDir;
	}

	// �e�����Ă�̂ł����
	mShootTimer += deltaTime;
	const bool isShot = mShootTimer > mInterval && INPUT_INSTANCE.GetInput(KEY_R) == KEY_STATE_PRESSED;
	if (isShot)
	{
		mShootTimer = 0.0f;
		Bullet* ba = new Bullet(mPosition, this->GetForward(), Tag::PlayerBullet);
	}
}

// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void Player::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* Player::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void Player::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// �^�O�ǉ�
	Tag colliderTag = other->GetTag();

	// �Փ˂������̂̃^�O���w�i�̏ꍇ
	if (colliderTag == Tag::BackGround)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// �ǂƂԂ������Ƃ�
			AABB playerBox = mHitBox->GetWorldBox();
			AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// �߂荞�݂��C��
			calcCollisionFixVec(playerBox, bgBox, fix);

			// �␳�x�N�g�����߂�
			mPosition += fix;

			// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
			ComputeWorldTransform();
		}
	}

	// �Փ˂������̂̃^�O���G�̏ꍇ
	if (colliderTag == Tag::Enemy)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// �ǂƂԂ������Ƃ�
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// �߂荞�݂��C��
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// �␳�x�N�g�����߂�
			mPosition += fix;

			// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
			ComputeWorldTransform();
		}
	}
}
