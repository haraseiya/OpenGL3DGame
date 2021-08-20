#include "WeakEnemy.h"
#include "Player.h"
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

#include "EnemyBehaviorComponent.h"
#include "EnemyIdle.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"

#include <iostream>

WeakEnemy::WeakEnemy(GameObject* target)
	: mCoolTime(0.0f)
	, mTarget(target)
{
	// �p�����[�^�[������
	mScale = 0.5f;
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 3;
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
	SetAttackTrigger();
}

WeakEnemy::~WeakEnemy()
{
	std::cout << "�{�X�G�j��" << std::endl;
}

void WeakEnemy::UpdateActor(float _deltaTime)
{
	// �O�������ɉ���������
	//if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	//{
	//	std::cout << "ForwardBoxHit!!" << std::endl;
	//}

	if (mHitPoint <= 0)
	{
		mState=STATE_DEAD;
	}
	mCoolTime += _deltaTime;
}

void WeakEnemy::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	Tag colliderTag = other->GetTag();

	// �����蔻��ŋA���Ă������ʂ�mHitBox�A�w�i�Ƃ̏Փ˂������ꍇ
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

		// �ʒu�Čv�Z
		ComputeWorldTransform();
	}

	// �v���C���[�e�ƏՓ˂�����
	if (colliderTag == Tag::PlayerBullet)
	{
		mSkelMeshComponent->SetHitColor(Color::Red);
		mHitPoint--;
	}

	//// �A�^�b�N�g���K�[�Ƀq�b�g������
	//if (other->GetTag() == Tag::NPC)
	//{
	//	if (mCoolTime > 3.0f)
	//	{
	//		mCoolTime = 0.0f;
	//		// �U���A�j���[�V�����ɃX�e�[�g�`�F���W
	//		m_enemyBehaviorComponent->ChangeState(EnemyStateEnum::Attack1);
	//	}
	//}

	//if (colliderTag == Tag::Enemy)
	//{
	//	Vector3 fix;

	//	//�ǂƂԂ������Ƃ�
	//	AABB enemyBox = mEnemyBox;
	//	AABB otherEnemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

	//	// �߂荞�݂��C��
	//	calcCollisionFixVec(enemyBox, otherEnemyBox, fix);

	//	// �␳�x�N�g�����߂�
	//	mPosition += fix;

	//	// �ʒu�Čv�Z
	//	ComputeWorldTransform();
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
	// �U������p�{�b�N�X�̐���
	mAttackBox = new BoxCollider(this);

	// �G�O�������̓����蔻��
	AABB box = mEnemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	mAttackBox->SetObjectBox(box);
}

void WeakEnemy::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

//bool BossEnemy::IsFrontHit()
//{
//	//return mAttackBox->IsTrigerHit();
//}

void WeakEnemy::LoadModel()
{
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/SK_Greater_Spider.gpmesh");
}

void WeakEnemy::LoadSkeleton()
{
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Mesh/SK_Greater_Spider.gpskel"));
}

void WeakEnemy::LoadAnimation()
{
	//mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/Greater_Spider_Walk.gpanim", true));
	//mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	//mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Death.gpanim", false));
}

void WeakEnemy::BehaviorResister()
{
	m_enemyBehaviorComponent = new EnemyBehaviorComponent(this);
	m_enemyBehaviorComponent->RegisterState(new EnemyIdle(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyPatrol(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyLookAround(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyChase(m_enemyBehaviorComponent, mTarget));
	m_enemyBehaviorComponent->RegisterState(new EnemyAttack(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->SetFirstState(EnemyStateEnum::Idle);
}

void WeakEnemy::SetCollider()
{
	mEnemyBox = mMesh->GetCollisionBox();
	mEnemyBox.mMin.y *= 0.5f;
	mEnemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(mEnemyBox);
	mHitBox->SetArrowRotate(true);

	AABB box = mMesh->GetCollisionBox();
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);
}

void WeakEnemy::SetAttackTrigger()
{
	mEnemyForward.mMin.x = mEnemyBox.mMax.x;
	mEnemyForward.mMin.y = mEnemyBox.mMin.y;
	mEnemyForward.mMin.z = mEnemyBox.mMin.z;
	mEnemyForward.mMax.x = mEnemyForward.mMin.x + 100.0f;
	mEnemyForward.mMax.y = mEnemyForward.mMin.y + 100.0f;
	mEnemyForward.mMax.z = mEnemyForward.mMin.z + 100.0f;
	mAttackTrigger = new BoxCollider(this);
	mAttackTrigger->SetObjectBox(mEnemyForward);
}
