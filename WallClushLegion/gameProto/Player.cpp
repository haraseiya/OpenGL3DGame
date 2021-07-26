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

Player::Player()
	: GameObject(Tag::Player)
	, mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
{
	// �傫����100����1��
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
	mStatePools.push_back(new PlayerStateRevive);		// �h�����

	// �����蔻��Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// �r���͈͗p�g���K�[
	AABB playerTriggerBox = mesh->GetCollisionBox();
	mHitTrigger = new BoxCollider(this);
	playerTriggerBox.mMin.x *= 5.0f;
	playerTriggerBox.mMax.x *= 5.0f;
	playerTriggerBox.mMin.y *= 10.0f;
	playerTriggerBox.mMax.y *= 10.0f;

	mHitTrigger->SetObjectBox(playerTriggerBox);
	mHitTrigger->SetArrowRotate(false);

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

	//�L�[�������ꂽ
	if (INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PUSHDOWN)
	{
		//���ˈʒu���v�Z
		Vector3 firePos;
		firePos = mDirection * 100.0f;
		firePos.z = 100.0f;

		Bullet* bullet = new Bullet(firePos + mPosition, mDirection, Tag::PlayerBullet);
	}
	//if (INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PUSHDOWN)
	//{
	//	ChantEffect* effect = new ChantEffect(this,true);
	//}
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

void Player::OnCollisionEnter(ColliderComponent* other)
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
