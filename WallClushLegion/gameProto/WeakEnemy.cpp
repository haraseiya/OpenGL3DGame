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

#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemySpawn.h"
#include "EnemyDeath.h"

#include <iostream>

const float WeakEnemy::mInterval = 0.01f;

WeakEnemy::WeakEnemy(GameObject* target)
	: mCoolTime(0.0f)
	, mTarget(target)
	, mTimer(0.0f)
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

	// �U���p�g���K�[�ǉ�
	//SetAttackTrigger();
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "�{�X�G�j��" << std::endl;
}

void WeakEnemy::UpdateActor(float deltaTime)
{
	// �O�������ɉ���������
	//if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	//{
	//	std::cout << "ForwardBoxHit!!" << std::endl;
	//}

	// HP��0�ɂȂ�����
	if (mHitPoint <= 0)
	{
		//mExplosion = new ExplosionEffect(mPosition);
	}

	mCoolTime += deltaTime;
	mTimer += deltaTime;
}

void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// �����Ă���Tag���i�[
	Tag colliderTag = other->GetTag();

	// �w�i�ƏՓ˂����ꍇ
	if (colliderTag == Tag::BackGround)
	{
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB enemyBox = mBoxCollider->GetWorldBox();
		AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, bgBox, fix);
	}

	// �Փˏ��
	CollisionInfo info;

	// �G�ƏՓ˂�����
	if (colliderTag==Tag::Enemy)
	{
		Vector3 fix;

		//�ǂƂԂ������Ƃ�
		AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();
		AABB enemyBox = mHitBox->GetWorldBox();

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

		// �␳�x�N�g�����߂�
		mPosition += fix;
		mPosition.z = 500.0f;
		// �ʒu�Čv�Z
		ComputeWorldTransform();
	}

	// �G���G��{�F
	mSkelMeshComponent->SetHitColor(Color::Black);

	// �v���C���[�e�ƏՓ˂�����
	if (colliderTag == Tag::PlayerBullet)
	{
		// ��e�F�Z�b�g
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

	// �v���C���[�Ƃ̏Փ�
	//if (colliderTag == Tag::Player)
	//{
	//	// �U�����[�V�����Ƀ`�F���W
	//	mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Attack1);
	//}
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

//bool BossEnemy::IsFrontHit()
//{
//	//return mAttackBox->IsTrigerHit();
//}

void WeakEnemy::LoadModel()
{
	mMesh = RENDERER->GetMesh("Assets/Mesh/SK_Greater_Spider.gpmesh");
}

void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Mesh/SK_Greater_Spider.gpskel"));
}

void WeakEnemy::LoadAnimation()
{
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));				// �ҋ@
	mAnimations.emplace(EnemyStateEnum::Spawn, RENDERER->GetAnimation("Assets/Animation/ExoGame_Greater_Spider_Spawn.gpanim",false));	// �o��
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));				// ����
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));				// ����
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Greater_Spider_Attack_Melee.gpanim", false));		// �U��
	mAnimations.emplace(EnemyStateEnum::Death, RENDERER->GetAnimation("Assets/Animation/ExoGame_Greater_Spider_Death.gpanim", false));	// ���S
}

void WeakEnemy::BehaviorResister()
{
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyIdle(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyPatrol(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent, mTarget));
	mEnemyBehaviorComponent->RegisterState(new EnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemySpawn(mEnemyBehaviorComponent));
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

	//AABB box = mMesh->GetCollisionBox();
	//mBoxCollider = new BoxCollider(this);
	//mBoxCollider->SetObjectBox(box);
}

void WeakEnemy::SetAttackTrigger()
{
	//mEnemyForward.mMin.x = mEnemyBox.mMax.x;
	//mEnemyForward.mMin.y = mEnemyBox.mMin.y;
	//mEnemyForward.mMin.z = mEnemyBox.mMin.z;
	//mEnemyForward.mMax.x = mEnemyForward.mMin.x + 100.0f;
	//mEnemyForward.mMax.y = mEnemyForward.mMin.y + 100.0f;
	//mEnemyForward.mMax.z = mEnemyForward.mMin.z + 100.0f;
	//mAttackTrigger = new BoxCollider(this);
	//mAttackTrigger->SetObjectBox(mEnemyForward);
}

