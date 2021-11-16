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
#include "AttachMeshComponent.h"
#include "Weapon.h"

// �v���C���[�X�e�[�g�֘A
#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDie.h"
#include "PlayerStateHold.h"
#include "PlayerStateForward.h"
#include "PlayerStateRunForward.h"

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mBullet(nullptr)
{
	printf("�v���C���[�P�쐬\n");

	// �̗�
	mLevel = 1;
	mHitPoint = 100;

	// �v���C���[�X�e�[�g�v�[���̏�����
	mPlayerBehavior = new PlayerBehaviorComponent(this);

	// ���\�[�X�̓ǂݍ���
	LoadModel();
	LoadSkeleton();
	LoadAnimation();
	//AttachWeapon();

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
	// ���x���A�b�v�ɕK�v�Ȍo���l���W�߂��烌�x���A�b�v
	const bool isLevelUp = mExperience >= mRequireExprience;
	if (isLevelUp)
	{
		mExperience = 0;
		mLevel++;
	}

	// �e�����Ă�Ȃ�
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;

		if (mLevel == 1)
		{
			mBullet = new PlayerBullet(this, mPosition, GetForward(), 1.0, mBulletSpeed);
		}
		else if (mLevel == 2)
		{
			Vector3 upperRight = Vector3(1.0f, 0.3, 0.0f);
			Vector3 upperLeft = Vector3(1.0f, -0.3, 0.0f);
			upperRight.Normalize();
			upperLeft.Normalize();
			mBullet = new PlayerBullet(this, mPosition, GetForward(), 0.3, 1000);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperRight), 0.3, mBulletSpeed);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperLeft), 0.3, mBulletSpeed);
		}
		else if (mLevel >= 3)
		{
			Vector3 upperRight1 = Vector3(1.0f, 0.3, 0.0f);
			Vector3 upperLeft1 = Vector3(1.0f, -0.3, 0.0f);
			Vector3 upperRight2 = Vector3(1.0f, 0.5, 0.0f);
			Vector3 upperLeft2 = Vector3(1.0f, -0.5, 0.0f);
			upperRight1.Normalize();
			upperLeft1.Normalize();
			upperRight2.Normalize();
			upperLeft2.Normalize();

			mBullet = new PlayerBullet(this, mPosition, GetForward(), 0.3, 1000);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperRight1), 0.3, mBulletSpeed);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperLeft1), 0.3, mBulletSpeed);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperRight2), 0.3, mBulletSpeed);
			mBullet = new PlayerBullet(this, mPosition, GetDirectionFromForward(upperLeft2), 0.3, mBulletSpeed);
		}
	}
	// �X�y�V�����V���b�g�����Ă�Ȃ�
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
	mMesh = RENDERER->GetMesh("Assets/Character/Player1/Player1.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

// �X�P���g���̃��[�h
void Player1::LoadSkeleton()
{
	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Player1/Player1.gpskel"));
}

// �A�j���[�V�����̃��[�h
void Player1::LoadAnimation()
{
	// �A�j���[�V�����������̃��������m��
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));

	// �V�[�����̃A�j���[�V�����ǂݍ���
	switch (mPlayerSceneState)
	{
	// �^�C�g���V�[��
	case PlayerSceneState::PLAYER_TITLESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HOLD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Hold.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateHold(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Hold);
		break;  

	// �Z���N�g�V�[��
	case PlayerSceneState::PLAYER_SELECTSCENE:
		// �I�𒆂̃A�j���[�V����

		// �I����̃A�j���[�V����

		break;

	// �Q�[���V�[��
	case PlayerSceneState::PLAYER_GAMESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Idle.gpanim", true);
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Forward.gpanim", true);
		// �c��3�������̃A�j���[�V����

		// 4�������̑���A�j���[�V����
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Sprint_Forward.gpanim", true);

		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("Assets/Character/Player1/Animation/Player1_Death.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
		break;

	// ���U���g�V�[��
	case PlayerSceneState::PLAYER_RESULT:
		// �����A�j���[�V����

		break;
	}
}

// �ӂ�܂��̓o�^
void Player1::BehaviorResister()
{
}

// ���g�̃R���C�_�[�̃Z�b�g
void Player1::SetCollider()
{
	// �����蔻��Z�b�g
	mPlayerBox = mMesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	//mPlayerBox.mMin.x *= 1.0f;
	//mPlayerBox.mMin.y *= 1.0f;
	//mPlayerBox.mMax.x *= 1.0f;
	//mPlayerBox.mMax.y *= 1.0f;
	mHitBox->SetObjectBox(mPlayerBox);
}

// �v���C���[�ɕ�����A�^�b�`
void Player1::AttachWeapon()
{
	AttachMeshComponent* attachMesh;
	attachMesh = new AttachMeshComponent(this, mMeshComp, "hand_l");
	attachMesh->SetMesh(RENDERER->GetMesh("Assets/Weapon/AK-47/AK-47.gpmesh"));

	Vector3 rot, pos;
	rot = Vector3(-Math::PiOver2 * 0.5f, Math::Pi, 0);
	pos = Vector3(0, 0, 0);
	attachMesh->SetOffsetRotation(rot);
	attachMesh->SetOffsetPosition(pos);
}
