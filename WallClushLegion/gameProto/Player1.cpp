#include "Player1.h"
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
#include "BulletManager.h"
#include "LaserEffect.h"

// �v���C���[�X�e�[�g�֘A
#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDie.h"

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mBullet(nullptr)
{
	printf("�v���C���[�P�쐬\n");

	mHitPoint = 100;

	// ���\�[�X�̓ǂݍ���
	LoadModel();
	LoadSkeleton();
	LoadAnimation();

	// �ӂ�܂���ǉ�
	BehaviorResister();

	// �����蔻��Z�b�g
	SetCollider();
}

Player1::~Player1()
{
	//�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	mAnimTypes.clear(); 
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void Player1::UpdateActor(float deltaTime)
{
	// �e�����Ă�Ȃ猂��
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;
		mBullet = new PlayerBullet(this);
	}

	// �X�y�V�����V���b�g�����Ă�Ȃ猂��
	mSpecialShotTimer += deltaTime;
	const bool isSpecialShot= INPUT_INSTANCE.IsKeyPressed(KEY_Y) && mSpecialShotTimer > mSpecialShotInterval;
	if (isSpecialShot)
	{
		mSpecialShotTimer = 0.0f;
		mLaser = new LaserEffect(this);
	}
}

SkeletalMeshComponent* Player1::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player1::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// ���f���̃��[�h
void Player1::LoadModel()
{
	//���b�V���̃��[�h
	mMesh = RENDERER->GetMesh("assets/Mesh/Player1.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

// �X�P���g���̃��[�h
void Player1::LoadSkeleton()
{
	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/Player1.gpskel"));
}

// �A�j���[�V�����̃��[�h
void Player1::LoadAnimation()
{
	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("assets/Animation/Player1_Idle.gpanim", false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("assets/Animation/Player1_Forward.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("assets/Animation/Player_Die2.gpanim", false);
}

// �ӂ�܂��̓o�^
void Player1::BehaviorResister()
{
	// �v���C���[�X�e�[�g�v�[���̏�����
	mPlayerBehavior = new PlayerBehaviorComponent(this);
	mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
	mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
}

// ���g�̃R���C�_�[�̃Z�b�g
void Player1::SetCollider()
{
	// �����蔻��Z�b�g
	mPlayerBox = mMesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	mPlayerBox.mMin.x *= 1.2f;
	mPlayerBox.mMin.y *= 1.2f;
	mPlayerBox.mMax.x *= 1.2f;
	mPlayerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(mPlayerBox);
}
