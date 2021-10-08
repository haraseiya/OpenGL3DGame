#include "PlayerBase.h"
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
#include "PlayerBullet.h"

#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed=0.5f;
const float PlayerBase::mInterval = 0.1f;

// ��ԂɕύX���������炱������ύX
const char* playerStateEnumName[static_cast<int>(PlayerStateEnum::StateNum)] =
{
	"PlayerStateEnum::Invalid",
	"PlayerStateEnum::Spawn",
	"PlayerStateEnum::Idle",
	"PlayerStateEnum::Walk",
	"PlayerStateEnum::Turn",
	"PlayerStateEnum::Run",
	"PlayerStateEnum::LookAround"
	"PlayerStateEnum::Attack1",
	"PlayerStateEnum::Attack2",
	"PlayerStateEnum::Attack3",
	"PlayerStateEnum::GetDamage",
	"PlayerStateEnum::Roar",
	"PlayerStateEnum::Stun",
	"PlayerStateEnum::Die",

	"PlayerStateEnum::StateNum"
};

PlayerBase::PlayerBase()
	: GameObject(Tag::PLAYER)
	, mScenePlayer(PlayerSceneState::PLAYER_TITLESCENE)
	, mShootTimer(0.0f)
	, mIsDeadAnimFinish(false)
{	
	// �傫����100����1��
	mScale = 0.01f;
	mHitPoint = 10;
	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	// �V���b�g�\�ł���Βe�𐶐�
	//mShootTimer += deltaTime;
	//const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	//if (isShoot)
	//{
	//	mShootTimer = 0.0f;
	//	mPlayerBullet = new PlayerBullet(shotPos1, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PLAYER_BULLET);
	//	//mBullet = new Bullet(shotPos2, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	//	//mBullet = new Bullet(shotPos3, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	//}
	//// �X�e�[�g�`�F���W�\�ł����true
	//const bool canChangeState = mNowState != mNextState;

	//// �X�e�[�g�O������X�e�[�g�ύX�����������H
	//if (canChangeState)
	//{
	//	mStatePools[static_cast<unsigned int>(mNowState)]->OnExit();
	//	mStatePools[static_cast<unsigned int>(mNextState)]->OnEnter();
	//	mNowState = mNextState;
	//	return;
	//}

	//// �X�e�[�g���s
	//mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(deltaTime);

	//// �X�e�[�g��������X�e�[�g�ύX���������H
	//if (mNowState != mNextState)
	//{
	//	mStatePools[static_cast<unsigned int>(mNowState)]->OnExit();
	//	mStatePools[static_cast<unsigned int>(mNextState)]->OnEnter();
	//	mNowState = mNextState;
	//}

	//// �G�����݂��Ȃ��Ȃ�Aim���[�h��~
	//if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	//{
	//	mAimMode = false;
	//	return;
	//}
	//if (!mAimMode)
	//{
	//	mTarget = GAMEINSTANCE.GetEnemyActor();
	//}

	//// �^�[�Q�b�g���w��
	//Vector3 aimPos, aimDir;
	//aimPos = mTarget->GetPosition();

	////���g����G�Ɍ����������x�N�g�����v�Z
	//aimDir = aimPos - mPosition;
	//aimDir.z = 0.0f;

	//// �v���[���[�Ə\������������Ȃ������ύX
	//if (aimDir.LengthSq() > 0.5f)
	//{
	//	aimDir.Normalize();
	//	//mDirection = aimDir;
	//}
}

// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void PlayerBase::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// �^�O�ǉ�
	Tag colliderTag = other->GetTag();

	// �Փ˂������̂̃^�O���w�i�̏ꍇ
	if (colliderTag == Tag::BACK_GROUND)
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
	if (colliderTag == Tag::ENEMY)
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

const char* PlayerBase::GetPlayerStateEnumName(PlayerStateEnum state)
{
	return playerStateEnumName[static_cast<int>(state)];
}
