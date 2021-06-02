#include "BossEnemy.h"
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

BossEnemy::BossEnemy(NPCActorBase* npc)
{
	// �p�����[�^�[������
	mScale = Vector3(3.0f,3.0f,3.0f);
	mWalkSpeed = 500.0f;
	mRunSpeed = 500.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 1000;
	mIsOnGround = true;

	// ���f���ǂݍ���
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	mMesh = RENDERER->GetMesh("Assets/Mesh/Enemy_Bear.gpmesh");

	// �X�P���g���ǂݍ���
	mSkelMeshComponent->SetMesh(mMesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Enemy_Bear.gpskel"));

	// �A�j���[�V�����ǂݍ���
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Idle.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Run, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Walk.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Melee.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Animation/ExoGame_Bears_Attack_Death.gpanim", false));

	// EnemyBehavior�ɂӂ�܂���o�^
	m_enemyBehaviorComponent = new EnemyBehaviorComponent(this);
	m_enemyBehaviorComponent->RegisterState(new EnemyIdle(m_enemyBehaviorComponent, npc));
	m_enemyBehaviorComponent->RegisterState(new EnemyPatrol(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyLookAround(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->RegisterState(new EnemyChase(m_enemyBehaviorComponent,npc));
	m_enemyBehaviorComponent->RegisterState(new EnemyAttack(m_enemyBehaviorComponent));
	m_enemyBehaviorComponent->SetFirstState(EnemyStateEnum::Idle);

	// �G�L�����̓����蔻���ǉ�
	m_enemyBox = mMesh->GetCollisionBox();
	m_enemyBox.mMin.y *= 0.5f;
	m_enemyBox.mMax.y *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(m_enemyBox);
	mHitBox->SetArrowRotate(true);
}

BossEnemy::~BossEnemy()
{
	std::cout << "�{�X�G�j��" << std::endl;
}

void BossEnemy::UpdateActor(float _deltaTime)
{
	// �O�������ɉ���������
	if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	{
		std::cout << "ForwardBoxHit!!" << std::endl;
	}

	if (mHitPoint <= 0)
	{
		this->EDead;
	}
}

void BossEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// �����蔻��ŋA���Ă������ʂ�mHitBox�A�w�i�Ƃ̏Փ˂������ꍇ
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumBG)
	{
		AABB bgBox = hitOtherBox->GetWorldBox();
		AABB thisBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		calcCollisionFixVec(thisBox, bgBox, fixVec);
		mPosition += fixVec;
		mHitBox->OnUpdateWorldTransform();
	}

	// NPC�Ƃ̓����蔻��
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumNPC)
	{
		m_enemyBehaviorComponent->ChangeState(EnemyStateEnum::Attack1);
	}

	if (mHitBox==hitThisBox&&
		hitOtherBox->GetType()==EnumPhysicsType::EnumNPCAttackBox)
	{
		mHitPoint -= 10;
	}
}

void BossEnemy::FixCollision(BoxCollider* hitEnemyBox, BoxCollider* hitPlayerBox)
{
	// �������Ƃ��̈ʒu
	Vector3 fix;

	// �ǂƂԂ������Ƃ�
	AABB playerBox = hitPlayerBox->GetWorldBox();
	AABB enemyBox = mHitBox->GetWorldBox();

	// �߂荞�݂��C��
	calcCollisionFixVec(playerBox, enemyBox, fix);

	// �␳�x�N�g�����߂�
	mPosition += fix;

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	mHitBox->OnUpdateWorldTransform();
}

void BossEnemy::SetAttackHitBox(float scale)
{
	m_attackBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyAttackBox);

	// �G�O�������̓����蔻��
	AABB box = m_enemyBox;
	box.mMin *= 1.5;
	box.mMax *= 1.5;
	m_attackBox->SetObjectBox(box);
}

void BossEnemy::RemoveAttackHitBox()
{
	if (m_attackBox)
	{
		delete m_attackBox;
		m_attackBox = nullptr;
	}
}

bool BossEnemy::IsFrontHit()
{
	return m_attackBox->IsTrigerHit();
}
