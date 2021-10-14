#include "WeakEnemy.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Animation.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "PhysicsWorld.h"
#include "ExplosionEffect.h"
#include "InstanceMeshComponent.h"
#include "EnemyBullet.h"

// �G���G�X�e�[�g�֘A
#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"

#include <iostream>

const float WeakEnemy::mInterval = 2.0f;

WeakEnemy::WeakEnemy(GameObject* target)
	: mShootTimer(0.0f)
	, mTarget(target)
	, mBullet(nullptr)
{
	// �p�����[�^�[������
	mScale = 0.5f;
	mHitPoint = 5;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mIsOnGround = true;

	// ���f���ǂݍ���
	LoadModel();

	// �X�P���g���ǂݍ���
	LoadSkeleton();

	// �A�j���[�V�����ǂݍ���
	LoadAnimation();

	// EnemyBehavior�ɂӂ�܂���o�^
	BehaviorResister();

	// �����蔻���ǉ�
	SetCollider();
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "�G���G�j��" << std::endl;
}

void WeakEnemy::UpdateActor(float deltaTime)
{
	mTimer += deltaTime;

	// �G���G��{�F
	mSkelMeshComponent->SetHitColor(Color::Black);

	// 5�b�����Ƀv���C���[�Ɍ������Ĕ���
	mShootTimer += deltaTime;
	const bool isShot = mShootTimer > mInterval;
	if (isShot)
	{
		mShootTimer = 0.0f;

		Vector3 firePos;
		firePos = mDirection;
		firePos.z = 550.0f;

		mBullet = new EnemyBullet(this, 1.0f, 300.0f);
	}
}

// �����蔻��
void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// �����Ă���Tag���i�[
	Tag colliderTag = other->GetTag();

	// �w�i�ƏՓ˂����ꍇ
	if (colliderTag == Tag::BACK_GROUND)
	{
		// �C�����̈ʒu���i�[
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB enemyBox = mBoxCollider->GetWorldBox();
		AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, bgBox, fix);
	}

	// �Փˏ��
	CollisionInfo info;

	// ���̓G���v���C���[�ƏՓ˂����ꍇ
	if (colliderTag == Tag::ENEMY || colliderTag == Tag::PLAYER)
	{
		// �C�����̈ʒu������
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// �x�N�g����␳���Ȃ���߂�
		mPosition = Vector3::Lerp(mPosition, mPosition + fix, 0.1f);
		mPosition.z = 500.0f;

		// �ʒu�Čv�Z
		ComputeWorldTransform();
	}

	// �v���C���[�e�ƏՓ˂�����
	if (colliderTag == Tag::PLAYER_BULLET)
	{
		// ��e�F�Z�b�g
		mSkelMeshComponent->SetHitColor(Color::LightPink);
		mHitPoint--;
	}

	// �v���C���[�X�y�V�����V���b�g�ƏՓˎ�
	if (colliderTag == Tag::PLAYER_SPECIAL_SHOT)
	{
		mHitPoint -= 10;
	}
}

void WeakEnemy::FixCollision(BoxCollider* enemy, BoxCollider* player)
{
	//// �������Ƃ��̈ʒu
	//Vector3 fix;

	//// �ǂƂԂ������Ƃ�
	//AABB playerBox = hitPlayerBox->GetWorldBox();
	//AABB enemyBox = mHitBox->GetWorldBox();

	//// �߂荞�݂��C��
	//calcCollisionFixVec(playerBox, enemyBox, fix);

	//// �␳�x�N�g�����߂�
	//mPosition += fix;

	//// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	//mHitBox->OnUpdateWorldTransform();
}

void WeakEnemy::SetAttackHitBox(float scale)
{
	//// �U������p�{�b�N�X�̐���
	//mAttackBox = new BoxCollider(this);

	//// �G�O�������̓����蔻��
	//AABB box = mEnemyBox;
	//box.mMin *= 1.5;
	//box.mMax *= 1.5;
	//mAttackBox->SetObjectBox(box);
}

void WeakEnemy::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

void WeakEnemy::RemoveHitBox()
{
	if (mHitBox)
	{
		delete mHitBox;
		mHitBox = nullptr;
	}
}

void WeakEnemy::LoadModel()
{
	mMesh = RENDERER->GetMesh("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpmesh");
}

void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Character/Enemy/WeakEnemy/WeakSpider.gpskel"));
}

void WeakEnemy::LoadAnimation()
{
	// �A�j���[�V�����z��ɏ�Ԃ�ǉ�
	mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Spawn.gpanim",false));	
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Walk.gpanim", true));							
	mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Character/Enemy/Animation/Spider_Death.gpanim", false));
}

void WeakEnemy::BehaviorResister()
{
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Spawn);
}

void WeakEnemy::SetCollider()
{
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);
}


