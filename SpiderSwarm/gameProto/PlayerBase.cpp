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
#include "SceneBase.h"

#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed=0.5f;
const float PlayerBase::mInterval = 0.1f;
const float PlayerBase::mInvincibleTime = 3.0f;
const float PlayerBase::mSpecialShotInterval = 5.0f;

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
	, mShootTimer(0.0f)
	, mIsDeadAnimFinish(false)
	, mPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE)
{	
	// �傫����100����1��
	mScale = 0.01f;
	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	mInvincibleTimer += deltaTime;
	mMeshComp->SetVisible(false);
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// �v���C���[�Փ˔���
void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// ���������I�u�W�F�N�g�̃^�O�擾
	Tag colliderTag = other->GetTag();

	const bool isHitBullet = colliderTag == Tag::ENEMY_BULLET && mInvincibleTimer >= mInvincibleTime;
	// �G�̒e�ɓ���������v���C���[�̗̑͂��P���炷
	if (isHitBullet)
	{
		mInvincibleTimer = 0.0f;
		
		mMeshComp->SetVisible(false);

		mHitPoint--;
	}

	// �Փ˂����̂��w�i�̏ꍇ
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
		// �ʂ̃^�C�v���{�b�N�X�̏ꍇ
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// �ǂƂԂ������Ƃ�
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// �߂荞�݂��C��
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// �␳�x�N�g�����߂�
			mPosition = Vector3::Lerp(mPosition,mPosition+fix,0.1);
			mPosition.z = 500.0f;

			// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
			ComputeWorldTransform();
		}
	}
}

// �v���C���[�̃X�e�[�g�����擾
const char* PlayerBase::GetPlayerStateEnumName(PlayerStateEnum state)
{
	return playerStateEnumName[static_cast<int>(state)];
}

void PlayerBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}