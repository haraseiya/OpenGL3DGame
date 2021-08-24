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
#include "Bullet.h"

#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

PlayerBase::PlayerBase()
	: GameObject(Tag::Player)
{	// �傫����100����1��
	mScale = 0.01f;

	//���b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Player.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Player.gpskel"));

	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Animation/Player_Idle.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)] = RENDERER->GetAnimation("Assets/Animation/Player_Running.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_REVIVE)] = RENDERER->GetAnimation("Assets/Animation/Player_Revive.gpanim", true);

	// �A�C�h����ԃA�j���[�V�������Z�b�g
	mMeshComp->PlayAnimation(mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// �v���C���[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new PlayerStateIdle);			// �ҋ@���
	mStatePools.push_back(new PlayerStateRun);			// ������	

	// �����蔻��Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// �v���[���[�̑����𒲂ׂ�{�b�N�X���쐬�@�{�b�N�X��1/4, �{�b�N�X��ʂ����_�ɗ���悤�ɂ���
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //�W�����v���Ɉ���������Ȃ�����
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this);
	mHitGroundBox->SetObjectBox(groundBox);

	// �v���[���[�̓���𒲂ׂ�{�b�N�X���쐬 �{�b�N�X��ʂ�����ɗ���悤�ɂ���
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this);
	mHitHeadBox->SetObjectBox(headBox);

	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);

}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
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

